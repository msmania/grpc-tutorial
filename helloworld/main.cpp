#include <string.h>
#include <grpc/support/log.h>
#include <string>

void RunClient(const char *endpoint, int timeout_ms);
void RunServer(const char *endpoint);
void RunAsyncClient(const char *endpoint, int timeout_ms);
void RunAsyncServer(const char *endpoint);

int main(int argc, char *argv[]) {
  if (getenv("SILENT")) {
    // silent mode
    gpr_set_log_verbosity(GPR_LOG_SEVERITY_ERROR);
    gpr_set_log_function([](gpr_log_func_args*) {});
  }
  else {
    // verbose mode
    gpr_set_log_verbosity(GPR_LOG_SEVERITY_DEBUG);
    //  nullptr means using the default logger
    gpr_set_log_function(nullptr);
  }

  if (argc >= 2) {
    auto endpoint = argc >= 3 ? argv[2] : nullptr;
    int client_timeout = 0;
    try {
      client_timeout = std::stoi(getenv("TIMEOUT"));
    }
    catch (...) {
      client_timeout = 3600000;
    }
    if (strcmp(argv[1], "-c") == 0) {
      RunClient(endpoint, client_timeout);
    }
    else if (strcmp(argv[1], "-s") == 0) {
      RunServer(endpoint);
    }
    else if (strcmp(argv[1], "-ac") == 0) {
      RunAsyncClient(endpoint, client_timeout);
    }
    else if (strcmp(argv[1], "-as") == 0) {
      RunAsyncServer(endpoint);
    }
  }
  return 0;
}
