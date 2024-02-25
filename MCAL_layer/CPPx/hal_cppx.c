/* 
 * File:   hal_ccpx.c
 * Author: Honey
 *
 * Created on October 15, 2023, 8:43 PM
 */
#include "hal_ccpx.h"

#if CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE 
    static Std_ReturnType CCP1_interrupt_cnfg(const CCPx_t* ccpx);
    static Interrupt_Handler CCP1_Interrupt_handler;

#if PIRIORITY_ENABLE == PIRIORITY_ON 
      static Std_ReturnType CCP1_Set_priority(const CCPx_t* ccpx);
#endif
     
#endif
      
#if CCP2_INTERRUPT_CONFG == INTERRUPT_ENABLE 
      static Std_ReturnType CCP2_interrupt_cnfg(const CCPx_t* ccpx);
      static Interrupt_Handler CCP2_Interrupt_handler;

#if PIRIORITY_ENABLE == PIRIORITY_ON 
      static Std_ReturnType CCP2_Set_priority(const CCPx_t* ccpx);
#endif
      
#endif
      
#if CCP2_INTERRUPT_CONFG == INTERRUPT_ENABLE || CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE
   static Std_ReturnType CCPx_interrupt_cnfg(const CCPx_t* ccpx);
#endif
#if ((CCP1_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED) || (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED) )
    static Std_ReturnType PWM_SET_FREQ(const CCPx_t* ccpx);
    static Std_ReturnType PWM_CNFG(const CCPx_t* ccpx);

#endif
#if ((CCP1_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED) || (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED))
    static Std_ReturnType Capture_mode_select_cnfg(const CCPx_t* ccpx);
    static Std_ReturnType CCP1_select_capture_cnfg(const CCPx_t* ccpx);
    static Std_ReturnType CCP2_select_capture_cnfg(const CCPx_t* ccpx);
#endif

 static Std_ReturnType CCPx_Disable_module(const CCPx_t* ccpx);
 static Std_ReturnType CCPx_Timer_base_select(const CCPx_t* ccpx);
 
 
    
Std_ReturnType hal_CCPx_initialize(const CCPx_t* ccpx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {   
        /*Disable module*/
        ret |= CCPx_Disable_module(ccpx);  
#if ((CCP1_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED) || (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED) )
       PWM_CNFG(ccpx);        
#endif  
       
#if ((CCP1_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED) || (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED))
      Capture_mode_select_cnfg(ccpx);
#endif
        /*interrupt configuration*/
        CCPx_interrupt_cnfg(ccpx);
        /*Set pin configuration*/
        ret = gpio_pin_direction_intialize(&(ccpx->_pin));
    }
    return ret;
}

/*---------------------------------------------------------Capture Mode---------------------------------------------------------------------------*/
#if ((CCP1_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED) || (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED) )
static Std_ReturnType Capture_mode_select_cnfg(const CCPx_t* ccpx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
      switch (ccpx->inst)
      {
            case ccp1_inst:
                CCP1_select_capture_cnfg(ccpx);
                break;
            case ccp2_inst:
                CCP2_select_capture_cnfg(ccpx);
                break;
            default : ret|= E_NOT_OK;
      }
      ret |= CCPx_Timer_base_select(ccpx);
    }
    return ret;

}

 static Std_ReturnType CCP1_select_capture_cnfg(const CCPx_t* ccpx)
 {
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
               switch(ccpx->varient)
       {
           case CCPx_CAPTURE_MODE_EVERY_FALLING_EDGE:
               CCP1_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_FALLING_EDGE); 
               ret |= E_OK;
               break;
           case CCPx_CAPTURE_MODE_EVERY_RISING_EDGE:
               CCP1_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_RISING_EDGE);   
               ret |= E_OK;
               break;
           case CCPx_CAPTURE_MODE_EVERY_4RISING_EDGE:
               CCP1_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_4RISING_EDGE);
               ret |= E_OK;
               break; 
           case CCPx_CAPTURE_MODE_EVERY_16RISING_EDGE:
               CCP1_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_16RISING_EDGE); 
               ret |= E_OK;
               break;
           default: ret |= E_NOT_OK; 
       }
    }
    return ret;
 
 }
 static Std_ReturnType CCP2_select_capture_cnfg(const CCPx_t* ccpx)
 {
     Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK; 
    
    }
    else
    {
        switch(ccpx->varient)
       {
           case CCPx_CAPTURE_MODE_EVERY_FALLING_EDGE:
               CCP2_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_FALLING_EDGE); 
               ret |= E_OK;
               break;
           case CCPx_CAPTURE_MODE_EVERY_RISING_EDGE:
               CCP2_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_RISING_EDGE);   
               ret |= E_OK;
               break;
           case CCPx_CAPTURE_MODE_EVERY_4RISING_EDGE:
               CCP2_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_4RISING_EDGE);
               ret |= E_OK;
               break; 
           case CCPx_CAPTURE_MODE_EVERY_16RISING_EDGE:
               CCP2_SELECT_MODE(CCPx_CAPTURE_MODE_EVERY_16RISING_EDGE); 
               ret |= E_OK;
               break;
           default: ret |= E_NOT_OK; 
       }
    }
    return ret;
 
 
 }



