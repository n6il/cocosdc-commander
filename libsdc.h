#include "proto.h"

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
#define DIR_ATTR_SDF    0x04
#define DIR_ATTR_DIR    0x10
#define isDir(a) (a & DIR_ATTR_DIR)

int sdumnt _OP((int));
int sdmnt _OP((int, char *));
int sdnewimg _OP((int, int, int, char *));
int sdimginf _OP((char *, unsigned char));
int sdgtdpag _OP((char *));
int sdlstdir _OP((char *, char *));
int sdchdir _OP((char *));
int sdgetcwd _OP((char *));
int sdgetpth _OP((char *));
int getdir _OP((char *, struct dirent *, int));
int findsdc();
int sdnxtdsk();

#endif

/*
# vim: ts=8 sw=8 sts=8 et
*/
