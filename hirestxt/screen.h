#ifndef SCREEN_H
#define SCREEN_H

#define BANNERLN 0
#define CWDLN 2
#define DIRSTLN 4
#define PROMPTLN 21

#define KEY_BRK		0x03
#define KEY_BS 		0x08
#define KEY_ENTER	0x0D
#define KEY_DOWN	0x0A
#define KEY_EXP		0x21
#define KEY_AT		0x22
#define KEY_ZERO	0x30
#define KEY_ONE		0x31
#define KEY_TWO		0x32
#define KEY_C		0x43
#define KEY_M		0x4D
#define KEY_PGDN	0x5B
#define KEY_UP		0x5E
#define KEY_PGUP	0x5F


int scr_init();
int csr_up();
int csr_dn();
int csr_rt();
int csr_lt();
int csr_home();
int clr_eos();
int clr_eol();
int csr_pos(unsigned char, unsigned char);
int scr_cls();

#define puts(s) putstr(s, strlen(s))

#define getchar() waitkey(1)
char *gets(char *);

#endif
