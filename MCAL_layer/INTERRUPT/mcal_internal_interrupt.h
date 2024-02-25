/* 
 * File:   mcal_internal_interrupt.h
 * Author: Honey
 *
 * Created on September 22, 2023, 1:48 AM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/*Section include*/
#include "mcal_interrupt_manger.h"

/*Section Macro declaration*/

/*Section Macro function declaration*/
#if ADC_INTERRUPT_CONFG == INTERRUPT_ENABLE
  /*ADC INTERRUPT*/ 
#define ADC_CLEAR_INTERRUPT_FLAG()                            PIR1bits.ADIF = 0    
#define ADC_INTERRUPT_ENABLE()                                PIE1bits.ADIE = 1
#define ADC_INTERRUPT_DISABLE()                               PIE1bits.ADIE = 0

#if PIRIORITY_ENABLE == PIRIORITY_ON  
#define ADC_SET_HIGH_PRIORITY()                               IPR1bits.ADIP = 1
#define ADC_SET_LOW_PRIORITY()                                IPR1bits.ADIP = 0

#endif
#endif


/*TIMER0 INTERRUPT*/
#if TIMER0_INTERRUPT_CONFG == INTERRUPT_ENABLE

#define TIMER0_INTERRUPT_ENABLE()                             INTCONbits.TMR0IE = 1
#define TIMER0_INTERRUPT_DISABLE()                            INTCONbits.TMR0IE = 0
#define TIMER0_CLEAR_INTERRUPT_FLAG()                         INTCONbits.TMR0IF = 0


#if PIRIORITY_ENABLE == PIRIORITY_ON 
#define TIMER0_SET_HIGH_PRIORITY()                            INTCON2bits.TMR0IP = 1
#define TIMER0_SET_LOW_PRIORITY()                             INTCON2bits.TMR0IP = 0
#endif

#endif


/*timer1 interrupt*/
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE

#define TIMER1_INTERRUPT_ENABLE()                             PIE1bits.TMR1IE = 1
#define TIMER1_INTERRUPT_DISABLE()                            PIE1bits.TMR1IE = 0
#define TIMER1_CLEAR_INTERRUPT_FLAG()                         PIR1bits.TMR1IF = 0


#if PIRIORITY_ENABLE == PIRIORITY_ON 
#define TIMER1_SET_HIGH_PRIORITY()                            IPR1bits.TMR1IP = 1
#define TIMER1_SET_LOW_PRIORITY()                             IPR1bits.TMR1IP = 0
#endif

#endif

/*timer2 interrupt*/
#if TIMER2_INTERRUPT_CONFG == INTERRUPT_ENABLE

#define TIMER2_INTERRUPT_ENABLE()                             PIE1bits.TMR2IE = 1
#define TIMER2_INTERRUPT_DISABLE()                            PIE1bits.TMR2IE = 0
#define TIMER2_CLEAR_INTERRUPT_FLAG()                         PIR1bits.TMR2IF = 0


#if PIRIORITY_ENABLE == PIRIORITY_ON 
#define TIMER2_SET_HIGH_PRIORITY()                            IPR1bits.TMR2IP = 1
#define TIMER2_SET_LOW_PRIORITY()                             IPR1bits.TMR2IP = 0
#endif
#endif

/*timer3 interrupt*/
#if TIMER3_INTERRUPT_CONFG == INTERRUPT_ENABLE

#define TIMER3_INTERRUPT_ENABLE()                             PIE2bits.TMR3IE = 1
#define TIMER3_INTERRUPT_DISABLE()                            PIE2bits.TMR3IE = 0
#define TIMER3_CLEAR_INTERRUPT_FLAG()                         PIR2bits.TMR3IF = 0


#if PIRIORITY_ENABLE == PIRIORITY_ON 
#define TIMER3_SET_HIGH_PRIORITY()                            IPR2bits.TMR3IP = 1
#define TIMER3_SET_LOW_PRIORITY()                             IPR2bits.TMR3IP = 0
#endif

#endif

