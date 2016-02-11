/*
 *	Serial port driver (uses bit-banging)
 *	for 16Cxx series parts.
 *
 * 	IMPORTANT: Compile this file with FULL optimization
 *
 *	Copyright (C)1996 HI-TECH Software.
 *	Freely distributable.
 */
#include	<htc.h>
/*
 *	Tunable parameters
 */

/*	Xtal frequency */
#define	XTAL		4000000

/*	Baud rate	*/
#define	BRATE		9600

/*	Transmit and Receive port bits */
#define	TxData		GP2				/* Map TxData to pin */
#define	RxData		GP3				/* Map RxData to pin */
#define	INIT_PORT	TRISIO = 1<<7	/* set up I/O direction */

/*	Don't change anything else */
#define SCALER			10000000
#define ITIME			4*SCALER/XTAL	/* Instruction cycle time */
#if BRATE > 1200
	#define	DLY			3				/* cycles per null loop */
	#define	TX_OHEAD	13				/* overhead cycles per loop */
#else
	#define	DLY			9				/* cycles per null loop */
	#define TX_OHEAD	14
#endif
#define	RX_OHEAD		12				/* receiver overhead per loop */

#define	DELAY(ohead)	(((SCALER/BRATE)-(ohead*ITIME))/(DLY*ITIME))

void
putch(char c)
{
	unsigned char	bitno;
#if BRATE > 1200
	unsigned char	dly;
#else
	unsigned int	dly;
#endif

	INIT_PORT;
	TxData = 0;			/* start bit */
	bitno = 12;
	do {
		dly = DELAY(TX_OHEAD);	/* wait one bit time */
		do
			/* waiting in delay loop */ ;
		while(--dly);
		if(c & 1)
			TxData = 1;
		if(!(c & 1))
			TxData = 0;
		c = (c >> 1) | 0x80;
	} while(--bitno);
NOP();
}

char
getch(void)
{
	unsigned char	c, bitno;
#if BRATE > 1200
	unsigned char	dly;
#else
	unsigned int	dly;
#endif

	for(;;) {
		while(RxData)
			continue;		/* wait for start bit */
		dly = DELAY(3)/2;
		do
							/* waiting in delay loop */ ;
		while(--dly);
		if(RxData)
			continue;		/* twas just noise */
		bitno = 8;
		c = 0;
		do {
			dly = DELAY(RX_OHEAD);
			do
							/* waiting in delay loop */ ;
			while(--dly);
			c = (c >> 1) | (RxData << 7);
		} while(--bitno);
		return c;
	}
}

char
getche(void)
{
	char c;

	putch(c = getch());
	return c;
}
