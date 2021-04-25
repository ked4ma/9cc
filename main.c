#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "arg num is invalid\n");
    return 1;
  }

  // tokenize
  user_input = argv[1];
  token = tokenize();
  program();

  codegen();
  return 0;
}
