#include <sgtty.h>
#include <stdio.h>


int main(argc, argv)
int argc;
char **argv;
{
    struct sgttyb buf,buf2;
    char c;

    gtty(0, &buf);
    gtty(0, &buf2);
    buf.sg_flags |= RAW;
    stty(0,&buf);

    write(1, "\x1b*", 2);
    while(c!=0x03)
    {
        read(0, &c, 1);
        printf("%02x\n\l", c);
    }    
    stty(0,&buf2);
    return 0;
}
                                                                                                                                                                                     