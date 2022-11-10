#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(const char *path, const char *target) {
  //   printf("path: %s, target: %s\n", path, target);
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512], *p = 0;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0) {
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (strcmp(de.name, target) == 0) {
        printf("%s\n", buf);
      }
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      find(buf, target);
    }
  }
  close(fd);
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(2, "Usage: find [dir] [name]\n");
    exit(0);
  }

  find(argv[1], argv[2]);

  return 0;
}