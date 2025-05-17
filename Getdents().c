#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/syscall.h> 
#include <dirent.h> 
#include <string.h> 
#define BUF_SIZE 4096  
#ifndef __NR_getdents 
#define __NR_getdents 78 
#endif 
struct linux_dirent { 
    unsigned long d_ino; 
    off_t d_off; 
    unsigned short d_reclen; 
    char d_name[]; 
}; 
int main(int argc, char *argv[]) { 
    int fd; 
    char buf[BUF_SIZE]; 
    struct linux_dirent *d; 
    int bpos; 
    char *dirpath = "."; 
    if (argc > 1) { 
        dirpath = argv[1]; 
    } 
    fd = open(dirpath, O_RDONLY | O_DIRECTORY); 
    if (fd == -1) {
    	perror("open"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Contents of '%s':\n", dirpath); 
    printf("INODE\t\tNAME\n"); 
    while (1) { 
        int nread = syscall(__NR_getdents, fd, buf, BUF_SIZE); 
        if (nread == -1) { 
            perror("getdents"); 
            close(fd); 
            exit(EXIT_FAILURE); 
        } 
        if (nread == 0) break; 
        for (bpos = 0; bpos < nread;) { 
            d = (struct linux_dirent *)(buf + bpos); 
            printf("%-10lu\t%s\n", d->d_ino, d->d_name); 
            bpos += d->d_reclen; 
        } } 
    close(fd); 
    return 0; 
} 