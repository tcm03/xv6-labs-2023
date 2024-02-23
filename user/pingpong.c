#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc > 1) {
    fprintf(2, "Too many arguments.\nUsage: pingpong\n");
    exit(1);
  }

  int parent_to_child[2];
  int child_to_parent[2];
  pipe(parent_to_child);
  pipe(child_to_parent);
  if (fork() == 0) {
    char byte;
    read(parent_to_child[0], &byte, 1);
    printf("%d: received ping\n", getpid());
    close(parent_to_child[0]);
    close(parent_to_child[1]);

    close(child_to_parent[0]);
    write(child_to_parent[1], &byte, 1);
    close(child_to_parent[1]);
  }
  else {
    close(parent_to_child[0]);
    write(parent_to_child[1], "b", 1);
    close(parent_to_child[1]);

    wait(0);
    char byte;
    read(child_to_parent[0], &byte, 1);
    printf("%d: received pong\n", getpid());
    //printf("The byte is: %c\n", byte[0]);
    close(child_to_parent[0]);
    close(child_to_parent[1]);
  }
  exit(0);
}
