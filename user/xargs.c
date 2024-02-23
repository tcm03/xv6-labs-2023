#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX 512

int
main(int argc, char *argv[])
{	
	char* new_argv[MAXARG];
    for (int i = 0; i < argc-1; ++i)
        new_argv[i] = argv[i+1];
    --argc;

    char buf[MAX];
    int N;
    int buf_size = 0;
    // dynamic mem allocation (malloc) seems to be evil in this xv6 OS, so let it be as follows:
    while ((N = read(0, buf+buf_size, sizeof(buf))) > 0) {
        if (buf[buf_size+N-1] == '\n')
            buf[buf_size+N-1] = ' ';
        buf_size += N;
    }
    buf[buf_size] = 0;
    int idx = 0;
    for (int i = 0; i < buf_size; ++i) {
        if (buf[i] == ' ') {
            buf[i] = 0;
            new_argv[argc++] = buf+idx;
            new_argv[argc] = 0;
            idx = i+1;
        }
    }
    if (fork() == 0) {
        exec(new_argv[0], new_argv);
    }
    else wait(0);
    exit(0);
}