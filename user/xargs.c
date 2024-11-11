#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
*/

#define MAXLINE 1024
#define MAXARGS 32

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: xargs <command>\n");
        exit(1);
    }

    char *command = argv[1];
    char *new_argv[MAXARGS] = {0};
    int i, index = 0;

    // Copy the command and its arguments
    for(i = 1; i < argc && i < MAXARGS - 1; i++){
        new_argv[index] = argv[i];
        index++;
    }

    char buf[MAXLINE] = {'\0'};
    char *buf_ptr = buf; 
    int ret = 0;

    while((ret = read(0, buf_ptr, MAXLINE)) != 0){
        buf_ptr += ret;
    }

    // while((n = read(0, buf, MAXLINE)) > 0){
        // buf[n] = '\0';  // Null terminate the input buffer
        
        int start = 0, end = 0;
        while(buf[end] != '\0'){
            // Skip leading spaces
            while(buf[end] == ' ' || buf[end] == '\n'){
                end++;
            }
            start = end;
            
            // Find the end of the current argument
            while(buf[end] != ' ' && buf[end] != '\n' && buf[end] != '\0'){
                end++;
            }
            
            if(start < end){  // Only add non-empty arguments
                buf[end] = '\0';  // Null terminate the current argument
                new_argv[index] = &buf[start];  // Add argument to new_argv
                index++;
            }

            if(index >= MAXARGS - 1){
                printf("xargs: too many arguments\n");
                exit(1);
            }

            end++;  // Move to the next character
        }
    // }


    // Fork and execute the command
    if(fork() == 0){
        exec(command, new_argv);
        printf("exec %s failed\n", command);  // Print error if exec fails
        exit(1);
    } else {
        wait(0);  // Wait for child to finish
    }
    
    exit(0);
}
