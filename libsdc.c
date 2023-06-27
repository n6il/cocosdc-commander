#include <cmoc.h>
#include "commsdc.h"
#include "string.h"
#include "libsdc.h"

char _buf[256];

/* sdumnt */
int sdumnt(drive)
int drive;
{
	unsigned char cmd;

	if (drive < 0 || drive > 1)
		return FAILED;

	cmd = 0xe0 + drive;

	strcpy(_buf, "M:");
	return CommSDC(cmd, 0, 0, 0, _buf);
}

/* sdmnt */
int sdmnt(drive, path)
int drive;
char *path;
{
	unsigned char cmd;

	if (drive < 0 || drive > 1)
		return FAILED;

	cmd = 0xe0 + drive;

	strcpy(_buf, "M:");
	strcat(_buf, path);
	return CommSDC(cmd, 0, 0, 0, _buf);
}

/* sdimginf */
int sdimginf(buf, drive)
char *buf;
unsigned char drive;
{
	int r;
	unsigned char cmd;

	cmd = 0xc0 + drive;

	if (buf==0)
		return FAILED;

	r = CommSDC(cmd, 0x49, 0, 0, buf);
	return r;
}

/* sdgtdpag */
int sdgtdpag(buf)
char *buf;
{
	int r;

	if (buf==0)
		return FAILED;

	r = CommSDC(0xc0, 0x3e, 0, 0, buf);
	return r;
}


/* sdlstdir */
int sdlstdir(path, filter)
char *path, *filter;
{
	int r;

	strcpy(_buf, "L:");
	if (path != 0)
	{
		strcat(_buf, path);
		strcat(_buf, "/");
	}
	if (filter==0)
		strcat(_buf, "*.*");
	else
		strcat(_buf, filter);
	
	// printf("LD:%s\n", _buf);
	r = CommSDC(0xe0, 0, 0, 0, _buf);
	return r;
}

/* Change SDC Directory */
int sdchdir(dir)
char *dir;
{
	int r;
	char *p;

	strcpy(_buf, "D:");
	strcat(_buf, dir);
	// for (p=dir; *p != 0; p++)
	// 	printf("%02x", *p);
	// printf("\n");
	// printf("CHDIR:%s\n", _buf);
  	r = CommSDC(0xe0, 0,0,0, _buf);
	// printf("CHDIR:STATUS:%02x\n", r);
	return r;
}


/*
this command only retrieves the leaf name
of the current directory, not the full path. 
 * NOTE: The result of this command may be
 * SPACE Padded -- Space Padded strings are
 * not compatible with C NULL Terminated 
 * Strings
*/
int sdgetcwd(buf)
char *buf;
{
	int r;
	char * p;

	*buf = 0;
	r = CommSDC(0xc0, 0x43,0,0, _buf);
	return r;
}

/* sdgetpth recursive worker */
int _sdgetpth(buf, n)
char *buf;
int n;
{
	int r;
	char dir[33];
	char *p;

	r = sdgetcwd(_buf);
	if ( r == 0x90 )
		strcpy(buf, "/");
	else if ( ! (r & FAILED ))
	{
		// termstr(_buf);
		strncpyz(dir, _buf, 8);
		// printf("%d: %s\n", n, dir);
		sdchdir("..");
		r = _sdgetpth(buf, n+1);
		strncatz(buf, dir, 8);
		if (n>0)
			strcat(buf, "/");
	}
	if (n==0)
	{
		// printf("%d: %s\n", n, buf);
		r = sdchdir(buf);
		// printf("STATUS: %02x\n", r);
	}
	return r;
}

/* 
   Get the current SDC path.  Uses the algorithm from the CoCo SDC Manual:
   It is possible to construct the full path of the current directory by
   using the Set Current Directory command to walk up the directory
   hierarchy one step at a time (using “..”), retrieving each name along
   the way. Once you have the full path you can use it to restore the
   Current Directory to its original location.
*/
int sdgetpth(buf)
char *buf;
{
	*buf = 0;
	return _sdgetpth(buf, 0);
}



/*
 * getdir
 * Scroll through directory pages and copy them to dir.
 * path - path to list or NULL for cwd
 * dir - array of struct dirent
 * n - number of entries in array
 */
int getdir(path, dir, n)
char *path;
struct dirent* dir;
int n;
{
	int i,r, end, m;
	struct dirent *p;
	char buf[256];
	char name[9];
	char ext[4];

	r = sdlstdir(path, 0);
	if (r & FAILED)
		return 0;
	end = 0;
	m = 0;
	while(n && (! end))
	{
		r = sdgtdpag(buf);
		if (r & FAILED)
			break;
		for (i=0, p = (struct dirent *)buf ; n && i<16; i++, p++, n--, dir++)
		{
			*dir = *p;
			if (p->name[0] == 0)
			{
				end = 1;
				break;
			}
		       	else
			{
				m++;
			}
		}
	}
	return m;
}