#endif
#if (CCP1_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED)
 Std_ReturnType hal_CCP1_Read_capture_val(const CCPx_t* ccpx,uint16* val)
 {
     Std_ReturnType ret = E_NOT_OK;
     CCP_REG_T ccp1_obj;
     if(ccpx == NULL)
     {
         ret = E_NOT_OK;
     }
     else
     {
         ccp1_obj.ccpr_low = CCPR1L;
         ccp1_obj.ccpr_high = CCPR1H;
         *val = ccp1_obj.ccpr_16Bit;
         ret = E_OK;
     }
     return ret;

 }
 Std_ReturnType hal_CCP1_Iscapture_Ready(const CCPx_t* ccpx,uint8 *status)
 {
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;  
    }
    else
    {
        if(PIR2bits.CCP2IF == 1)
        {
            CCP2_CLEAR_INTERRUPT_FLAG();
            *status = 1;
        }
        else
        {
             *status = 0;
        }
    }
    return ret;
 }
 
   
#endif
 
#if (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED)
 
Std_ReturnType hal_CCP2_Read_capture_val(const CCPx_t* ccpx,uint16* val)
 {
     Std_ReturnType ret = E_NOT_OK;
     CCP_REG_T ccp2_obj;
     if(ccpx == NULL)
     {
         ret = E_NOT_OK;
     }
     else
     {
         ccp2_obj.ccpr_low = CCPR2L;
         ccp2_obj.ccpr_high = CCPR2H;
         *val = ccp2_obj.ccpr_16Bit;
         ret = E_OK;
     }
     return ret;

 }
Std_ReturnType hal_CCP2_Iscapture_Ready(const CCPx_t* ccpx, uint8* status)
{
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;  
    }
    else
    {
        if(PIR2bits.CCP2IF == 1)
        {
            CCP2_CLEAR_INTERRUPT_FLAG();
            *status = 1;
        }
        else
        {
             *status = 0;
        }
    }
    return ret;
}
 
#endif
/*-----------------------------------------------------------PWM Mode----------------------------------------------------------------------------*/
#if ((CCP1_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED) || (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED) )
 static Std_ReturnType PWM_CNFG(const CCPx_t* ccpx)
 {
     Std_ReturnType ret = E_NOT_OK;
     if(ccpx == NULL)
     {
         ret = E_NOT_OK;
     }
     else
     {
        /*Set Frequency */
        PWM_SET_FREQ(ccpx);
         
     }
     return ret;
 
 }