/*CCPx interrupt*/
#if CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE || CCP2_INTERRUPT_CONFG == INTERRUPT_ENABLE
/*  CCP2 */
#define CCP2_INTERRUPT_ENABLE()                               PIE2bits.CCP2IE = 1
#define CCP2_INTERRUPT_DISABLE()                              PIE2bits.CCP2IE = 0
#define CCP2_CLEAR_INTERRUPT_FLAG()                           PIR2bits.CCP2IF = 0
/*  CCP1 */
#define CCP1_INTERRUPT_ENABLE()                               PIE1bits.CCP1IE = 1
#define CCP1_INTERRUPT_DISABLE()                              PIE1bits.CCP1IE = 0
#define CCP1_CLEAR_INTERRUPT_FLAG()                           PIR1bits.CCP1IF = 0


#if PIRIORITY_ENABLE == PIRIORITY_ON 
#define CCP2_SET_HIGH_PRIORITY()                              IPR2bits.CCP2IP = 1
#define CCP2_SET_LOW_PRIORITY()                               IPR2bits.CCP2IP = 0

#define CCP1_SET_HIGH_PRIORITY()                              IPR1bits.CCP1IP = 1
#define CCP1_SET_LOW_PRIORITY()                               IPR1bits.CCP1IP = 0

#endif

#endif
/*EUSART interrupt*/

#if EUSART_INTERRUPT_CONFG == INTERRUPT_ENABLE

#if EUSART_TRANSMIT_INTERRUPT_CONFG == INTERRUPT_ENABLE
#define EUSART_TRANSMIT_INTERRUPT_ENABLE()                   PIE1bits.TXIE = 1
#define EUSART_TRANSMIT_INTERRUPT_DISABLE()                  PIE1bits.TXIE = 0
#define EUSART_TRANSMIT_CLEAR_INTERRUPT_FLAG()               PIR1bits.RCIF = 0

#if PIRIORITY_ENABLE == PIRIORITY_ON 

#define EUSART_RECEIVE_SET_HIGH_PRIORITY()                   IPR1bits.RCIP = 1
#define EUSART_RECEIVE_SET_LOW_PRIORITY()                    IPR1bits.RCIP = 0

#endif
#endif

#if EUSART_RECEIVE_INTERRUPT_CONFG == INTERRUPT_ENABLE

#define EUSART_RECEIVE_INTERRUPT_ENABLE()                    PIE1bits.RCIE = 1
#define EUSART_RECEIVE_INTERRUPT_DISABLE()                   PIE1bits.RCIE = 0   
#define EUSART_RECEIVE_CLEAR_INTERRUPT_FLAG()                PIR1bits.RCIF = 0

#if PIRIORITY_ENABLE == PIRIORITY_ON 

#define EUSART_TRANSMIT_SET_HIGH_PRIORITY()                  IPR1bits.TXIP = 1
#define EUSART_TRANSMIT_SET_LOW_PRIORITY()                   IPR1bits.TXIP = 0

#endif
#endif

#endif
/*I2C interrupt*/
#if I2C_INTERRUPT_CONFG == INTERRUPT_ENABLE

#define I2C_INTERRUPT_ENABLE()                               PIE1bits.SSPIE = 1
#define I2C_INTERRUPT_DISABLE()                              PIE1bits.SSPIE = 0   
#define I2C_CLEAR_INTERRUPT_FLAG()                           PIR1bits.SSPIF = 0

#define I2C_BUS_COL_INTERRUPT_ENABLE()                       PIE2bits.BCLIE = 1
#define I2C_BUS_COL_INTERRUPT_DISABLE()                      PIE2bits.BCLIE = 0
#define I2C_BUS_COL_CLEAR_INTERRUPT_FLAG()                   PIR2bits.BCLIF = 0


#if PIRIORITY_ENABLE == PIRIORITY_ON 

#define I2C_SET_HIGH_PRIORITY()                              IPR1bits.SSPIP = 1
#define I2C_SET_LOW_PRIORITY()                               IPR1bits.SSPIP = 0

#define I2C_BUS_COL_SET_HIGH_PRIORITY()                      IPR2bits.BCLIP = 1
#define I2C_BUS_COL_SET_LOW_PRIORITY()                       IPR2bits.BCLIP = 0

#endif
#endif


/*Section Data type declaration*/
/*Section fun prototype*/


#endif	/* MCAL_INTERNAL_INTERRUPT_H */

