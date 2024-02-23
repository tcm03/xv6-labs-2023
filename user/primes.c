#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_primes(int fd)
{
  int prime;
  if (read(fd, &prime, sizeof(prime)) == 0) {
    close(fd);
    return;
  }
  printf("prime %d\n", prime);
  int X;
  int p[2];
  pipe(p);
  while (read(fd, &X, sizeof(X)) > 0) {
    if (X % prime != 0)
      write(p[1], &X, sizeof(X));
  }
  close(fd);
  if (fork() == 0) {
    close(p[1]);
    print_primes(p[0]);
  }
  else {
    close(p[0]);
    close(p[1]);
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  if (argc > 1) {
    fprintf(2, "Too many arguments.\nUsage: primes\n");
    exit(1);
  }

  int p[2];
  pipe(p);
  for (int i = 2; i <= 35; ++i) {
    write(p[1], &i, sizeof(i));
  }
  close(p[1]);
  print_primes(p[0]);
  exit(0);
}
