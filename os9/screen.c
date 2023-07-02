#ifdef DECB
#include <cmoc.h>
    struct sgbuf nstate;
#endif
#ifdef OS9
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sgstat.h>
#endif
#include "os9/screen.h"

char csr_up_s[1];
char csr_dn_s[1];
char csr_rt_s[1];
char csr_lt_s[1];
char csr_hm_s[1];
char clr_es_s[1];
char clr_el_s[1];
char csr_po_s[3];
char echo;

int interrupt(sig)
int sig;
{
        csr_pos(PROMPTLN, 0);
        clr_eos();
        scr_exit();
        exit(sig);
}

int scr_init()
{
    struct sgbuf nstate;

    intercept(interrupt);

    csr_up_s[0] = 0x09;
    
    csr_dn_s[0] = 0x0a;

    csr_rt_s[0] = 0x06;

    csr_lt_s[0] = 0x08;

    csr_hm_s[0] = 0x01;

    clr_es_s[0] = 0x0b;

    clr_el_s[0] = 0x04;

    csr_po_s[0] = 0x02;
    csr_po_s[1] = ' ';
    csr_po_s[2] = ' ';

    scr_cls();

    getstat(0, 0, &nstate);
    echo = nstate.sg_echo;
    nstate.sg_echo = 0;
    setstat(0, 0, &nstate);

    return 0;
}

int csr_up()
{
        WRITE(1, csr_up_s, 1);
        return 0;
}

int csr_dn()
{
        WRITE(1, csr_dn_s, 1);
        return 0;
}

int csr_rt()
{
        WRITE(1, csr_rt_s, 1);
        return 0;
}

int csr_lt()
{
        WRITE(1, csr_lt_s, 1);
        return 0;
}


int csr_home()
{
        WRITE(1, csr_hm_s, 1);
        return 0;
}

int clr_eos()
{
        WRITE(1, clr_es_s, 1);
        return 0;
}

int clr_eol()
{
        WRITE(1, clr_el_s, 1);
        return 0;
}

int csr_pos(y,x)
unsigned char y,x;
{
        csr_po_s[1] = ' '+x;
        csr_po_s[2] = ' '+y;
        WRITE(1, csr_po_s, 3);
        return 0;
}

int scr_cls()
{
        scr_putc(0x0c);
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
    struct sgbuf nstate;
    getstat(0, 0, &nstate);
    nstate.sg_echo = echo;
    setstat(0, 0, &nstate);
}


/*
# vim: ts=8 sw=8 sts=8 et
*/
