#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p2c[2], c2p[2], pid;
    char buf[1];
    if(pipe(p2c)<0 || pipe(c2p)<0){
        printf("pipe error\n");
        exit(1);
    }
    if((pid=fork()) >0){//부모
        buf[0] = 'P';
        write(p2c[1], buf, 1);
        if(read(c2p[0], buf, 1)==1)
            printf("%d: received pong\n", getpid());
    }else{//자식
        if(read(p2c[0], buf, 1) == 1)
            printf("%d: received ping\n", getpid());
        buf[0] = 'C';
        write(c2p[1], buf, 1);
    }
    exit(0);
}
