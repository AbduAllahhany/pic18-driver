/* 
 * File:   mcal_timer3.h
 * Author: Honey
 *
 * Created on October 10, 2023, 2:51 PM
 */

#ifndef MCAL_TIMER3_H
#define	MCAL_TIMER3_H
/*Section include*/

#include "pic18f4620.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"
#include "../mcal_std_types.h"
#include "../GPIO/hal_gpio.h"

/*Section Macro declaration*/
#define TIMER3_ON                                           0x01
#define TIMER3_OFF                                          0x00

#define TIMER3_COUNTER_MODE                                 0x00
#define TIMER3_TIMER_MODE                                   0x01

#define TIMER3_COUNTER_SYNC                                 0x00
#define TIMER3_COUNTER_ASYNC                                0x01

#define TIMER3_RW_REG_8BIT_MODE                             0x00
#define TIMER3_RW_REG_16BIT_MODE                            0x01


/*Section Macro function declaration*/

#define TIMER3_ENABLE()                                     T3CONbits.TMR3ON = 1
#define TIMER3_DISABLE()                                    T3CONbits.TMR3ON = 0

#define TIMER3_COUNTER_MODE_ENABLE()                        T3CONbits.TMR3CS = 1
#define TIMER3_TIMER_MODE_ENABLE()                          T3CONbits.TMR3CS = 0

#define TIMER3_COUNTER_SYNC_ENABLE()                        T3CONbits.T3SYNC = 1
#define TIMER3_COUNTER_ASYNC_ENABLE()                       T3CONbits.T3SYNC = 0

#define TIMER3_PRESCALER_VAL(_VAL)                          T3CONbits.T3CKPS = _VAL

#define TIMER3_RW_REG_8BIT_MODE_ENABLE()                    T3CONbits.RD16 = 0
#define TIMER3_RW_REG_16BIT_MODE_ENABLE()                   T3CONbits.RD16 = 1

/*Section Data type declaration*/

typedef enum{
    TIMER3_PRESCALER_DIV_BY_1 = 0,
    TIMER3_PRESCALER_DIV_BY_2,
    TIMER3_PRESCALER_DIV_BY_4,
    TIMER3_PRESCALER_DIV_BY_8
}TIMER3_Prescaler_val;

typedef struct{
#if TIMER3_INTERRUPT_CONFG == INTERRUPT_ENABLE
    Interrupt_Handler Timer3_interrupt_handler;
#if PIRIORITY_ENABLE == PIRIORITY_ON
    uint8 priority            :1;
#endif
#endif
    uint16 preloaded_val;
    uint8 prescaler_val       :3;
    uint8 timer_mode          :1;
    uint8 sync_counter_mode   :1;
    uint8 RW_REG_mode         :1;


}Timer3_cnfg_t;

/*Section fun prototype*/
Std_ReturnType mcal_Timer3_initialize(const Timer3_cnfg_t* _timer3);
Std_ReturnType mcal_Timer3_deinitialize(const Timer3_cnfg_t* _timer3);
Std_ReturnType mcal_Timer3_read_val(const Timer3_cnfg_t* _timer3, uint16* val);
Std_ReturnType mcal_Timer3_write_val(const Timer3_cnfg_t* _timer3, uint16 val);


#endif	/* MCAL_TIMER3_H */

