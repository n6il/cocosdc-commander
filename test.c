#include "proto.h"

#define DECB

#ifdef __STDC__
#define FOO bar
#else
#define FOO baz
#endif

int DEFFN(hello, char *, int)

int hello(str, len)
char *str;
int len
{
	return FOO;
}

