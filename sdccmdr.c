#include <cmoc.h>
#include <coco.h>
#include "commsdc.h"
#include "libsdc.h"
#include "string.h"
#include "hirestxt/screen.h"
#include "sdccmdr.h"

struct dirent dbuf[DBUFLEN];

void listdir(p, start, count)
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
		printf(" %-8s %-3s\n", name, ext);
	}
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
		printf("sdgetpth() FAILED\n");
		printf("Result:0x%02x - Press Key...", r);
		c = waitkey(1);
	}
	else
		printf("Dir: %s\n", buf);
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
	printf("Current Mounts");

	csr_pos(DIRSTLN+2, 26);
	clr_eol();
	printf("0: ");
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
		printf("%-8s %-3s", name, ext);
	}
	else
	{
		printf("--");
	}

	csr_pos(DIRSTLN+3 ,26);
	clr_eol();
	printf("1: ");
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
		printf("%-8s %-3s", name, ext);
	}
	else
	{
		printf("--");
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

	r =  sdmnt(drive, buf);
	if (r & FAILED)
	{
		csr_pos(PROMPTLN, 0);
		clr_eos();
		printf("sdmnt(%d, %s) FAILED\n", drive, buf);
		printf("Result:0x%02x - Press Key...", r);
		c = waitkey(1);
	}
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
		printf("sdumnt(%d) FAILED\n", drive);
		printf("Result:0x%02x - Press Key...", r);
		c = waitkey(1);
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
			printf("sdchdir(%s) FAILED\n", buf);
			printf("Result:0x%02x - Press Key...", r);
			c = waitkey(1);
		}
	}
	else if ( ! isHidden(attr) )
	{
		strncpyz(buf, dbuf[idx].name, 8);
		strcat(buf, ".");
		strncatz(buf, dbuf[idx].ext, 3);
		csr_pos(PROMPTLN, 0);
		clr_eos();
		printf("File: %s \n", buf);
		printf("Enter drive (0,1) Any-aborts ?");
		c = waitkey(1);
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
	char c, buf[80], *p;
	int r;

	csr_pos(PROMPTLN, 0);
	clr_eos();
	printf("Change Directory\n");
	printf("? ");
	c=0;
	p=buf;
	while ((c=waitkey(1)) != KEY_ENTER)
	{
		*p++ = c;
		printf("%c", c);
	}
	*p = 0;
	r = sdchdir(buf);
	if ( r & FAILED )
	{
		csr_pos(PROMPTLN, 0);
		clr_eos();
		printf("sdchdir(%s) FAILED\n", buf);
		printf("Result:0x%02x - Press Key...", r);
		c = waitkey(1);
	}
	return r;
}

int banner()
{
	csr_pos(BANNERLN, 9);
	printf("CoCoSDC Commander - by MikeyN6IL");
	return 0;
}

int main()
{
	int start, count, crsr, maxpg;
	int newdir, newpag, newmount;
	char c;
	scr_init();
	banner();

	c=0;
	newdir=1; newpag=0; newmount=1;
	count=0;

	while (c != 0x03 )
	{
		if (newdir)
		{
			prntdir();
			count = getdir(0, dbuf, DBUFLEN);
			start = 0;
			crsr = 0;
			maxpg = (count / PGLEN) * PGLEN;
			newdir = 0;
			newpag = 1;
		}
		if (count)
		// for (start=0; start<count; start+=PGLEN)	
		// start = 0;
		// crsr = 0;
		// while (c != 0x03 )
		{
			if (newpag)
			{
				csr_pos(DIRSTLN, 0);
				clr_eos();
				/*
				for (i=0; i<PGLEN; i++)
					printf("            \n");
				*/
				csr_pos(DIRSTLN, 0);
				listdir(dbuf, start, min(PGLEN, count-start));
				newpag = 0;
				newmount = 1;
			}
			if (newmount)
			{
				prntmnts();
				newmount = 0;
			}
			csr_pos(DIRSTLN+crsr, 0);
			printf(">");
			csr_pos(PROMPTLN, 0);
			printf("(Mnt)1,2 (Umnt)Sh-1/Sh-2 (Sel)Enter\n");
			printf("(Move)U/D (Pag)Sh-U/Sh-D (Chd)C (Quit)Brk");
			c = waitkey(0);
			csr_pos(DIRSTLN+crsr, 0);
			printf(" ");
			switch (c)
			{
				case KEY_UP:
					if (crsr > 0)
					{
						crsr -= 1;
						break;
					}
					else
					{
						crsr = (start>=PGLEN) ? PGLEN-1 : 0;
					}
				case KEY_PGUP:
					start = start>=PGLEN ? start - PGLEN : 0;
					if (c==KEY_PGUP)
						crsr = 0;
					newpag=1;
					break;
				case KEY_DOWN:
					if (crsr < (PGLEN-1))
					{
						if ( (start+crsr) < count )
							crsr += 1;
						break;
					}
					else if (start<maxpg)
					{
						crsr = 0;
					}
				case KEY_PGDN:
					start = start<maxpg ? start + PGLEN : maxpg;
					crsr = 0;
					newpag=1;
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
					break;

				case KEY_C:
					doChdir();
					newdir=1;
					break;
			}
		}
	}
	return 0;
}
