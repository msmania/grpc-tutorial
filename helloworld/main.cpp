#include <string.h>

void RunClient();
void RunServer();

int main(int argc, char *argv[]) {
  if (argc >= 2) {
    if (strcmp(argv[1], "-c") == 0) {
      RunClient();
    }
    else if (strcmp(argv[1], "-s") == 0) {
      RunServer();
    }
  }
  return 0;
}
