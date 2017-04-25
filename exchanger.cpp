#include <grpc++/grpc++.h>
#include "stub/tutorial.grpc.pb.h"
#include "exchanger.hpp"

void string_exchanger::thread_worker() {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(listenport, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
}

void *string_exchanger::thread_worker_start(void *context) {
  if (context) {
    reinterpret_cast<string_exchanger*>(context)->thread_worker();
  }
  pthread_exit(nullptr);
}

void string_exchanger::timer_handler() {
  // We don't wait for the lock.  Just give up if someone owns it.
  if (pthread_mutex_trylock(&data_access_lock) == 0) {
    if (data.size() > 0) {
      grpc_tutorial::String request;
      request.set_data(data[rand() % data.size()]);

      grpc::ClientContext context;
      context.set_deadline(std::chrono::system_clock::now()
                           + std::chrono::milliseconds(rpc_timeout_ms));

      grpc_tutorial::Reply reply;
      stub->Push(&context, request, &reply); // ignore the result
    }
    pthread_mutex_unlock(&data_access_lock);
  }
}

void string_exchanger::timer_handler_start(union sigval context) {
  if (context.sival_ptr) {
    reinterpret_cast<string_exchanger*>(context.sival_ptr)->timer_handler();
  }
}

void string_exchanger::wait() {
  if (thread) {
    pthread_join(thread, nullptr);
    thread = 0;
  }
}

void string_exchanger::add(std::string &&new_item) {
  if (pthread_mutex_lock(&data_access_lock) == 0) {
    data.push_back(new_item);
    pthread_mutex_unlock(&data_access_lock);
  }
}

grpc::Status string_exchanger::Push(grpc::ServerContext *context,
                                    const grpc_tutorial::String *request,
                                    grpc_tutorial::Reply *reply) {
  add(std::string(request->data()));
  reply->set_status(0);
  return grpc::Status::OK;
}

string_exchanger::string_exchanger(const char *listenport,
                                   const char *destination,
                                   int rpc_timeout_ms)
  : stub(grpc_tutorial::StringExchanger::NewStub(
           grpc::CreateChannel(destination,
                               grpc::InsecureChannelCredentials()))),
    thread(0),
    timer(0),
    rpc_timeout_ms(rpc_timeout_ms),
    listenport(listenport),
    destination(destination),
    data() {
  pthread_mutex_init(&data_access_lock, nullptr);
  pthread_t thread_temp;
  if (pthread_create(&thread_temp, nullptr, thread_worker_start, this) == 0) {
    thread = thread_temp;
  }

  struct sigevent sev = {};
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = timer_handler_start;
  sev.sigev_value.sival_ptr = this;

  timer_t timer_temp;
  if (timer_create(CLOCK_REALTIME, &sev, &timer_temp) == 0) {
    struct itimerspec spec = {};
    spec.it_interval.tv_sec = 1;
    spec.it_value.tv_sec = 1;
    if (timer_settime(timer_temp, 0, &spec, nullptr) == -1) {
      timer_delete(timer_temp);
      timer_temp = 0;
    }
  }
  timer = timer_temp;
}

string_exchanger::~string_exchanger() {
  pthread_cancel(thread);
  wait();

  pthread_mutex_destroy(&data_access_lock);
  if (timer) {
    timer_delete(timer);
    timer = 0;
  }
}

std::vector<std::string> &string_exchanger::get_data() {
  return data;
}

int string_exchanger::dispatch_command(std::string &&command) {
  int quit_if_nonzero = 0;
  if (command.size() > 1 && command[0] == '+') {
    add(command.substr(1));
  }
  return quit_if_nonzero;
}
