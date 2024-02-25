/* 
 * File:   mcal_timer0.c
 * Author: Honey
 *
 * Created on October 1, 2023, 5:55 PM
 */

#include "mcal_timer0.h" 
static inline Std_ReturnType timer0_set_size(const timer0_cnfg_t* timer0);
static inline Std_ReturnType timer0_select_mode(const timer0_cnfg_t* timer0);
static inline Std_ReturnType timer0_clock_src_edge(const timer0_cnfg_t* timer0);
static inline Std_ReturnType timer0_prescaler_cnfg(const timer0_cnfg_t* timer0);
static inline Std_ReturnType write_val_TMR0L_TMR0H(uint16 val);

Interrupt_Handler timer0_interrupt_handler;
static volatile uint16 _preloaded_val;


Std_ReturnType mcal_timer0_intialize(const timer0_cnfg_t* timer0)
{
    Std_ReturnType ret = E_NOT_OK;
   if(timer0 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        /* Disable timer0 */
         TIMER0_DISABLE();
        /* set timer0 register size */
          timer0_set_size(timer0);
        /* select clock source */
          timer0_select_mode(timer0);
        /* configure pre-scaler */
          timer0_prescaler_cnfg(timer0);
        /*pre-loaded value*/
          ret = write_val_TMR0L_TMR0H(timer0->preloadedval);
          _preloaded_val = timer0->preloadedval;
        /* timer0 interrupt */
#if TIMER0_INTERRUPT_CONFG == INTERRUPT_ENABLE
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_HIGH_PRIORITY(); 
    ENABLE_ALL_LOW_PRIORITY();
    switch(timer0->_priority)
         {
                 case LOW_PRIORITY:
                     TIMER0_SET_LOW_PRIORITY();
                     break;
                 case HIGH_PRIORITY:
                     TIMER0_SET_HIGH_PRIORITY();
                     break;
         }
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
   TIMER0_CLEAR_INTERRUPT_FLAG();
   TIMER0_INTERRUPT_ENABLE();
   timer0_interrupt_handler = timer0->timer0_handler;
#endif
        /* enable timer0 */
         TIMER0_ENABLE();
    }
    
    return ret;
    

}
Std_ReturnType mcal_timer0_deinitialize(const timer0_cnfg_t* _timer0)
{
     Std_ReturnType ret = E_NOT_OK;
    if(_timer0 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        TIMER0_DISABLE();
#if TIMER1_INTERRUPT_CONFG == INTERRUPT_ENABLE
        TIMER0_INTERRUPT_DISABLE();
        
#endif
    }
    return ret;
    

}
Std_ReturnType mcal_timer0_read_register(const timer0_cnfg_t* timer0, uint16* val)
{
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr0l = 0,l_tmr0h = 0;
    if(timer0 == NULL || val == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
         
        l_tmr0l = TMR0L;
        l_tmr0h =  TMR0H ;
        *val = (uint16)(l_tmr0l + (l_tmr0h << 8));
        ret = E_OK; 
    }
    return ret;

}
Std_ReturnType mcal_timer0_write_value(const timer0_cnfg_t* timer0, uint16 val)
{
     Std_ReturnType ret = E_NOT_OK;
    if(timer0 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        ret = write_val_TMR0L_TMR0H(val);
    }
    return ret;

}

static inline Std_ReturnType timer0_set_size(const timer0_cnfg_t* timer0)
{
    Std_ReturnType ret = E_NOT_OK;
    if(timer0 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        switch (timer0->timer0_register_size)
        {
            case (TIMER0_8BIT_REGISTER):
                TIMER0_SET_8BIT_REG();
                break;
            case (TIMER0_16BIT_REGISTER):
                TIMER0_SET_16BIT_REG();
                break;
            default:
                ret = E_NOT_OK; 
        }
         ret = E_OK; 
    
    
    }
    return ret;
}
static inline Std_ReturnType timer0_select_mode(const timer0_cnfg_t* timer0)
{
    Std_ReturnType ret = E_NOT_OK;
    if(timer0 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        switch (timer0->timer0_select_mode)
        {
            case (TIMER0_COUNTER_MODE):
                TIMER0_COUNTER_MODE_ENABLE();
                timer0_clock_src_edge(timer0);
                break;
            case (TIMER0_TIMER_MODE):
                TIMER0_TIMER_MODE_ENABLE();
                break;
            default:
                ret = E_NOT_OK; 
        }
         ret = E_OK; 
    
    
    }
     return ret;
}
static inline Std_ReturnType timer0_clock_src_edge(const timer0_cnfg_t* timer0)
{
     Std_ReturnType ret = E_NOT_OK;
    if(timer0 == NULL)    
    {
        ret = E_NOT_OK; 
    }
    else
    {
        switch (timer0->timer0_edge_select)
        {
            case (TIMER0_RISING_EDGE):
                TIMER0_RISING_EDGE_INCREMENT();
                break;
            case (TIMER0_FALLING_EDGE):
                TIMER0_FALLING_EDGE_INCREMENT();
                break;
            default:
                ret = E_NOT_OK; 
        }
         ret = E_OK; 
    
    
    }
     return ret;
}
static inline Std_ReturnType timer0_prescaler_cnfg(const timer0_cnfg_t* timer0)
{
    Std_ReturnType ret = E_NOT_OK;
    if(timer0 == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        switch (timer0->timer0_prescaler_cnfg)
        {
            case (TIMER0_PRESCALER_OFF):
                TIMER0_PRESCALER_DISABLE();
                break;
            case (TIMER0_PRESCALER_ON):
                TIMER0_PRESCALER_ENABLE();
                TIMER0_PRESCALER_VAL(timer0->timer0_prescaler_val);
                break;
            default:
                ret = E_NOT_OK; 
        }
         ret = E_OK; 
    
    
    }
     return ret;
}


 
 static Std_ReturnType write_val_TMR0L_TMR0H(uint16 val)
 {
     Std_ReturnType ret = E_NOT_OK;
     TMR0H =  (uint8)(val >> 8);
     TMR0L =  (uint8)val;
     ret = E_OK; 
    
     return ret;
 
 }
 void TIMER0_ISR(void)
 {
     TIMER0_CLEAR_INTERRUPT_FLAG();
      write_val_TMR0L_TMR0H(_preloaded_val);
     if(timer0_interrupt_handler != NULL)
     { 
         timer0_interrupt_handler();
     }
 
 }






