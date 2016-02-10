/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"


/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/


/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void){
    /* TODO Initialize User Ports/Peripherals/Project here */
    GPIO = 0b00000000;
    TRISIO = 0b00101111;

    /* Setup analog functionality and port direction */

    /* Initialize peripherals */

    /* Enable interrupts */
    IOC = 0b00000000;// Disable
    
#if defined(BBK)    
    // Select
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.INTE = 0;
    INTCONbits.GPIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.INTF = 0;
    INTCONbits.GPIF = 0;
    
    // Enable
    IOCbits.IOC5 = 0;
    IOCbits.IOC4 = 0;
    IOCbits.IOC3 = 0;
    IOCbits.IOC2 = 1;
    IOCbits.IOC1 = 0;
    IOCbits.IOC0 = 0;
    
    /* Timer */
    OPTION_REGbits.nGPPU = 0;
    OPTION_REGbits.INTEDG = 0;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;
#endif
#if defined(SKY)    
    // Select
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.INTE = 0;
    INTCONbits.GPIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.INTF = 0;
    INTCONbits.GPIF = 0;
    
    // Enable
    IOCbits.IOC5 = 0;
    IOCbits.IOC4 = 0;
    IOCbits.IOC3 = 0;
    IOCbits.IOC2 = 1;
    IOCbits.IOC1 = 0;
    IOCbits.IOC0 = 0;
    
    /* Timer */
    OPTION_REGbits.nGPPU = 0;
    OPTION_REGbits.INTEDG = 0;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
#endif
    
    TMR0 = 0;//Clear the TMR0 register
    GPIF = 0;
    T0IF = 0;    
}

void ExecCmd(unsigned char ucAddr, unsigned char ucCmd){
    if((ucAddr == 128) && (ucCmd == 96)){
        GP4 = ~GP4;
    }
}