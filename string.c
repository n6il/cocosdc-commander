#ifdef DECB
#include <cmoc.h>
#endif
#include "string.h"
#ifdef FUZIX
#include <string.h>
#endif

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

#ifdef FLEX
/*
** Copy n bytes from s2 to s1.
** Return s1
*/

char *memcpy(s1, s2, n)
register char  *s1, *s2;
int            n;
{
        char  *os1 = s1;

        while (n-- > 0)
                *s1++ = *s2++;
        return (os1);
}


/*
** Return a pointer to the char 'c' in 's' if found
**  else return NULL
*/

char  *strchr(s, c)
register char  *s, c;
{
        do {
                if (*s == c)
                        return (s);
        } while (*s++);
        return ((char *) 0);
}
#endif

/*
# vim: ts=8 sw=8 sts=8 et
*/
