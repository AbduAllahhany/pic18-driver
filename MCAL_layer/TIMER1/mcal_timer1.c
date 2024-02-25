/* 
 * File:   mcal_timer1.c
 * Author: Honey
 *
 * Created on October 4, 2023, 6:17 PM
 */
#include "mcal_timer1.h"

static Std_ReturnType Timer1_select_mode(const Timer1_cnfg_t* _timer1);
static Std_ReturnType Timer1_write_to_reg(uint16 val);
static Std_ReturnType OSC_CNFG(uint8 val);
static Std_ReturnType SYNC_CNFG(uint8 val);
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
static volatile Interrupt_Handler _timer1_interrupr_handler = NULL;
#endif

static volatile uint16 preloaded_val;


Std_ReturnType mcal_Timer1_initialize(const Timer1_cnfg_t* _timer1) 
{
    Std_ReturnType ret = E_NOT_OK;
    if(_timer1 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        /*disable timer1*/
        TIMER1_DISABLE();
        /*select mode*/
        Timer1_select_mode(_timer1);
        /*select pre-scaler configuration*/
        TIMER1_PRESCALER_VAL(_timer1->prescaler_val);
        /*write pre-loaded value*/
        Timer1_write_to_reg(_timer1->preloaded_val);
        preloaded_val = _timer1->preloaded_val;
        /*interrupt configuration*/
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
#if PIRIORITY_ENABLE == PIRIORITY_ON
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_HIGH_PRIORITY(); 
    ENABLE_ALL_LOW_PRIORITY();
    switch(_timer1->_priority)
         {
                 case LOW_PRIORITY:
                     TIMER1_SET_LOW_PRIORITY();
                     break;
                 case HIGH_PRIORITY:
                     TIMER1_SET_HIGH_PRIORITY();
                     break;
         }
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    TIMER1_CLEAR_INTERRUPT_FLAG();
    TIMER1_INTERRUPT_ENABLE();
   _timer1_interrupr_handler = _timer1->timer1_handler;
#endif
        /*enable timer1*/
        TIMER1_ENABLE();
    }
    return ret;


}
Std_ReturnType mcal_Timer1_deinitialize(const Timer1_cnfg_t* _timer1)
{
     Std_ReturnType ret = E_NOT_OK;
    if(_timer1 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        TIMER1_DISABLE();
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
        TIMER1_INTERRUPT_DISABLE();
        
#endif
    }
    return ret;
    

}
Std_ReturnType mcal_Timer1_read_val(const Timer1_cnfg_t* _timer1, uint16* val)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr1l = 0,l_tmr1h = 0;
    if(_timer1 == NULL || val == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        l_tmr1l = TMR1L;
        l_tmr1h =  TMR1H ;
        *val = (uint16)(l_tmr1l + (l_tmr1h << 8));
        ret = E_OK; 
    }
    return ret;




}
Std_ReturnType mcal_Timer1_write_val(const Timer1_cnfg_t* _timer1, uint16 val)
{
      Std_ReturnType ret = E_NOT_OK;
    if(_timer1 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {       
        Timer1_write_to_reg(val);
    }
      return ret;


}


static Std_ReturnType Timer1_select_mode(const Timer1_cnfg_t* _timer1)
{
     Std_ReturnType ret = E_NOT_OK;
    if(_timer1 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        if(_timer1->timer_mode == TIMER1_COUNTER_MODE)
        {
            /*OSC configuration*/
            OSC_CNFG(_timer1->OSC_status);
            /*SYNC configuration*/
            SYNC_CNFG(_timer1->sync_counter_mode);
            /*enable counter*/
            TIMER1_COUNTER_MODE_ENABLE();   
        }
        else if(_timer1->timer_mode == TIMER1_TIMER_MODE)
        {
            TIMER1_TIMER_MODE_ENABLE();
        }
    }
     return ret;
}
static Std_ReturnType Timer1_write_to_reg(uint16 val)
{
    Std_ReturnType ret = E_OK;
     TMR1H =  (uint8)(val >> 8);
     TMR1L =  (uint8)val;
    ret = E_OK; 
    return ret;
    
}

static Std_ReturnType OSC_CNFG(uint8 val)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (val)
    {
        case TIMER1_OSC_ON:
            TIMER1_OSC_ENABLE();
            ret = E_OK;
            break;
        case TIMER1_OSC_OFF:
            TIMER1_OSC_DISABLE();
            ret = E_OK;
            break;   
    }
    return ret;
}



static Std_ReturnType SYNC_CNFG(uint8 val)
{
    Std_ReturnType ret = E_NOT_OK;
    switch (val)
    {
        case TIMER_COUNTER_SYNC:
            TIMER_COUNTER_SYNC_ENABLE();
            ret = E_OK;
            break;
        case TIMER_COUNTER_ASYNC:
            TIMER_COUNTER_ASYNC_ENABLE();
            ret = E_OK;
            break;   
    }
    return ret;
}

void TIMER1_ISR(void)
{
    Timer1_write_to_reg(preloaded_val);
    TIMER1_CLEAR_INTERRUPT_FLAG();
    if(_timer1_interrupr_handler)
        _timer1_interrupr_handler();
}