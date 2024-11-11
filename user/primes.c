#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void proc(int p[]){
    int prime;
    if(read(p[0], &prime, sizeof(int)) == 0){
        exit(0);
    }
    printf("prime %d\n", prime);

    int p1[2];
    pipe(p1);
    int pid;
    if((pid = fork()) == 0){
        //close(p[0]);
        close(p1[1]);
        proc(p1);
    }
    else{
        close(p1[0]);
        int num;
        while((read(p[0], &num, sizeof(int))) != 0){
            if(num % prime != 0){
                write(p1[1], &num, sizeof(int));
            }
        }
        close(p1[1]);
        close(p[0]);
        wait(0);
        exit(0);
    }
}


int main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    int pid;
    if((pid = fork()) == 0){
        close(p[1]);
        proc(p);
    }
    else{
        close(p[0]);
        for(int i = 2; i <= 35; i++){
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
        exit(0);
    }
    exit(0);
}