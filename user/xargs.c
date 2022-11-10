#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  //   printf("argc: %d\n", argc);
  //   for (int i = 0; i < argc; ++i) {
  //     printf("argv[%d]: %s\n", i, argv[i]);
  //   }
  //   printf("\n");
  if (argc + 1 >= MAXARG) {
    fprintf(2, "xargs: number of args is too long!\n");
    exit(1);
  }
  char buf[512];
  char *param[MAXARG];
  for (int i = 1; i < argc; ++i) {
    param[i - 1] = argv[i];
  }
  char *start = buf, *cur = buf;
  int nbytes = 0;
  //   int cnt = 0;
  //   int map[24];
  while ((nbytes = read(0, cur, buf + 512 - cur)) != 0) {
    char *tail = cur + nbytes;
    // printf("nbytes: %d\n", nbytes);
    for (; cur < tail; ++cur) {
      if (*cur != '\n') {
        continue;
      }
      *cur = 0;
      if (argc == 1) {
        printf("%s\n", start);
      } else {
        if (fork() == 0) {
          param[argc - 1] = start;
          param[argc] = 0;
          //   for (int i = 0; i < argc; ++i) {
          //     printf("param[%d]: %s\n", i, param[i]);
          //   }
          //   printf("\n");
          exec(argv[1], param);
          exit(0);
        }
        wait(0);
      }
      start = cur + 1;
    }
    int m = start - buf;
    memmove(buf, start, m);
    cur -= m;
    start = buf;
  }
  //   printf("nbytes: %d, cnt: %d\n", nbytes, cnt);
  //   for (int i = 0; i < cnt; ++i) {
  //     printf("read %d\n", map[i]);
  //   }

  return 0;
}