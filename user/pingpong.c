#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    char c;
    int pid, p[2];
    pipe(p);
    pid = fork();

    if(pid == 0){
        read(p[0], &c, 1);
        int child_pid = getpid();
        printf("%d: received ping\n", child_pid);
        write(p[1], "c", 1);
    }
    else{
        write(p[1], "p", 1);
        read(p[0], &c, 1);
        int parent_pid = getpid();
        printf("%d: received pong\n", parent_pid);
    }
    exit(0);
}