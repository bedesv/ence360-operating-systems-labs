#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char buf[512];


int main(int argc, char *argv[])
{
    int src_fd;
    int dest_fd;

    if (argc != 3){
        printf(1, "usage: cp src_file dest_file\n");
        exit();
    }

    // TODO:
    if ((src_fd = open(argv[1], 0)) < 0) {
      printf(1, "cp: cannot open %s\n", argv[1]);
      exit();
    }
    if ((dest_fd = open(argv[2], O_CREATE | O_WRONLY)) < 0) {
      printf(1, "cp: cannot open %s\n", argv[2]);
      exit();
    }

    int n;
    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        if (write(dest_fd, buf, n) != n) {
            printf(1, "cp: write error\n");
        }
    }
    if(n < 0) {
        printf(1, "cp: read error\n");
        exit();
    }

    close(src_fd);
    close(dest_fd);

    exit();
}
