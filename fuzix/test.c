#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include "screen.h"

struct termios prior, new;


int main(argc, argv)
int argc;
char **argv;
{
    char c;
    int fd;

        fd = open("/dev/tty", O_RDWR | O_NDELAY);
        if( ! fd ){
                scr_puts( "Cannot open device\n" );
                exit(-1);
        }
        tcgetattr(fd, &prior);
        tcgetattr(fd, &new);
        new.c_iflag &= ~ICRNL;
        new.c_lflag &= ~ECHO;
        new.c_lflag &= ~ICANON;
        new.c_oflag &= ~OPOST;
        tcsetattr(fd, TCSANOW, &new);
        close(fd);

    write(1, "\x1b*", 2);
    while(c!=0x03)
    {
        read(0, &c, 1);
        printf("%02x\r\n", c);
    }    
        fd = open("/dev/tty", O_RDWR | O_NDELAY);
        if( ! fd ){
                scr_puts( "Cannot open device\n" );
                exit(-1);
        }
        tcsetattr(fd, TCSANOW, &prior);
        close(fd);
    return 0;
}
