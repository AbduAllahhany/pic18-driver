/* 
 * File:   mcal_interrupt_manger.h
 * Author: Honey
 *
 * Created on September 20, 2023, 5:17 PM
 */

#ifndef MCAL_INTERRUPT_MANGER_H
#define	MCAL_INTERRUPT_MANGER_H
/*Section include*/

#include "pic18f4620.h"
#include "../GPIO/hal_gpio.h"
#include "../mcal_std_types.h"
#include "mcal_interrupt_cnfg.h"

/*Section Macro declaration*/

#define PIRIORITY_ON              0x1
#define PIRIORITY_OFF             0x0

#define INTERRUPT_OCCUR                             0x01
#define INTERRUPT_ENABLE                            0x01

#define high_src                                    0x01
#define low_src                                     0x00
/*Section Macro function declaration*/


#if PIRIORITY_ENABLE == PIRIORITY_ON    

#define READ_ALL_HIGH_PRIORITY_BIT()                INTCONbits.GIEH 
#define READ_ALL_LOW_PRIORITY_BIT()                 INTCONbits.GIEL 
#define DISABLE_PRIORITY_BIT()                      RCONbits.IPEN   = 0
#define ENABLE_PRIORITY_BIT()                       RCONbits.IPEN   = 1
#define DISABLE_ALL_HIGH_PRIORITY()                 INTCONbits.GIEH = 0
#define ENABLE_ALL_HIGH_PRIORITY()                  INTCONbits.GIEH = 1
#define DISABLE_ALL_LOW_PRIORITY()                  INTCONbits.GIEL = 0
#define ENABLE_ALL_LOW_PRIORITY()                   INTCONbits.GIEL = 1

#else

#define READ_GLOBAL_INTERRUPT_BIT()                 INTCONbits.GIE
#define READ_PERIPHERAL_INTERRUPT_BIT()             INTCONbits.PEIE
#define DISABLE_PRIORITY_BIT()                      RCONbits.IPEN   = 0
#define DISABLE_GLOBAL_INTERRUPT_BIT()              INTCONbits.GIE  = 0
#define ENABLE_GLOBAL_INTERRUPT_BIT()               INTCONbits.GIE  = 1
#define DISABLE_PERIPHERAL_INTERRUPT_BIT()          INTCONbits.PEIE = 0
#define ENABLE_PERIPHERAL_INTERRUPT_BIT()           INTCONbits.PEIE = 1


#endif

/*Section Data type declaration*/
typedef void (*Interrupt_Handler)(void); 

typedef enum{
     LOW_PRIORITY = 0,
     HIGH_PRIORITY
}interrupt_priority;

typedef enum{
     FALLING_EDGE = 0,
     RISING_EDGE
}interrupt_edge;

/*Section fun prototype*/
 void INT0_ISR();
 void INT1_ISR();
 void INT2_ISR();
 
 void RB4_ISR(uint8 src);
 void RB5_ISR(uint8 src);
 void RB6_ISR(uint8 src);
 void RB7_ISR(uint8 src);
 
 void ADC_ISR(void);
 
 void TIMER0_ISR(void);
 void TIMER1_ISR(void);
 void TIMER2_ISR(void);
 void TIMER3_ISR(void);
 
 void CCP1_ISR(void);
 void CCP2_ISR(void);
 
void Rx_ISR(void);
void Tx_ISR(void);
void I2C_ISR(void);
void I2C_BUS_COL_ISR(void);

#endif	/* MCAL_INTERRUPT_MANGER_H */

