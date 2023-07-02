#include <cmoc.h>
#include "screen.h"
#include "hirestxt-0.3.1/hirestxt.h"

char csr_up_s[2];
char csr_dn_s[2];
char csr_rt_s[2];
char csr_lt_s[2];
char csr_home_s[2];
char clr_eos_s[2];
char clr_eol_s[2];
char csr_pos_s[4];

int scr_init()
{
    // initHiResTextScreen() must be called first.
    // Assumes 4 graphics pages reserved at $0E00.
    // TRUE requests that printf() be redirected to the VT52 interpreter,
    // which writes to the 51x24 screen.
    //
    struct HiResTextScreenInit init =
        {
            51,
            writeCharAt_51cols,
            (byte) (0x0E00 / 512),
            TRUE
        };
    initHiResTextScreen(&init);

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
        return 0;
}

/*
# vim: ts=8 sw=8 sts=8 et
*/
