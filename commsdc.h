#ifndef COMMSDC_H
#define COMMSDC_H

/* Hardware Addressing - CoCo Scheme */
#define CTRLATCH 0xff40
#define CMDREG   0xff48
#define STATREG  0xff48
#define PREG1    0xff49
#define PREG2    0xff4a
#define PREG3    0xff4b
#define DATREGA  PREG2
#define DATREGB  PREG3

/* Status Register Masks */
#define OK       0x00
#define BUSY     0x01
#define READY    0x02
#define FAILED   0x80

/* Mode and Command Values */
#define CMDMODE  0x43
#define CMDREAD  0x80
#define CMDWRITE 0xa0
#define CMDEX    0xc0
#define CMDEXD   0xd0

/* int CommSDC(cmd, prm1, prm2, prm3, buf) */
int CommSDC(unsigned char, unsigned char, unsigned char, unsigned char, char *);

#endif
