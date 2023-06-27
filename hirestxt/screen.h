#ifndef SCREEN_H
#define SCREEN_H

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

#endif
