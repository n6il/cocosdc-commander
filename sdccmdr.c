#ifdef DECB
#include <cmoc.h>
#include <coco.h>
#endif

#include "libsdc.h"
#include "commsdc.h"
#include "string.h"
#include "sdccmdr.h"

#ifdef DECB
#include "hirestxt/screen.h"
#endif

#ifdef OS9
#include <stdio.h>
#include <string.h>
#include "os9/screen.h"
#endif

#ifdef FLEX
#include <stdio.h>
#include "screen.h"
#endif

#ifdef FUZIX
#include <stdio.h>
#include <string.h>
#include "screen.h"
#endif

struct dirent dbuf[DBUFLEN];

char lbuf[81];

int listdir(p, start, count)
struct dirent *p;
int start, count;
{
        int n;
        char name[9];
        char ext[4];

        for (n=DBUFLEN; start && n && p->name != 0; p++, start--, n--);
        for (; count && n && p->name[0] != 0; p++, n--, count--)
        {
                strncpyz(name, p->name, 8);
                if (p->attr & 0x10)
                        strcpy(ext, "<D>");
                else
                {
                        strncpyz(ext, p->ext, 3);
                        ext[3] = 0;
                }
                sprintf(lbuf, " %-8s %-3s%s", name, ext, NL);
                scr_puts(lbuf);
        }
        return 0;
}

int prntdir()
{
        char c, buf[256];
        int r;
        csr_pos(CWDLN, 0);
        clr_eol();
        r = sdgetpth(buf);
        if ( r & FAILED )
        {
                csr_pos(PROMPTLN, 0);
                clr_eos();
                scr_puts("sdgetpth() FAILED");
                scr_puts(NL);
                sprintf(lbuf, "Result:0x%02x - Press Key...", r);
                scr_puts(lbuf);
                c = scr_getc();
        }
        else
        {
                sprintf(lbuf, "Dir: %s%s", buf, NL);
                scr_puts(lbuf);
        }
        return r;
}

int prntmnts()
{
        char buf[256];
        int r;
        char name[9];
        char ext[4];
        struct dirent *p;

        csr_pos(DIRSTLN, 26);
        scr_puts("Current Mounts");

        csr_pos(DIRSTLN+2, 26);
        clr_eol();
        scr_puts("0: ");
        r = sdimginf(buf, 0);
        if ( ! (r& FAILED))
        {
                p = (struct dirent *)buf;

                strncpyz(name, p->name, 8);
                if (p->attr & 0x10)
                        strcpy(ext, "<D>");
                else
                {
                        strncpyz(ext, p->ext, 3);
                        ext[3] = 0;
                }
                sprintf(lbuf, "%-8s %-3s", name, ext);
                scr_puts(lbuf);
        }
        else
        {
                scr_puts("--");
        }

        csr_pos(DIRSTLN+3 ,26);
        clr_eol();
        scr_puts("1: ");
        r = sdimginf(buf, 1);
        if ( ! (r& FAILED))
        {
                p = (struct dirent *)buf;

                strncpyz(name, p->name, 8);
                if (p->attr & 0x10)
                        strcpy(ext, "<D>");
                else
                {
                        strncpyz(ext, p->ext, 3);
                        ext[3] = 0;
                }
                sprintf(lbuf, "%-8s %-3s", name, ext);
                scr_puts(lbuf);
        }
        else
        {
                scr_puts("--");
        }
        return r;
}

int _mount(drive, buf)
int drive;
char *buf;
{
        char c;
        int r;

        r =  sdmnt(drive, buf);
        if (r & FAILED)
        {
                csr_pos(PROMPTLN, 0);
                clr_eos();
                sprintf(lbuf, "sdmnt(%d, %s) FAILED%s", drive, buf, NL);
                scr_puts(lbuf);
                sprintf(lbuf, "Result:0x%02x - Press Key...", r);
                scr_puts(lbuf);
                c = scr_getc();
        }
        return r;
}

int mount(dbuf, idx, drive)
struct dirent* dbuf;
int idx, drive;
{
        char buf[256];
        char c;
        int r;

        strncpyz(buf, dbuf[idx].name, 8);
        strcat(buf, ".");
        strncatz(buf, dbuf[idx].ext, 3);

        r =  _mount(drive, buf);
        return r;
}

