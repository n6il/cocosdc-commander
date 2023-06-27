#include <cmoc.h>
#include "commsdc.h"

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
		status = *(unsigned char *)STATREG;
		if (status & FAILED)
		{
			// status = FAILED;
			break;
		}
		if (status & READY)
		{
			// status = READY;
			break;
		}
		if (! (status & BUSY))
		{
			// status = OK;
			break;
		}
		status = 0x81; /* timeout */
	}
	// if (status & BUSY)
	// 	status = BUSY;
	//printf("STATUS:%02x\n", status);
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
	unsigned char c;
	char *p;

	/* Put controller in Command mode */
	*(unsigned char *)CTRLATCH = CMDMODE;

	/* Put input parameters into the hardware registers.
	 * It does no harm to put random data in the
	 * registers for commands which do not uset them */
	*(unsigned char *)PREG1  = prm1;
	*(unsigned char *)PREG2  = prm2;
	*(unsigned char *)PREG3  = prm3;

	/* Wait for not busy */
	r = waitForIt();
	if (! (r & FAILED))
	{
		/* Send command to controller */
		//  printf("C:%x\n", cmd);
		*(unsigned char *)CMDREG = cmd;

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
					*(unsigned char *)DATREGA = *p++;
					*(unsigned char *)DATREGB = *p++;
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
				*p++ = *(unsigned char *)DATREGA;
				*p++ = *(unsigned char *)DATREGB;
			}
		}
	}

	*(unsigned char *)CTRLATCH = 0;
	return r;
}
