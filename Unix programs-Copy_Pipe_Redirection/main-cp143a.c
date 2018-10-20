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
    char data[1024];

    int fdfrom = open(argv[1], O_RDONLY);
    int fdto = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);

    if(fdfrom < 0 | fdto < 0)
        return -1;

    if(read(fdfrom, data, 1024) < 0)
        write(2, "An error occurred in the read.\n", 31);

    if(write(fdto, data, strlen(data)) !=  strlen(data))
    {
        write(2, "An error occurred in the write.\n", 32);
        return -1;
    }

    if(close(fdfrom) < 0 | close(fdto) < 0)
        return -1;

    exit(0);
}


