/* 
 * File:   mcal_timer2.c
 * Author: Honey
 *
 * Created on October 5, 2023, 2:41 PM
 */
#include "mcal_timer2.h"
static volatile Interrupt_Handler timer2_interrupr_handler;
static volatile uint8 preloaded_val ;
Std_ReturnType mcal_Timer2_initialize(const Timer2_cnfg_t* timer2)
{
    Std_ReturnType ret = E_NOT_OK;
    if(timer2 == NULL)
    {
         ret = E_NOT_OK;
    
    }
    else
    {
        TIMER2_DISABLE();
        TMR2 = timer2->timer2_preloaded_val;
        preloaded_val = timer2->timer2_preloaded_val;
        TIMER2_POSTSCALER_CNFG(timer2->postscaler_val);
        TIMER2_PRESCALER_CNFG(timer2->prescaler_val);
#if TIMER2_INTERRUPT_CONFG == INTERRUPT_ENABLE
#if PIRIORITY_ENABLE == PIRIORITY_ON
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_HIGH_PRIORITY(); 
    ENABLE_ALL_LOW_PRIORITY();
    switch(timer2->priority)
         {
                 case LOW_PRIORITY:
                     TIMER2_SET_LOW_PRIORITY();
                     break;
                 case HIGH_PRIORITY:
                     TIMER2_SET_HIGH_PRIORITY();
                     break;
         }
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    TIMER2_CLEAR_INTERRUPT_FLAG();
    TIMER2_INTERRUPT_ENABLE();
   timer2_interrupr_handler = timer2->timer2_interrupt_handler;
#endif
        TIMER2_ENABLE();
    
    
    }
    return ret;

}
Std_ReturnType mcal_Timer2_deinitialize(const Timer2_cnfg_t* timer2)
{
    Std_ReturnType ret = E_NOT_OK;
    if(timer2 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        TIMER2_DISABLE();
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
        TIMER2_INTERRUPT_DISABLE();
        
#endif
    }
    return ret;
    

}
Std_ReturnType mcal_Timer2_write_val(const Timer2_cnfg_t* timer2,uint8 val)
{
    Std_ReturnType ret = E_NOT_OK;
    if(timer2 == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
        TMR2 = val;
        ret = E_OK;
    }
    return ret;

}
Std_ReturnType mcal_Timer2_read_val(const Timer2_cnfg_t* timer2,uint8 *val)
{  
    Std_ReturnType ret = E_NOT_OK;
    if(timer2 == NULL || val == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
        *val = TMR2;
        ret = E_OK;
    }
    return ret;

}
void TIMER2_ISR(void)
{
    TIMER2_CLEAR_INTERRUPT_FLAG();
    TMR2 = preloaded_val;
    if(timer2_interrupr_handler)
        timer2_interrupr_handler();
    
}