Std_ReturnType hal_CCPx_start_PWM(const CCPx_t* ccpx)
{
    Std_ReturnType ret = E_NOT_OK;;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        if(ccpx->inst == ccp1_inst)
        {
            CCP1_SELECT_MODE(CCPx_PWM_MODE);   
            ret = E_OK;
        }
        else if(ccpx->inst== ccp2_inst)
        {
           CCP1_SELECT_MODE(CCPx_PWM_MODE);     
           ret = E_OK; 
        }
    }   
    return ret;



}
Std_ReturnType hal_CCPx_set_duty_cyle(const CCPx_t* ccpx, uint8 val)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 duty_val;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;
    }
    else 
    {
        uint8 period =(uint8)( _XTAL_FREQ / ((float)((4.0*ccpx->freq)*ccpx->prescaler)));
        duty_val = (uint16)(period *(val/100.0)); 
        if(ccpx->inst == ccp1_inst)
        {
            CCP1CONbits.DC1B = duty_val & 0x03;
            CCPR1L = duty_val;
            ret = E_OK;
        }
        else if(ccpx->inst== ccp2_inst)
        {
            CCP2CONbits.DC2B = duty_val & 0x03;
            CCPR2L = duty_val >> 2;
           ret = E_OK; 
        }
    }   
    return ret;
}
Std_ReturnType hal_CCPx_stop_PWM(const CCPx_t* ccpx)
{
    Std_ReturnType ret = E_NOT_OK;;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;
    }
    else 
    {
       ret = CCPx_Disable_module(ccpx);
    }
    return ret;

}
static Std_ReturnType PWM_SET_FREQ(const CCPx_t* ccpx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;
    }
    else{
        PR2 = (uint8) ( _XTAL_FREQ / ((float)((4.0*ccpx->freq)*ccpx->prescaler)) -1 );
    }
    return ret;
}
#endif



static Std_ReturnType CCPx_Timer_base_select(const CCPx_t* ccpx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(ccpx == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch(ccpx->timer_base)
        {
            case CCPx_Timer1_base_for_CCP1_CCP2:
                T3CONbits.T3CCP1 = 0;
                T3CONbits.T3CCP2 = 0;
                ret = E_OK;
                break;
            case CCPx_Timer3_base_for_CCP1_CCP2:
                T3CONbits.T3CCP1 = 0;
                T3CONbits.T3CCP2 = 1;
                ret = E_OK;
                break;
            case CCPx_Timer1_base_for_CCP1_Timer3_for_CCP2:
                T3CONbits.T3CCP1 = 1;
                T3CONbits.T3CCP2 = 0;
                ret = E_OK;
                break;
            default : ret |= E_NOT_OK;
        }
    }
    return ret;
}
 static Std_ReturnType CCPx_Disable_module(const CCPx_t* ccpx)
 {
      Std_ReturnType ret = E_NOT_OK;
      if(ccpx == NULL)
      {
          ret = E_NOT_OK;
      
      }
      else{
      if(ccpx->inst == ccp1_inst)
        {
            CCP1_SELECT_MODE(CCPx_DISABLE_MODULE);   
            ret = E_OK;
        }
        else if(ccpx->inst== ccp2_inst)
        {
           CCP1_SELECT_MODE(CCPx_DISABLE_MODULE);     
           ret = E_OK; 
        }
      }
      return ret;
   }
 
 /*---------------------------------------------------------interrupt configuration---------------------------------------------------------*/
#if CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE 
    static Std_ReturnType CCP1_interrupt_cnfg(const CCPx_t* ccpx)
    {
        Std_ReturnType ret = E_NOT_OK;
          /*Disable interrupt*/
          switch(ccpx->inst)
          {
              case ccp1_inst:
                  CCP1_INTERRUPT_DISABLE();
                  ret |= E_OK; 
                  break;
              default: ret|= E_NOT_OK; 
          }
#if PIRIORITY_ENABLE == PIRIORITY_ON 
         ENABLE_PRIORITY_BIT();
         ENABLE_ALL_HIGH_PRIORITY();
         ENABLE_ALL_LOW_PRIORITY();
          switch(ccpx->inst)
          {
              case ccp1_inst:
                  CCP1_Set_priority(ccpx);
                   ret |= E_OK; 
                  break;
              default: ret|= E_NOT_OK; 
          }
#else
        /*Disable Global interrupt*/
         ENABLE_GLOBAL_INTERRUPT_BIT();
         ENABLE_PERIPHERAL_INTERRUPT_BIT();
          
#endif
          CCP1_Interrupt_handler = ccpx->CCPx_interrupt_handler ;
          switch(ccpx->inst)
          {
              case ccp1_inst:
                  CCP1_INTERRUPT_ENABLE();
                  ret |= E_OK;
                  break;
              default: ret|= E_NOT_OK; 
          }
          return ret;
    }

