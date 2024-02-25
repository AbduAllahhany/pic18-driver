/* 
 * File:   mcal_timer2.h
 * Author: Honey
 *
 * Created on October 5, 2023, 2:41 PM
 */

#ifndef MCAL_TIMER2_H
#define	MCAL_TIMER2_H

/*Section include*/

#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/*Section Macro declaration*/

#define TIMER2_ON                                           0x1
#define TIMER2_OFF                                          0x0

/*Section Macro function declaration*/
#define TIMER2_ENABLE()                                     T2CONbits.TMR2ON = 1
#define TIMER2_DISABLE()                                    T2CONbits.TMR2ON = 0

#define TIMER2_PRESCALER_CNFG(_VAL)                         T2CONbits.T2CKPS = _VAL

#define TIMER2_POSTSCALER_CNFG(_VAL)                        T2CONbits.TOUTPS = _VAL

/*Section Data type declaration*/
typedef enum{
    TIMER2_POSTSCALER_DIV_BY_1 = 0,
    TIMER2_POSTSCALER_DIV_BY_2,
    TIMER2_POSTSCALER_DIV_BY_3,
    TIMER2_POSTSCALER_DIV_BY_4,
    TIMER2_POSTSCALER_DIV_BY_5,
    TIMER2_POSTSCALER_DIV_BY_6,
    TIMER2_POSTSCALER_DIV_BY_7,
    TIMER2_POSTSCALER_DIV_BY_8,
    TIMER2_POSTSCALER_DIV_BY_9,
    TIMER2_POSTSCALER_DIV_BY_10,
    TIMER2_POSTSCALER_DIV_BY_11,
    TIMER2_POSTSCALER_DIV_BY_12,
    TIMER2_POSTSCALER_DIV_BY_13,
    TIMER2_POSTSCALER_DIV_BY_14,
    TIMER2_POSTSCALER_DIV_BY_15,
    TIMER2_POSTSCALER_DIV_BY_16
}Timer2_Postscale_val;
typedef enum{
    TIMER2_PRESCALER_DIV_BY_1,
    TIMER2_PRESCALER_DIV_BY_4,
    TIMER2_PRESCALER_DIV_BY_16
}Timer2_Prescale_val;

typedef struct{
#if TIMER2_INTERRUPT_CONFG == INTERRUPT_ENABLE
    Interrupt_Handler timer2_interrupt_handler;
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    uint8 priority :1;
#endif 
#endif
    uint8 timer2_preloaded_val;
    uint8 prescaler_val  :2;
    uint8 postscaler_val :4;

}Timer2_cnfg_t;

/*Section fun prototype*/
Std_ReturnType mcal_Timer2_initialize(const Timer2_cnfg_t* timer2);
Std_ReturnType mcal_Timer2_deinitialize(const Timer2_cnfg_t* timer2);
Std_ReturnType mcal_Timer2_write_val(const Timer2_cnfg_t* timer2,uint8 val);
Std_ReturnType mcal_Timer2_read_val(const Timer2_cnfg_t* timer2,uint8 *val);

#endif	/* MCAL_TIMER2_H */

