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
		printf("  %-8s %-3s\n", name, ext);
	}
}

void prntdir()
{
	char buf[256];
	int r;
	csr_pos(CWDLN,1);
	clr_eol();
	r = sdgetpth(buf);
	if ( r & FAILED )
		printf("FAILED\n");
	else
		printf("Dir: %s\n", buf);
}

void prntmnts()
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
}

int mount(dbuf, idx, drive)
struct dirent* dbuf;
int idx, drive;
{
	char buf[256];
	int r;
	r = sdgetpth(buf);
	if ( r & FAILED )
		printf("FAILED\n");
	strncatz(buf, dbuf[idx].name, 8);
	strcat(buf, ".");
	strncatz(buf, dbuf[idx].ext, 3);
	return sdmnt(drive, buf);
}

int umount(drive)
{
	return sdumnt(drive);
}


int doSelect(idx)
int idx;
{
	char c, buf[32];
	unsigned char attr;
	attr = dbuf[idx].attr;
	if ( isDir(attr) )
	{
		strncpyz(buf, dbuf[idx].name, 8);
		sdchdir(buf);		
	}
	else if ( ! isHidden(attr) )
	{
		strncpyz(buf, dbuf[idx].name, 8);
		strcat(buf, ".");
		strncatz(buf, dbuf[idx].ext, 3);
		csr_pos(PROMPTLN ,1);
		clr_eol();
		printf("File: %s \n", buf);
		clr_eol();
		printf("Enter drive (0,1) Any-aborts ?");
		c = waitkey(1);
		switch (c)
		{
			case KEY_ZERO:
			case KEY_ONE:
				mount(dbuf, idx, c-KEY_ZERO);
				break;

		}
	}

}

/*
int main()
{
	sdchdir("/DEV");
	prntdir();	
	return 0;
}
*/
int banner()
{
	csr_pos(1,9);
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
				csr_pos(DIRSTLN ,1);
				clr_eos();
				/*
				for (i=0; i<PGLEN; i++)
					printf("            \n");
				*/
				csr_pos(DIRSTLN,1);
				listdir(dbuf, start, min(PGLEN, count-start));
				newpag = 0;
				newmount = 1;
			}
			if (newmount)
			{
				prntmnts();
				newmount = 0;
			}
			csr_pos(DIRSTLN+crsr, 1);
			printf(">");
			csr_pos(PROMPTLN ,1);
			printf("(Mnt)1,2 (Umnt)Sh-1/Sh-2 (Sel)Enter\n");
			printf("(Move)U/D (Pag)Sh-U/D (Quit)Brk");
			c = waitkey(0);
			csr_pos(BANNERLN ,51-4);
			printf("0x%02x", c);
			csr_pos(DIRSTLN+crsr, 1);
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
			}
		}
	}
	return 0;
}
