#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void)
{
    int status;
    int p1[2];
    char *args1[]={"ls",NULL};
    char *args2[]={"grep","main",NULL};
    char *args3[]={"wc",NULL};
    int p2[2];
    pipe(p1);

    if (fork()  == 0)
    {
        dup2(p1[0], 0);
        close(p1[1]);
        execvp(args3[0],args3);
        printf("error occured in wc");
        return 1;
    }

    pipe(p2);

    if(fork() == 0)
    {
        dup2(p1[1], 1);
        close(p1[0]);

        dup2(p2[0],0);
        close(p2[1]);
        execvp(args2[0],args2);
        printf("error occured in grep main");
        return 1;
    }
    else  
    {
        dup2(p2[1],1);
        close(p2[0]);
        execvp(args1[0], args1);
        printf("error occured in ls");
        return 1;
    }
   
    wait(&status); 
    exit(0);
}


