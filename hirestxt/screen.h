#ifndef SCREEN_H
#define SCREEN_H

#define BANNERLN 0
#define CWDLN 2
#define DIRSTLN 4
#define PROMPTLN 21

#define KEY_BRK         0x03
#define KEY_BS          0x08
#define KEY_ENTER       0x0D
#define KEY_DOWN        0x0A
#define KEY_SPC         0x20
#define KEY_EXP         0x21
#define KEY_AT          0x22
#define KEY_ZERO        0x30
#define KEY_ONE         0x31
#define KEY_TWO         0x32
#define KEY_B           0x42
#define KEY_b           0x62
#define KEY_C           0x43
#define KEY_c           0x63
#define KEY_M           0x4D
#define KEY_m           0x6D
#define KEY_N           0x4E
#define KEY_n           0x6E
#define KEY_Q           0x51
#define KEY_q           0x71
#define KEY_PGDN        0x5B
#define KEY_UP          0x5E
#define KEY_PGUP        0x5F
#define KEY_PLUS        0x2B

#define WRITE(fd, str, n) putstr(str, n)
#define NL "\n"

int scr_init();
int scr_exit();

int csr_up();
int csr_dn();
int csr_rt();
int csr_lt();
int csr_home();
int clr_eos();
int clr_eol();
int csr_pos(unsigned char, unsigned char);
int scr_cls();

#define scr_getc() waitkey(1)
#define scr_putc(c) putchar(c)
#define scr_puts(s) putstr(s, strlen(s))
char *scr_gets(char *);

#endif

/*
# vim: ts=8 sw=8 sts=8 et
*/
