#ifdef DECB
#include <cmoc.h>
#endif
#include "commsdc.h"

char mpiSlot = -1;

#ifdef DECB
int maskIntr()
{
        asm {
                orcc #$50
        }
         return 0;
}

int rstrIntr()
{
        asm {
                andcc #$AF
        }
         return 0;
}

#endif

#ifdef OS9
int maskIntr()
{
#asm
 orcc #$50
#endasm
         return 0;
}

int rstrIntr()
{
#asm
 andcc #$AF
#endasm
         return 0;
}
#endif


#ifdef FLEX
int maskIntr()
{
#asm
 orcc #$50
#endasm
         return 0;
}

int rstrIntr()
{
#asm
 andcc #$AF
#endasm
         return 0;
}
#endif

#ifdef FUZIX
void maskIntr();
void rstrIntr();
#endif

/* Find which MPI Slot the SDC Is in */
/* Returns -1 if not found */
int FindSDC()
{
        unsigned char mpiSav, curMpi, t1;

        maskIntr();
        /* Save current MPI Slot */
        mpiSav = *(VOLATILE unsigned char *)MPIREG;
        /* Start with slot 4 */
        curMpi = mpiSav & 0x33 | 0x03;
        while (! (curMpi & 0x08) )
        {
                /* Activate slot being scanned */
                *(VOLATILE unsigned char *)MPIREG = curMpi;
                /* Store test pattern to Flash Data Reg */
                *(VOLATILE unsigned char *)FLSHDATA = 0x64;
                /* Get value from Flash Control Reg */
                t1 = *(VOLATILE unsigned char *)FLSHCTRL;
                /* Clear Flash Data Reg */
                *(VOLATILE unsigned char *)FLSHDATA = 0;
                /* Did expected bits change? */
                t1 ^= *(VOLATILE unsigned char *)FLSHCTRL;
                t1 -= 0x60;
                if (!t1)
                        break;
                curMpi--;
        }
        if ( curMpi & 0x08 )
                /* Not found */
                mpiSlot = -1;
        else
                /* Found */
                mpiSlot = curMpi;
        *(VOLATILE unsigned char *)MPIREG = mpiSav;
        rstrIntr();
        return mpiSlot;
}

/* Return - 
 *    OK - Command Completed
 *  READY - Command ready to return data
 * FAILED - Command Failed
 *   BUSY - Command Timeout
 */
int waitForIt()
{
        register counter;
        unsigned char status;

        counter = 0;
        while(--counter)
        {
                status = *(VOLATILE unsigned char *)STATREG;
                if (status & FAILED)
                {
                        break;
                }
                if (status & READY)
                {
                        break;
                }
                if (! (status & BUSY))
                {
                        break;
                }
                status = 0x81; /* timeout */
        }
        return status;
}

int longWait()
{
        int status;
        status = waitForIt();
        if (status & FAILED)
                status = waitForIt();
        return status;
}

int CommSDC(cmd, prm1, prm2, prm3, buf)
unsigned char cmd, prm1, prm2, prm3;
char *buf;
{
        int r,i;
        unsigned char c, mpi;
        char *p;

        maskIntr();
        /* Switch MPI Slot */
        if (mpiSlot >= 0)
        {
                mpi = *(VOLATILE unsigned char *)MPIREG;
                *(VOLATILE unsigned char *)MPIREG = mpiSlot;
        }

        /* Put controller in Command mode */
        *(VOLATILE unsigned char *)CTRLATCH = CMDMODE;

        /* Put input parameters into the hardware registers.
         * It does no harm to put random data in the
         * registers for commands which do not uset them */
        *(VOLATILE unsigned char *)PREG1  = prm1;
        *(VOLATILE unsigned char *)PREG2  = prm2;
        *(VOLATILE unsigned char *)PREG3  = prm3;

        /* Wait for not busy */
        r = waitForIt();
        if (! (r & FAILED))
        {
                /* Send command to controller */
                *(VOLATILE unsigned char *)CMDREG = cmd;

                /* Determine if any data block needs to be sent.
                 * Any command which requires a data block will
                 * have bit 5 set in the command code */
                if (cmd & 0x20)
                {
                        /* Wait for Ready to send */
                        r = waitForIt();
                        if (! (r &  FAILED))
                        {
                                /* Send 256 bytes of data */
                                for(i=0, p=buf; i<128;i++)
                                {
                                        *(VOLATILE unsigned char *)DATREGA = *p++;
                                        *(VOLATILE unsigned char *)DATREGB = *p++;
                                }

                                /* Wait for command completion */
                                for (i=0; i<5; i++)
                                {
                                        r = waitForIt();
                                        if (! (r & BUSY)) /* not busy and not failed */
                                                break;
                                }
                        }
                }

                /* For commands which return a 256-byte response block the
                 * controller will set the READY bit in the Status register
                 * when it has the data ready for transfer.  For commands
                 * which do not return a response block the BUSY bit will
                 * be cleared to indicate that the command has completed.
                 */
                r = longWait();
                if (r & READY && buf != 0xffff)
                {
                        for(i=0, p=buf; i<128;i++)
                        {
                                *p++ = *(VOLATILE unsigned char *)DATREGA;
                                *p++ = *(VOLATILE unsigned char *)DATREGB;
                        }
                }
        }

        *(VOLATILE unsigned char *)CTRLATCH = 0;
        if (mpiSlot >= 0)
                *(VOLATILE unsigned char *)MPIREG = mpi;

        rstrIntr();
        return r;
}

/*
# vim: ts=8 sw=8 sts=8 et
*/
