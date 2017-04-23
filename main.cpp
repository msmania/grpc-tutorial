#include <string.h>
#include <iostream>

void RunAsClient();
void RunAsServer();

int main(int argc, char *argv[]) {
  if (argc >= 2) {
    if (strcmp(argv[1], "-s") == 0) {
      RunAsServer();
    }
    else if (strcmp(argv[1], "-c") == 0) {
      RunAsClient();
    }
  }
  return 0;
}
