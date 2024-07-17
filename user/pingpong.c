#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[])
{
    char buf = 'P'; //用于传送的字节

    int fd_c2p[2]; //子进程->父进程
    int fd_p2c[2]; //父进程->子进程
    pipe(fd_c2p);
    pipe(fd_p2c);

    int pid = fork();
    int exit_status = 0;

    if(pid < 0){
        fprintf(2, "fork() error!\n");
        close(fd_c2p[0]);
        close(fd_c2p[1]);
        close(fd_p2c[0]);
        close(fd_p2c[1]);
        exit(1);
    }else if(pid == 0){
        close(fd_c2p[0]);
        close(fd_p2c[1]);
        if(read(fd_p2c[0], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "child process read() error!\n");
            exit_status = 1;
        }else {
            fprintf(1, "%d: received ping\n", getpid());
        }
        if(write(fd_c2p[1], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "child process write() error!\n");
            exit_status = 1;
        }
        close(fd_c2p[1]);
        close(fd_p2c[0]);
    }else{
        close(fd_c2p[1]);
        close(fd_p2c[0]);
        if(write(fd_p2c[1], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "parent process write() error!\n");
            exit_status = 1;
        }
        if(read(fd_c2p[0], &buf, sizeof(char)) != sizeof(char)){
            fprintf(2, "parent process read() error!\n");
            exit_status = 1;
        }else {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(fd_c2p[0]);
        close(fd_p2c[1]);
    }
     exit(exit_status);
}