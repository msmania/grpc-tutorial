#include <readline/readline.h>
#include <readline/history.h>
#include <grpc/support/log.h>
#include <grpc++/grpc++.h>
#include "stub/tutorial.grpc.pb.h"
#include "exchanger.hpp"

int main(int argc, char *argv[]) {
  // Prevent gRPC's log messages from being printed on the console
  gpr_set_log_verbosity(GPR_LOG_SEVERITY_ERROR);
  gpr_set_log_function([](gpr_log_func_args*) {});

  if (argc >= 3) {
    std::unique_ptr<string_exchanger> server(
      new string_exchanger(argv[1], argv[2], /*rpc_timeout_ms*/500));

    for (int quit_if_nonzero = 0; !quit_if_nonzero;) {
      char *input = readline(">> ");
      if (!input) {
        puts("quit\n");
        break;
      }
      add_history(input);
      quit_if_nonzero = server->dispatch_command(input);
      free(input);
    }

    for (auto it : server->get_data()) {
      std::cout << it << std::endl;
    }
  }
  return 0;
}
