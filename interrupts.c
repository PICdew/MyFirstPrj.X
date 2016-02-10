/******************************************************************************/
/*Files to Include                                                            */
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
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Unfortunately the baseline detection 
 * macro is named _PIC12 */

#ifndef _PIC12

volatile unsigned char m_ucSteps = 0;
volatile unsigned char m_ucTMR0 = 0;
volatile unsigned char m_ucValue = 0;
volatile unsigned char m_ucAddr_1 = 0;
volatile unsigned char m_ucAddr_2 = 0;
volatile unsigned char m_ucCmd_1 = 0;
volatile unsigned char m_ucCmd_2 = 0;


void interrupt isr(void)
{
#if defined(BBK)
    
    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */
    if(T0IF){// Timer Overflow, keep the value in reset
        // Overflow during the decoding...
        if(m_ucSteps > 0){
            m_ucAddr_1 = 0;
            m_ucAddr_2 = 0;
            m_ucCmd_1 = 0;
            m_ucCmd_2 = 0;
        }
        m_ucValue = 0;
        m_ucSteps = 0;

        TMR0 = 0;
        T0IF = 0;
    }
    
    if(GPIF){        
        // Reset Timer
        m_ucTMR0 = TMR0;
        TMR0 = 0;
        if(!GP2){//Low
            if(m_ucSteps == 1){
                // 76 are 4500 microSecs
                // 4 are 250 microSecs
                if((m_ucTMR0 >= 72) && (m_ucTMR0 <= 80)){// New command
                    m_ucSteps = 2;
                    // Reset old value
                    m_ucAddr_1 = 0;
                    m_ucAddr_2 = 0;
                    m_ucCmd_1 = 0;
                    m_ucCmd_2 = 0;
                }            
                // 38 are 2250 microSecs
                // 4 are 250 microSecs
                if((m_ucTMR0 >= 34) && (m_ucTMR0 <= 42)){// Repeat last command
                    m_ucSteps = 66;
                }            
            }

            if(m_ucSteps >= 3 && m_ucSteps <= 65){               
                // 0 = 562 microSecs
                // 1 = 1675 microSecs
                // 9 are 562 microSecs
                // 28 are 1675 microSecs
                // 2 are 125 microSecs
                if((m_ucTMR0 >= 7) && (m_ucTMR0 <= 11)){
                    m_ucSteps++; // Stop First Bit, 0
                    m_ucValue = m_ucValue << 1;
                    m_ucValue = m_ucValue | 0x00;
                }            
                if((m_ucTMR0 >= 26) && (m_ucTMR0 <= 30)){
                    m_ucSteps++; // Stop First Bit, 1
                    m_ucValue = m_ucValue << 1;
                    m_ucValue = m_ucValue | 0x01;
                }                        

                // Decoding
                if(m_ucSteps == 18){ // First Address byte 
                    m_ucAddr_1 = m_ucValue;
                    m_ucValue = 0;
                }
                if(m_ucSteps == 34){ // Second Address byte 
                    m_ucAddr_2 = ~m_ucValue;
                    m_ucValue = 0;
                }
                if(m_ucSteps == 50){ // First Cmd byte 
                    m_ucCmd_1 = m_ucValue;
                    m_ucValue = 0;
                }
                if(m_ucSteps == 66){ // Second Cmd byte 
                    m_ucCmd_2 = ~m_ucValue;
                    m_ucValue = 0;
                }
            }
        }
        if(GP2){//High
            if(m_ucSteps == 0){                
                // 153 are 9000 microSecs
                // 4 are 250 microSecs
                if((m_ucTMR0 >= 149) && (m_ucTMR0 <= 157)){
                    m_ucSteps = 1;
                }            
            }
            if(m_ucSteps >= 2 && m_ucSteps <= 64){                
                // 9 are 562 microSecs
                // 2 are 125 microSecs
                if((m_ucTMR0 >= 7) && (m_ucTMR0 <= 11)){
                    m_ucSteps++; // Start First Bit
                }            
            } 
            if(m_ucSteps == 66){// End                
                // 9 are 562 microSecs
                // 2 are 125 microSecs
                if((m_ucTMR0 >= 7) && (m_ucTMR0 <= 11)){
                    if((m_ucAddr_1 == m_ucAddr_2) && (m_ucCmd_1 == m_ucCmd_2)){
                        ExecCmd(m_ucAddr_1, m_ucCmd_1);
                    }
                    m_ucSteps = 0;
                }            
            }                 
        }
        GPIF = 0;        
    }
#endif

#if defined(SKY)
    
    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */
    if(T0IF){// Timer Overflow, keep the value in reset

        m_ucValue = 0;
        m_ucSteps = 0;

        m_ucAddr_1 = 0;
        m_ucAddr_2 = 0;
        m_ucCmd_1 = 0;
        m_ucCmd_2 = 0;

//        GP4 = ~GP4;  
        TMR0 = 0;
        T0IF = 0;  
    }
    
    if(GPIF){        
        // Reset Timer
        m_ucTMR0 = TMR0;
        TMR0 = 0;
        if(!GP2){//Low
            if(m_ucSteps == 1){//1
                // 70 center to 1040 microSecs
                if((m_ucTMR0 >= 68) && (m_ucTMR0 <= 72)){// New command
                    m_ucSteps++;
                    //GP4 = ~GP4;   
                    m_ucCmd_1++;
                    m_ucCmd_2++;
                }
            }

            if(m_ucSteps == 3){//2
                // 152 center to 2260 microSecs Key <<
                if((m_ucTMR0 >= 150) && (m_ucTMR0 <= 154)){
                    m_ucSteps++;
                    m_ucCmd_1++;
                    //GP4 = 1;     
                }  
                
                // 133 center to 1980 microSecs Key >>
                if((m_ucTMR0 >= 131) && (m_ucTMR0 <= 135)){
                    m_ucSteps++;
                    m_ucCmd_2++;

                    //GP4 = 0;     
                }  
            }

            if(m_ucSteps == 5){//3
                // 51 center to 760 microSecs Key << >>
                if((m_ucTMR0 >= 49) && (m_ucTMR0 <= 53)){
                    m_ucSteps++;      
                    m_ucCmd_1++;
                    m_ucCmd_2++;
                }  
            }
            if(m_ucSteps == 7){//4
                // 51 center to 760 microSecs Key 
                if((m_ucTMR0 >= 49) && (m_ucTMR0 <= 53)){
                    m_ucSteps++;
                    m_ucCmd_1++;
                    m_ucCmd_2++;
                }  
            }
            if(m_ucSteps == 9){//5
                // 79 center to 1170 microSecs Key 
                if((m_ucTMR0 >= 77) && (m_ucTMR0 <= 81)){
                    m_ucSteps++;
                    m_ucCmd_1++;
                    m_ucCmd_2++;
                }  
            }
            if(m_ucSteps == 11){//6
                // 70 center to 1040 microSecs >>
                if((m_ucTMR0 >= 68) && (m_ucTMR0 <= 72)){// New command
                    m_ucSteps++;
                    m_ucCmd_1++;
                }                
                // 51 center to 760 microSecs Key <<
                if((m_ucTMR0 >= 49) && (m_ucTMR0 <= 53)){
                    m_ucSteps++;
                    m_ucCmd_2++;
                }  
            }
            if(m_ucSteps == 13){//7
                // 51 center to 760 microSecs Key 
                if((m_ucTMR0 >= 49) && (m_ucTMR0 <= 53)){
                    m_ucSteps++;
                    m_ucCmd_1++;
                    m_ucCmd_2++;
                }  
            }
            if(m_ucSteps == 15){//8
                // 51 center to 760 microSecs Key 
                if((m_ucTMR0 >= 49) && (m_ucTMR0 <= 53)){
                    m_ucSteps++;
                    m_ucCmd_1++;
                    m_ucCmd_2++;
                }  
            }
        }
        if(GP2){//High
            if(m_ucSteps >= 0 && m_ucSteps <= 16){                
                // 13 center to 198 microSecs
                if((m_ucTMR0 >= 11) && (m_ucTMR0 <= 15)){
                    m_ucSteps++;
                }     
            }
            if(m_ucSteps == 17){// End     
                m_ucSteps = 0;
                //GP4 = ~GP4;
                if(m_ucCmd_1 == 8){// >>
                    GP4 = 1;
                }
                if(m_ucCmd_2 == m_ucCmd_1){// <<
                    //GP4 = 1;
                }
                m_ucCmd_1 = 0;
                m_ucCmd_2 = 0;
            }  
            
/*            
            if(m_ucSteps >= 2 && m_ucSteps <= 64){                
                // 9 are 562 microSecs
                // 2 are 125 microSecs
                if((m_ucTMR0 >= 7) && (m_ucTMR0 <= 11)){
                    m_ucSteps++; // Start First Bit
                }            
            } 
            if(m_ucSteps == 66){// End                
                // 9 are 562 microSecs
                // 2 are 125 microSecs
                if((m_ucTMR0 >= 7) && (m_ucTMR0 <= 11)){
                    if((m_ucAddr_1 == m_ucAddr_2) && (m_ucCmd_1 == m_ucCmd_2)){
                        ExecCmd(m_ucAddr_1, m_ucCmd_1);
                    }
                    m_ucSteps = 0;
                }            
            }  
 */                
        }
        GPIF = 0;        
    }
#endif

}
#endif


