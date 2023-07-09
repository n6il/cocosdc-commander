#ifdef FUZIX
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>


struct termios prior, new;

#endif
#include "screen.h"

char csr_up_s[2];
char csr_dn_s[2];
char csr_rt_s[2];
char csr_lt_s[2];
char csr_home_s[2];
char clr_eos_s[2];
char clr_eol_s[2];
char csr_pos_s[4];
char echo;

#define puterr scr_puts

void interrupt(sig)
int sig;
{
	signal(SIGINT, SIG_IGN);
        csr_pos(PROMPTLN, 0);
        clr_eos();
        scr_exit();
        exit(sig);
}

int scr_init()
{

        int fd;

        signal(SIGINT, interrupt);

        csr_up_s[0] = 0x1b;
        csr_up_s[1] = 'A';

        csr_dn_s[0] = 0x1b;
        csr_dn_s[1] = 'B';

        csr_rt_s[0] = 0x1b;
        csr_rt_s[1] = 'C';

        csr_lt_s[0] = 0x1b;
        csr_lt_s[1] = 'D';

        csr_home_s[0] = 0x1b;
        csr_home_s[1] = 'H';

        clr_eos_s[0] = 0x1b;
        clr_eos_s[1] = 'J';

        clr_eol_s[0] = 0x1b;
        clr_eol_s[1] = 'K';

        csr_pos_s[0] = 0x1b;
        csr_pos_s[1] = 'Y';
        csr_pos_s[2] = ' ';
        csr_pos_s[3] = ' ';
        scr_cls();

        fd = open("/dev/tty", O_RDWR | O_NDELAY);
        if( ! fd ){
                puterr( "Cannot open device\n" );
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

        return 0;
}

int csr_up()
{
        WRITE(1, csr_up_s, 2);
        return 0;
}

int csr_dn()
{
        WRITE(1, csr_dn_s, 2);
        return 0;
}

int csr_rt()
{
        WRITE(1, csr_rt_s, 2);
        return 0;
}

int csr_lt()
{
        WRITE(1, csr_lt_s, 2);
        return 0;
}


int csr_home()
{
        WRITE(1, csr_home_s, 2);
        return 0;
}

int clr_eos()
{
        WRITE(1, clr_eos_s, 2);
        return 0;
}

int clr_eol()
{
        WRITE(1, clr_eol_s, 2);
        return 0;
}

int csr_pos(y,x)
unsigned char y,x;
{
        csr_pos_s[2] = ' '+y;
        csr_pos_s[3] = ' '+x;
        WRITE(1, csr_pos_s, 4);
        return 0;
}

int scr_cls()
{
        csr_home();
        clr_eos();
        return 0;
}

int scr_getc()
{
        char c;
        read(0, &c, 1);
        return c;
}


int scr_putc(c)
char c;
{
        return WRITE(1, &c, 1);
}

int scr_puts(str)
char *str;
{
        return WRITE(1, str, strlen(str));
}



char *scr_gets(buf)
char *buf;
{
        char c, *p;

        p = buf;
        *p = 0;
        while ((c=scr_getc()) != KEY_ENTER)
        {
                if (c == KEY_BRK)
                        break;
                if (c == KEY_BS)
                {
                        if (strlen(buf) > 0)
                        {
                                p--;
                                *p=0;
                                csr_lt();
                                scr_putc(' ');
                                csr_lt();
                        }
                }
                else
                {
                        *p++ = c;
                        *p = 0;
                        scr_putc(c);
                }
        }
        return buf;
}

int scr_exit()
{
        int fd;

        fd = open("/dev/tty", O_RDWR | O_NDELAY);
        if( ! fd ){
                puterr( "Cannot open device\n" );
                exit(-1);
        }
        tcsetattr(fd, TCSANOW, &prior);
        close(fd);
}


/*
# vim: ts=8 sw=8 sts=8 et
*/
