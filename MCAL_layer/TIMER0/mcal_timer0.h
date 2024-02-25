/* 
 * File:   mcal_timer0.h
 * Author: Honey
 *
 * Created on October 1, 2023, 5:55 PM
 */

#ifndef MCAL_TIMER0_H
#define	MCAL_TIMER0_H

/*Section include*/

#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "mcal_timer0_cnfg.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/*Section Macro declaration*/
#define TIMER0_8BIT_REGISTER                                0x01
#define TIMER0_16BIT_REGISTER                               0x00
#define TIMER0_RISING_EDGE                                  0x01
#define TIMER0_FALLING_EDGE                                 0x00
#define TIMER0_COUNTER_MODE                                 0x01
#define TIMER0_TIMER_MODE                                   0x00
#define TIMER0_PRESCALER_ON                                 0x01
#define TIMER0_PRESCALER_OFF                                0x00

/*Section Macro function declaration*/

#define TIMER0_DISABLE()                                    T0CONbits.TMR0ON = 0
#define TIMER0_ENABLE()                                     T0CONbits.TMR0ON = 1
#define TIMER0_SET_8BIT_REG()                               T0CONbits.T08BIT = 1
#define TIMER0_SET_16BIT_REG()                              T0CONbits.T08BIT = 0
#define TIMER0_COUNTER_MODE_ENABLE()                        T0CONbits.T0CS   = 1
#define TIMER0_TIMER_MODE_ENABLE()                          T0CONbits.T0CS   = 0
#define TIMER0_PRESCALER_ENABLE()                           T0CONbits.PSA    = 0
#define TIMER0_PRESCALER_DISABLE()                          T0CONbits.PSA    = 1
#define TIMER0_RISING_EDGE_INCREMENT()                      T0CONbits.T0SE   = 1
#define TIMER0_FALLING_EDGE_INCREMENT()                     T0CONbits.T0SE   = 0
#define TIMER0_PRESCALER_VAL(_VAL)                          T0CONbits.T0PS   = _VAL

/*Section Data type declaration*/
typedef enum{
    Freq_Divided_by_2 = 0,
    Freq_Divided_by_4 , 
    Freq_Divided_by_8 ,
    Freq_Divided_by_16 ,
    Freq_Divided_by_32 ,
    Freq_Divided_by_64 ,
    Freq_Divided_by_128 ,
    Freq_Divided_by_256 ,
}Timer0_Prescaler_Select_bits;

typedef struct{
#if TIMER0_INTERRUPT_CONFG == INTERRUPT_ENABLE
   Interrupt_Handler timer0_handler;
#if PIRIORITY_ENABLE == PIRIORITY_ON 
   uint8  _priority             :1;
#endif
#endif
   uint16 preloadedval;
   uint8 timer0_register_size   : 1;
   uint8 timer0_select_mode     : 1;
   uint8 timer0_edge_select     : 1;
   uint8 timer0_prescaler_cnfg  : 1;
   uint8 timer0_prescaler_val   : 3;
}timer0_cnfg_t;

/*Section fun prototype*/

Std_ReturnType mcal_timer0_intialize(const timer0_cnfg_t* timer0);
Std_ReturnType mcal_timer0_deinitialize(const timer0_cnfg_t* _timer0);
Std_ReturnType mcal_timer0_read_register(const timer0_cnfg_t* timer0, uint16* val);
Std_ReturnType mcal_timer0_write_value(const timer0_cnfg_t* timer0, uint16 val);


#endif	/* MCAL_TIMER0_H */

