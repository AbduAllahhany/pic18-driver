/* 
 * File:   mcal_timer1.h
 * Author: Honey
 *
 * Created on October 4, 2023, 6:17 PM
 */

#ifndef MCAL_TIMER1_H
#define	MCAL_TIMER1_H

/*Section include*/

#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/*Section Macro declaration*/
#define TIMER1_ON                                           0x01
#define TIMER1_OFF                                          0x00

#define TIMER1_COUNTER_MODE                                 0x01
#define TIMER1_TIMER_MODE                                   0x00

#define TIMER1_OSC_ON                                       0x01
#define TIMER1_OSC_OFF                                      0x00

#define TIMER_COUNTER_SYNC                                  0x01
#define TIMER_COUNTER_ASYNC                                 0x00



/*Section Macro function declaration*/
#define TIMER1_ENABLE()                                     T1CONbits.TMR1ON = 1
#define TIMER1_DISABLE()                                    T1CONbits.TMR1ON = 0

#define TIMER1_COUNTER_MODE_ENABLE()                        T1CONbits.TMR1CS = 1
#define TIMER1_TIMER_MODE_ENABLE()                          T1CONbits.TMR1CS = 0

#define TIMER1_COUNTER_SYNC()                               T1CONbits.T1SYNC = 0
#define TIMER1_COUNTER_ASYNC()                              T1CONbits.T1SYNC = 1

#define TIMER1_OSC_ENABLE()                                 T1CONbits.T1OSCEN = 1
#define TIMER1_OSC_DISABLE()                                T1CONbits.T1OSCEN = 0

#define TIMER_COUNTER_SYNC_ENABLE()                         T1CONbits.T1SYNC = 0
#define TIMER_COUNTER_ASYNC_ENABLE()                        T1CONbits.T1SYNC = 1

#define TIMER1_RW_REG_8BIT_MODE_ENABLE()                    T1CONbits.RD16 = 0
#define TIMER1_RW_REG_16BIT_MODE_ENABLE()                   T1CONbits.RD16 = 1

#define TIMER1_PRESCALER_VAL(_VAL)                          T1CONbits.T1CKPS = _VAL

#define TIMER1_SYSTEM_CLOCK_STATUS()                        T1CONbits.T1RUN

/*Section Data type declaration*/

typedef enum{
    TIMER1_PRESCALER_DIV_BY_1 = 0,
    TIMER1_PRESCALER_DIV_BY_2,
    TIMER1_PRESCALER_DIV_BY_4,
    TIMER1_PRESCALER_DIV_BY_8,
}Timer1_Prescaler_Val;

typedef struct{
    uint16  preloaded_val;
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
   Interrupt_Handler timer1_handler;
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    uint8  _priority             :1;
#endif
#endif
    uint8   prescaler_val       :2;
    uint8   timer_mode          :1;
    /*counter mode configuration*/
    uint8   OSC_status          :1;
    uint8   sync_counter_mode   :1;

}Timer1_cnfg_t;

/*Section fun prototype*/
Std_ReturnType mcal_Timer1_initialize(const Timer1_cnfg_t* _timer1);
Std_ReturnType mcal_Timer1_deinitialize(const Timer1_cnfg_t* _timer1);
Std_ReturnType mcal_Timer1_read_val(const Timer1_cnfg_t* _timer1, uint16* val);
Std_ReturnType mcal_Timer1_write_val(const Timer1_cnfg_t* _timer1, uint16 val);

#endif	/* MCAL_TIMER1_H */

