#include <string.h>
#include <grpc/support/log.h>

void RunClient(const char *endpoint);
void RunServer(const char *endpoint);

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
    if (strcmp(argv[1], "-c") == 0) {
      RunClient(endpoint);
    }
    else if (strcmp(argv[1], "-s") == 0) {
      RunServer(endpoint);
    }
  }
  return 0;
}
