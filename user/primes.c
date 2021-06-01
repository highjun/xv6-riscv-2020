#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//pipe가 닫힌 것이 read를 통해서 반영이 안됨...
//그냥 -1을 전달해서 닫음.
int
main(int argc, char *argv[])
{
    int prev_pipe[2];
    if(pipe(prev_pipe) < 0){
        printf("pipe error\n");
        exit(0);
    }
    int pid;
    if((pid = fork()) > 0){
        for(int i = 2;i<36;i++){
            write(prev_pipe[1], &i, 4);
        }
        pid = -1;
        write(prev_pipe[1], &pid, 4);
        close(prev_pipe[1]);
        wait(&pid);
    }else{
        int prime = -1;
        int num = -1;
        int next_pipe[2];
        while(read(prev_pipe[0], &num, 4) > 0){
            if(num == -1) break;
            if(prime==-1){
                if(pipe(next_pipe) < 0){
                    printf("pipe error\n");
                    exit(1);
                }
                if((pid = fork())>0){
                    prime = num;
                    printf("prime %d\n", prime);
                }else{
                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                    prev_pipe[0] = next_pipe[0];
                    prev_pipe[1] = next_pipe[1];
                    next_pipe[0] = -1;
                    next_pipe[1] = -1;
                }
            }else{
                if(num %prime != 0)
                    write(next_pipe[1], &num, 4);
            }
        }
        if(next_pipe[0] != -1){
            write(next_pipe[1], &num, 4);
            close(prev_pipe[0]);
            close(next_pipe[1]);
        }
        wait(&pid);
    }
    exit(0);
}
