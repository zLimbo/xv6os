#include "kernel/types.h"
#include "user/user.h"

void prime_sieve(int from) {
  int x = 0, nbytes = 0;
  nbytes = read(from, &x, sizeof(x));
  if (nbytes == 0) {
    close(from);
    return;
  }
  printf("prime %d\n", x);
  int y = 0;
  nbytes = read(from, &y, sizeof(y));
  if (nbytes == 0) {
    close(from);
    return;
  }
  int p[2];
  pipe(p);
  if (fork() == 0) {
    close(p[1]);
    prime_sieve(p[0]);
    exit(0);
  }
  close(p[0]);
  do {
    if (y % x) {
      write(p[1], &y, sizeof(y));
    }
    nbytes = read(from, &y, sizeof(y));
  } while (nbytes != 0);
  close(from);
  close(p[1]);
  wait(0);
}

int main() {
  int p[2];
  pipe(p);

  if (fork() == 0) {
    close(p[1]);
    prime_sieve(p[0]);
    exit(0);
  }
  close(p[0]);
  for (int i = 2; i <= 35; ++i) {
    write(p[1], &i, sizeof(i));
  }
  close(p[1]);
  wait(0);

  return 0;
}