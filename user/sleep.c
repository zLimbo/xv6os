// Simple sleep

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(2, "usage: sleep [nclock]\n");
    exit(1);
  }

  int nclock = atoi(argv[1]);
  sleep(nclock);

  exit(0);
}
