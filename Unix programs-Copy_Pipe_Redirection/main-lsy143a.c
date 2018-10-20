#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    close(1);
    open("y", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
    char *args[]={"ls",NULL}; 
    execvp(args[0],args);
    
    exit(0);
}


