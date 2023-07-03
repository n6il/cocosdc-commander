 .module irq.S

 .area   .text

 .globl  _maskIntr

_maskIntr:
 orcc #$50
 rts

 .globl  _rstrIntr

_rstrIntr:
 andcc #$af
 rts
