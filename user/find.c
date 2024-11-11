#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"


char *get_filename(char *path) {
    char *p;
    for(p = path + strlen(path); p >= path && *p != '/'; p--);
    return ++p;
}

void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "Cannot open %s\n", path);
        exit(0);
    }
    if((fstat(fd, &st)) < 0) {
        fprintf(2, "Cannot stat %s\n", path);
        close(fd);
        exit(0);
    }

    switch(st.type) {
        case T_FILE:
            p = get_filename(path);;
            if(strcmp(p, filename) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                strcpy(buf, path);
                p = buf + strlen(buf);
                *p++ = '/';
                memmove(p, de.name, sizeof(de.name));
                *(p + sizeof(de.name)) = '\0';
                find(buf, filename);
            }
            break;
    }
    close(fd);
    return;
}



int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: find <path> <filename>\n");
        exit(1);
    }

    char *path = argv[1];
    char *filename = argv[2];

    find(path, filename);
    exit(0);
}