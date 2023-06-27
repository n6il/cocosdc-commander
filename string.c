#include <cmoc.h>
#include "string.h"

/* strcpyz
 * Copy from src to dest
 * Strings are terminated by SPACE or NULL
 * Result is always NULL terminated
 */
char *strcpyz(dst, src)
char *dst, *src;
{
	while( *src && *src != 0x20)
		*dst++ = *src++;
	*dst = 0;
	return dst;
}

/* strncpyz
 * Copy up to len bytes from src to dest
 * Strings are terminated by SPACE or NULL
 * Result is always NULL terminated
 */
char *strncpyz(dst, src, len)
char *dst, *src;
int len;
{
	while( *src && *src != 0x20 && len-- )
		*dst++ = *src++;
	*dst = 0;
	return dst;
}

/* strncatz
 * Concatenate dst and up to len bytes
 * from src.
 * Strings are terminated by SPACE or NULL
 * Result is always NULL terminated
 */
char *strncatz(dst, src, len)
char *dst, *src;
int len;
{
	while( *dst && *dst != 0x20 )
		dst++;
	return strncpyz(dst, src, len);
}

char * termstr(str)
char *str;
{
	char *p;
	p = strchr(str, ' ');
	if (p)
		*p = 0;
	return p;
}


