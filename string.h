#include "proto.h"

#ifndef MY_STRING_H
#define MY_STRING_H 

char *strcpyz _OP((char *, char *));
char *strncpyz _OP((char *, char *, int));
char *strncatz _OP((char *, char *, int));
char *termstr _OP((char *));

#ifdef FLEX
char *memcpy _OP((char *, char *, int));
char *strchr _OP((char *, char));
#endif

#endif

/*
# vim: ts=8 sw=8 sts=8 et
*/
