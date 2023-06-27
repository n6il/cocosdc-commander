#ifndef LIBSDC_H
#define LIBSDC_H

struct dirent {
	char name[8];
	char ext[3];
	unsigned char attr;
	char size[4];
};

#define DIR_ATTR_LOCKED 0x01
#define DIR_ATTR_HIDDEN 0x02
#define isHidden(a) (a & DIR_ATTR_HIDDEN)
#define DIR_ATTR_SDF	0x04
#define DIR_ATTR_DIR	0x10
#define isDir(a) (a & DIR_ATTR_DIR)

int sdumnt(int);
int sdmnt(int, char *);
int sdimginf(char *, unsigned char);
int sdgtdpag(char *);
int sdlstdir(char *, char *);
int sdchdir(char *);
int sdgetcwd(char *);
int sdgetpth(char *);
int getdir(char *, struct dirent *, int);


#endif
