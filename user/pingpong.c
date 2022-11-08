#include "kernel/types.h"
#include "user/user.h"

int main() {
  int ppc[2];
  int pcp[2];
  pipe(ppc);
  pipe(pcp);

  if (fork() == 0) {
    close(ppc[1]);
    close(pcp[0]);
    char data;
    read(ppc[0], &data, 1);
    int pid = getpid();
    printf("%d: received ping\n", pid);
    write(pcp[1], &data, 1);
    exit(0);
  } else {
    close(ppc[0]);
    close(pcp[1]);
    char data = 'a';
    write(ppc[1], &data, 1);
    read(pcp[0], &data, 1);
    int pid = getpid();
    printf("%d: received pong\n", pid);
    exit(0);
  }

  return 0;
}