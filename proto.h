#ifndef PROTO_H
#define PROTO_H

#ifdef DECB
#ifndef _OP
#define _OP(x) x
#endif
#ifndef VOLATILE
#define VOLATILE
#endif
#endif

#ifdef OS9
#ifndef _OP
#define _OP(x) ()
#endif
#ifndef _MEMCPY
#define _MEMCPY
#endif
#ifndef VOLATILE
#define VOLATILE
#endif
#endif

#ifdef FLEX
#ifndef _OP
#define _OP(x) ()
#endif
#ifndef _MEMCPY
#define _MEMCPY
#endif
#ifndef VOLATILE
#define VOLATILE
#endif
#endif

#ifdef FUZIX
#ifndef _OP
#define _OP(x) x
#endif
#ifndef VOLATILE
#define VOLATILE volatile
#endif
#endif

#endif

/*
# vim: ts=8 sw=8 sts=8 et
*/