int umount(drive)
{
        int r;
        char c;
        r = sdumnt(drive);
        if ( r & FAILED )
        {
                csr_pos(PROMPTLN, 0);
                clr_eos();
                sprintf(lbuf, "sdumnt(%d) FAILED%s", drive, NL);
                scr_puts(lbuf);
                sprintf(lbuf, "Result:0x%02x - Press Key...", r);
                scr_puts(lbuf);
                c = scr_getc();
        }
        return r;
}


int doSelect(idx)
int idx;
{
        char c, buf[32];
        unsigned char attr;
        int r;
        attr = dbuf[idx].attr;
        if ( isDir(attr) )
        {
                strncpyz(buf, dbuf[idx].name, 8);
                r = sdchdir(buf);
                if ( r & FAILED )
                {
                        csr_pos(PROMPTLN, 0);
                        clr_eos();
                        sprintf(lbuf, "sdchdir(%s) FAILED%s", buf, NL);
                        scr_puts(lbuf);
                        sprintf(lbuf, "Result:0x%02x - Press Key...", r);
                        scr_puts(lbuf);
                        c = scr_getc();
                }
        }
        else if ( ! isHidden(attr) )
        {
                strncpyz(buf, dbuf[idx].name, 8);
                strcat(buf, ".");
                strncatz(buf, dbuf[idx].ext, 3);
                csr_pos(PROMPTLN, 0);
                clr_eos();
                sprintf(lbuf, "File: %s%s", buf, NL);
                scr_puts(lbuf);
                scr_puts("Enter drive (0,1) Any-aborts ?");
                c = scr_getc();
                switch (c)
                {
                        case KEY_ZERO:
                        case KEY_ONE:
                                r = mount(dbuf, idx, c-KEY_ZERO);
                                break;

                }
        }

        return r;
}

int doChdir()
{
        char c, buf[256];
        int r;

        csr_pos(PROMPTLN, 0);
        clr_eos();
        scr_puts("Change Directory");
        scr_puts(NL);
        scr_puts("? ");
        scr_gets(buf);
        r = sdchdir(buf);
        if ( r & FAILED )
        {
                csr_pos(PROMPTLN, 0);
                clr_eos();
                sprintf(lbuf, "sdchdir(%s) FAILED%s", buf, NL);
                scr_puts(lbuf);
                sprintf(lbuf, "Result:0x%02x - Press Key...", r);
                scr_puts(lbuf);
                c = scr_getc();
        }
        return r;
}

int doMount()
{
        char c, buf[256];
        int r;

        csr_pos(PROMPTLN, 0);
        clr_eos();
        scr_puts("Mount - Enter File/Path");
        scr_puts(NL);
        scr_puts("? ");
        scr_gets(buf);
        csr_pos(PROMPTLN, 0);
        clr_eos();
        sprintf(lbuf, "File: %s%s", buf, NL);
        scr_puts(lbuf);
        scr_puts("Enter drive (0,1) Any-aborts ?");
        c = scr_getc();
        switch (c)
        {
                case KEY_ZERO:
                case KEY_ONE:
                        r = _mount(c-KEY_ZERO, buf);
                        break;

        }
        return r;
}


int banner()
{
        csr_pos(BANNERLN, 9);
        sprintf(lbuf, "CoCoSDC Commander - by MikeyN6IL");
        scr_puts(lbuf);
        return 0;
}