#if PIRIORITY_ENABLE == PIRIORITY_ON 
    static Std_ReturnType CCP1_Set_priority(const CCPx_t* ccpx)
      {
          Std_ReturnType ret = E_NOT_OK;
          switch (ccpx->priority)
          {
                  case HIGH_PRIORITY:
                      CCP1_SET_HIGH_PRIORITY();
                      ret |= E_OK;
                      break;
                  case LOW_PRIORITY:          
                      CCP1_SET_LOW_PRIORITY();
                      ret |= E_OK;
                      break;
                      default : ret |= E_NOT_OK;
          }
          return ret;
      }
#endif
     
#endif
    
#if CCP2_INTERRUPT_CONFG == INTERRUPT_ENABLE 
    static Std_ReturnType CCP2_interrupt_cnfg(const CCPx_t* ccpx)
    {
        Std_ReturnType ret = E_NOT_OK;
          /*Disable interrupt*/
          switch(ccpx->inst)
          {
              case ccp2_inst:
                  CCP2_INTERRUPT_DISABLE();
                  ret |= E_OK; 
                  break;
              default: ret|= E_NOT_OK; 
          }
#if PIRIORITY_ENABLE == PIRIORITY_ON 
         ENABLE_PRIORITY_BIT();
         ENABLE_ALL_HIGH_PRIORITY();
         ENABLE_ALL_LOW_PRIORITY();
          switch(ccpx->inst)
          {
              case ccp2_inst:
                  CCP2_Set_priority(ccpx);
                   ret |= E_OK; 
                  break;
              default: ret|= E_NOT_OK; 
          }
#else
        /*Disable Global interrupt*/
         DISABLE_GLOBAL_INTERRUPT_BIT();
         ENABLE_PERIPHERAL_INTERRUPT_BIT();
          
#endif
         CCP2_Interrupt_handler = ccpx->CCPx_interrupt_handler ;
          switch(ccpx->inst)
          {
              case ccp1_inst:
                  CCP1_INTERRUPT_ENABLE();
                  ret |= E_OK;
                  break;
              default: ret|= E_NOT_OK; 
          }
          return ret;
    }
#if PIRIORITY_ENABLE == PIRIORITY_ON 
      static Std_ReturnType CCP2_Set_priority(const CCPx_t* ccpx)
      {
                    Std_ReturnType ret = E_NOT_OK;
          switch (ccpx->priority)
          {
                  case HIGH_PRIORITY:
                      CCP2_SET_HIGH_PRIORITY();
                      ret |= E_OK;
                      break;
                  case LOW_PRIORITY:          
                      CCP2_SET_LOW_PRIORITY();
                      ret |= E_OK;
                      break;
                      default : ret |= E_NOT_OK;
          }
          return ret;
      
      
      }
#endif     
#endif
      
#if CCP2_INTERRUPT_CONFG == INTERRUPT_ENABLE || CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE
   static Std_ReturnType CCPx_interrupt_cnfg(const CCPx_t* ccpx)
   {
       Std_ReturnType ret = E_NOT_OK;
       if(ccpx == NULL )
       {
            ret |= E_NOT_OK;
       }
       else
       {
       if(ccpx->inst == ccp1_inst){
#if CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE
           ret |= CCP1_interrupt_cnfg(ccpx);
#endif
      }
      else if(ccpx->inst == ccp2_inst){
#if CCP2_INTERRUPT_CONFG == INTERRUPT_ENABLE
           ret |= CCP2_interrupt_cnfg(ccpx);
#endif
      }
      }
       return ret;
   }
#endif
   
 void CCP1_ISR(void)
 {
     CCP1_CLEAR_INTERRUPT_FLAG();
     if(CCP1_Interrupt_handler)
         CCP1_Interrupt_handler();
 
 }
 void CCP2_ISR(void)
 {
       CCP2_CLEAR_INTERRUPT_FLAG();
     if(CCP2_Interrupt_handler)
         CCP2_Interrupt_handler();
 
 
 }
 