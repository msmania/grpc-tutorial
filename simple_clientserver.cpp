#include <string.h>
#include <memory>
#include <iostream>

#include <grpc++/grpc++.h>

#include "helloworld.h"

using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;

const char ENDPOINT[] = "localhost:50051";
const char LISTEN_PORT[] = "0.0.0.0:50051";

GreeterClient::GreeterClient(std::shared_ptr<Channel> channel)
  : stub(Greeter::NewStub(channel)) {}

std::string GreeterClient::SayHello(const std::string &user) {
  HelloRequest request;
  request.set_name(user);

  HelloReply reply;
  ClientContext context;

  auto status = stub->SayHello(&context, request, &reply);
  if (status.ok()) {
    return reply.message();
  }
  else {
    std::cerr << status.error_code() << ": " << status.error_message()
      << std::endl;
    return "RPC failed";
  }
}

std::string GreeterClient::SayHelloAgain(const std::string &user) {
  HelloRequest request;
  request.set_name(user);

  HelloReply reply;
  ClientContext context;

  auto status = stub->SayHelloAgain(&context, request, &reply);
  if (status.ok()) {
    return reply.message();
  }
  else {
    std::cerr << status.error_code() << ": " << status.error_message()
      << std::endl;
    return "RPC failed";
  }
}

Status GreeterServiceImpl::SayHello(ServerContext *context,
                                    const HelloRequest *request,
                                    HelloReply *reply) {
  std::string prefix("Hello ");
  reply->set_message(prefix + request->name());
  return Status::OK;
}


Status GreeterServiceImpl::SayHelloAgain(ServerContext *context,
                                         const HelloRequest *request,
                                         HelloReply *reply) {
  std::string prefix("Hello again ");
  reply->set_message(prefix + request->name());
  return Status::OK;
}

void RunAsClient() {
  GreeterClient greeter(grpc::CreateChannel(ENDPOINT,
                                            grpc::InsecureChannelCredentials()));
  std::string user("world");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  reply = greeter.SayHelloAgain(user);
  std::cout << "Greeter received: " << reply << std::endl;
}

void RunAsServer() {
  GreeterServiceImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(LISTEN_PORT, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << LISTEN_PORT << std::endl;

  server->Wait();
}