int main()
{
        int r, start, count, crsr, maxpg;
        int newdir, newpag, newmount, newprompt;
        char c;
        scr_init();
        banner();

        if (findsdc() == -1)
        {
                csr_pos(PROMPTLN, 0);
                clr_eos();
                scr_puts("CoCoSDC Not Detected");
                scr_puts(NL);
                scr_puts("Press Key to Exit...");
                scr_getc();
                return 1;
        }

        c=0;
        newdir=1; newpag=0; newmount=1; newprompt=1;
        count=0;

        while (c != KEY_BRK  && c != KEY_Q && c != KEY_q)
        {
                if (newdir)
                {
                        r = prntdir();
                        if (! (r & FAILED))
                        {
                                count = getdir(0, dbuf, DBUFLEN);
                                start = 0;
                                crsr = 0;
                                maxpg = (count / PGLEN) * PGLEN;
                                newdir = 0;
                                newpag = 1;
                        }
                }
                if (count)
                {
                        if (newpag)
                        {
                                csr_pos(DIRSTLN, 0);
                                clr_eos();
                                /*
                                for (i=0; i<PGLEN; i++)
                                        printf("            "NL);
                                */
                                csr_pos(DIRSTLN, 0);
                                listdir(dbuf, start, MIN(PGLEN, count-start));
                                newpag = 0;
                                newmount = 1;
                                newprompt = 1;
                        }
                        if (newmount)
                        {
                                prntmnts();
                                newmount = 0;
                                newprompt = 1;
                        }
                        csr_pos(DIRSTLN+crsr, 0);
                        scr_putc('>');
                        csr_pos(PROMPTLN-1, 0);
                        if (newprompt)
                        {
                                csr_pos(PROMPTLN, 0);
                                sprintf(lbuf, "(Mnt)1,2,M (Umnt)Sh-1/Sh-2 (Sel)Enter%s", NL);
                                scr_puts(lbuf);
#ifdef FLEX
                                sprintf(lbuf, "(Move)U/D  (Pag)B/Spc  (Chd)C (Quit)Q");
#else
                                sprintf(lbuf, "(Move)U/D  (Pag)Sh-U/Sh-D  (Chd)C (Quit)Q");
#endif
                                scr_puts(lbuf);
                        }
                        c = scr_getc();
                        /*
                        csr_pos(BANNERLN ,51-4);
                        printf("0x%02x", c);
                        */
                        csr_pos(DIRSTLN+crsr, 0);
                        scr_putc(' ');
                        switch (c)
                        {
                                case KEY_UP:
                                        if (crsr > 0)
                                        {
                                                crsr -= 1;
                                                newprompt=0;
                                                break;
                                        }
                                        else
                                        {
                                                crsr = (start>=PGLEN) ? PGLEN-1 : 0;
                                        }
                                case KEY_PGUP:
                                case KEY_B:
                                case KEY_b:
                                        start = start>=PGLEN ? start - PGLEN : 0;
                                        if (c==KEY_PGUP)
                                                crsr = 0;
                                        newpag=1;
                                        newprompt=1;
                                        break;
                                case KEY_DOWN:
                                        if (crsr < (PGLEN-1))
                                        {
                                                if ( (start+crsr) < count )
                                                        crsr += 1;
                                                newprompt=0;
                                                break;
                                        }
                                        else if (start<maxpg)
                                        {
                                                crsr = 0;
                                        }
                                case KEY_PGDN:
                                case KEY_SPC:
                                        start = start<maxpg ? start + PGLEN : maxpg;
                                        crsr = 0;
                                        newpag=1;
                                        newprompt=1;
                                        break;

                                case KEY_ONE:
                                case KEY_TWO:
                                        mount(dbuf, start+crsr, c-KEY_ONE);
                                        newmount=1;
                                        break;

                                case KEY_EXP:
                                case KEY_AT:
                                        umount(c-KEY_EXP);
                                        newmount=1;
                                        break;

                                case KEY_ENTER:
                                        doSelect(start+crsr);
                                        newdir=1;
                                        newprompt=1;
                                        break;

                                case KEY_C:
                                case KEY_c:
                                        doChdir();
                                        newdir=1;
                                        newprompt=1;
                                        break;

                                case KEY_M:
                                case KEY_m:
                                        doMount();
                                        newmount=1;
                                        newprompt=1;
                                        break;
                        }
                }
                else
                {
                        csr_pos(PROMPTLN, 0);
                        clr_eos();
                        scr_puts("SD Card or Directory Empty");
                        scr_puts(NL);
                        scr_puts("Press Key to Exit...");
                        scr_getc();
                        break;
                }
        }

        csr_pos(PROMPTLN, 0);
        clr_eos();
        scr_exit();
        return 0;
}

/*
# vim: ts=8 sw=8 sts=8 et
*/
