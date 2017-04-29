class string_exchanger final : public grpc_tutorial::StringExchanger::Service {
private:
  static void *thread_worker_start(void *context);
  static void timer_handler_start(union sigval context);

  std::unique_ptr<grpc_tutorial::StringExchanger::Stub> stub;
  pthread_t thread;
  timer_t timer;
  pthread_mutex_t data_access_lock;
  int rpc_timeout_ms;
  std::string listenport;
  std::string destination;
  std::vector<std::string> data;

  void thread_worker();
  void timer_handler();
  void wait();
  void add(std::string &&new_item);

  grpc::Status Push(grpc::ServerContext *context,
                    const grpc_tutorial::String *request,
                    grpc_tutorial::Reply *reply) override;

public:
  string_exchanger(const char *listenport,
                   const char *destination,
                   int rpc_call_timeout_ms);
  ~string_exchanger();

  std::vector<std::string> &get_data();
  int dispatch_command(std::string &&command);

};
