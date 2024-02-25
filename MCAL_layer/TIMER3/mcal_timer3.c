/* 
 * File:   mcal_timer3.c
 * Author: Honey
 *
 * Created on October 10, 2023, 2:51 PM
 */
#include "mcal_timer3.h"

static Std_ReturnType Timer3_select_mode(const Timer3_cnfg_t* _timer3);
static Std_ReturnType Timer3_write_to_reg(uint16 val);
static Std_ReturnType SYNC_CNFG(uint8 val);

static volatile Interrupt_Handler _timer3_interrupr_handler;
uint16 preloaded_val;
Std_ReturnType mcal_Timer3_initialize(const Timer3_cnfg_t* _timer3)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_timer3 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        /*Disable timer*/
        TIMER3_DISABLE();
        /*Select timer mode*/
        Timer3_select_mode(_timer3);
        /*select pre-scaler value*/
        TIMER3_PRESCALER_VAL(_timer3->prescaler_val);
        /*write pre-loaded value*/
        Timer3_write_to_reg(_timer3->preloaded_val);
        preloaded_val = _timer3->preloaded_val;
        /*interrupt configuration*/
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
#if PIRIORITY_ENABLE == PIRIORITY_ON
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_HIGH_PRIORITY(); 
    ENABLE_ALL_LOW_PRIORITY();
    switch(_timer3->priority)
         {
                 case LOW_PRIORITY:
                     TIMER3_SET_LOW_PRIORITY();
                     break;
                 case HIGH_PRIORITY:
                     TIMER3_SET_HIGH_PRIORITY();
                     break;
         }
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    TIMER3_CLEAR_INTERRUPT_FLAG();
    TIMER3_INTERRUPT_ENABLE();
   _timer3_interrupr_handler = _timer3->Timer3_interrupt_handler;
#endif
        /*enable timer */
        TIMER3_ENABLE();
    
    }
    return ret;
}
Std_ReturnType mcal_Timer3_deinitialize(const Timer3_cnfg_t* _timer3)
{
     Std_ReturnType ret = E_NOT_OK;
    if(_timer3 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        TIMER3_DISABLE();
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
        TIMER3_INTERRUPT_DISABLE();
        
#endif
    }
    return ret;


}

Std_ReturnType mcal_Timer3_read_val(const Timer3_cnfg_t* _timer3, uint16* val)
{
   Std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr3l = 0,l_tmr3h = 0;
    if(_timer3 == NULL || val == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        l_tmr3l = TMR3L;
        l_tmr3h =  TMR3H ;
        *val = (uint16)(l_tmr3l + (l_tmr3h << 8));
        ret = E_OK; 
    }
    return ret;


}
Std_ReturnType mcal_Timer3_write_val(const Timer3_cnfg_t* _timer3, uint16 val)
{
      Std_ReturnType ret = E_NOT_OK;
    if(_timer3 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {       
        Timer3_write_to_reg(val);
    }
      return ret;


}


static Std_ReturnType Timer3_select_mode(const Timer3_cnfg_t* _timer3)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_timer3 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        switch(_timer3->timer_mode)
        {
            case TIMER3_COUNTER_MODE:
                TIMER3_COUNTER_MODE_ENABLE();
                SYNC_CNFG(_timer3->sync_counter_mode);
                break;
            case TIMER3_TIMER_MODE:
                TIMER3_TIMER_MODE_ENABLE();
                break;
            default: ret = E_NOT_OK; 
            
        }
        ret |= E_OK;   
    }
    return ret;
    


}
static Std_ReturnType Timer3_write_to_reg(uint16 val)
{
    Std_ReturnType ret = E_OK;
     TMR3H =  (uint8)(val >> 8);
     TMR3L =  (uint8)val;
     ret = E_OK; 
    return ret;
}
static Std_ReturnType SYNC_CNFG(uint8 val)
{
    Std_ReturnType ret = E_NOT_OK;
   switch (val)
   {
       case TIMER3_COUNTER_SYNC:
           TIMER3_COUNTER_SYNC_ENABLE(); 
           break;
       case TIMER3_COUNTER_ASYNC:
           TIMER3_COUNTER_ASYNC_ENABLE();
           break;
       default : ret = E_NOT_OK;
   }
   ret |= E_OK;
   return ret;
}


void TIMER3_ISR(void)
{
    TIMER3_CLEAR_INTERRUPT_FLAG();
    if(_timer3_interrupr_handler)
    {
        _timer3_interrupr_handler();
    }
}