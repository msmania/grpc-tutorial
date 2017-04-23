#include "stub/helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::Status;
using grpc::ServerContext;

using helloworld::Greeter;
using helloworld::HelloRequest;
using helloworld::HelloReply;

class GreeterClient {
private:
  std::unique_ptr<Greeter::Stub> stub;

public:
  GreeterClient(std::shared_ptr<Channel> channel);
  std::string SayHello(const std::string &user);
  std::string SayHelloAgain(const std::string &user);
};

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext *context,
                  const HelloRequest *request,
                  HelloReply *reply) override;
  Status SayHelloAgain(ServerContext *context,
                       const HelloRequest *request,
                       HelloReply *reply) override;
};
