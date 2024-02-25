/* 
 * File:   mcal_external_interrupt.c
 * Author: Honey
 *
 * Created on September 20, 2023, 5:29 PM
 */


#include "mcal_external_interrupt.h"


/*INTx interrupt helper functions*/
static Std_ReturnType INTx_disable_interrupt_bit(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_clear_interrupt_flag(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_enable_interrupt_bit(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_edge_interrupt_cnfg(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_setting_interrupt_priority(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_setting_interrupt_handler(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_falling_edge_intialize(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_rising_edge_intialize(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_low_priority(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INTx_high_priority(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INT0_interrupt_handler_initialize(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INT1_interrupt_handler_initialize(const INTx_Interrupt_config * _INTx );
static Std_ReturnType INT2_interrupt_handler_initialize(const INTx_Interrupt_config * _INTx );


static Std_ReturnType Setting_RBx_interrupt_handler(const RBx_Interrupt_config *_RBx);
static Std_ReturnType Setting_RBx_priority_bit(const RBx_Interrupt_config *_RBx);


static volatile Interrupt_Handler INT0_handler = NULL;
static volatile Interrupt_Handler INT1_handler = NULL;
static volatile Interrupt_Handler INT2_handler = NULL;


static volatile Interrupt_Handler RB4_high_handler = NULL;
static volatile Interrupt_Handler RB4_low_handler  = NULL;
static volatile Interrupt_Handler RB5_high_handler = NULL;
static volatile Interrupt_Handler RB5_low_handler  = NULL;
static volatile Interrupt_Handler RB6_high_handler = NULL;
static volatile Interrupt_Handler RB6_low_handler  = NULL;
static volatile Interrupt_Handler RB7_high_handler = NULL;
static volatile Interrupt_Handler RB7_low_handler  = NULL;
/*RBx interrupt helper functions*/ 



/* interrupts service routines*/
void INT0_ISR()
 {
    CLEAR_INT0_FLAG();
    if(INT0_handler)
    {
        INT0_handler();
    }
 }

void INT1_ISR()
 {
    CLEAR_INT1_FLAG();
    if(INT1_handler)
    {
        INT1_handler();
    }
 }

void INT2_ISR()
 {
    CLEAR_INT2_FLAG();
    if(INT2_handler)
    {
        INT2_handler();
    }
 }
void RB4_ISR(uint8 src)
{
    CLEAR_RBx_FLAG();
    if(src == 1 && (RB4_high_handler)) RB4_high_handler();
    else if(src == 0 && (RB4_high_handler)) RB4_low_handler();
}
void RB5_ISR(uint8 src)
{
    CLEAR_RBx_FLAG();
     if(src == 1 &&(RB5_high_handler)) RB5_high_handler();
    else if(src == 0 && (RB5_low_handler)) RB5_low_handler();
}
void RB6_ISR(uint8 src)
{
    CLEAR_RBx_FLAG();
     if(src == 1 &&(RB6_high_handler)) RB6_high_handler();
    else if(src == 0 && (RB6_low_handler)) RB6_low_handler();
}
void RB7_ISR(uint8 src)
{
    CLEAR_RBx_FLAG();
     if(src == 1 &&(RB7_high_handler)) RB7_high_handler();
    else if(src == 0 && (RB7_low_handler)) RB7_low_handler();
}


Std_ReturnType mcal_INTx_initialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK; 
    if(_INTx == NULL )
    {
        ret = E_NOT_OK;
    }
    else
    {   
#if PIRIORITY_ENABLE == PIRIORITY_ON
        /*PRIORITY MODE*/
           /*Enable priority bit*/
            ENABLE_PRIORITY_BIT(); 
           /*Enable high priority & low priority GIEH & GIEL*/
            ENABLE_ALL_HIGH_PRIORITY();
            ENABLE_ALL_LOW_PRIORITY();
           /*Setting the priority bit */
            ret |= INTx_setting_interrupt_priority(_INTx);
#else      
        /*COMATABLE MODE*/
           /*Disable priority bit*/
            DISABLE_PRIORITY_BIT();
           /*Enable Global interrupt GIE*/
            ENABLE_GLOBAL_INTERRUPT_BIT();
#endif  
        /*clear interrupt flag*/
        ret |= INTx_clear_interrupt_flag(_INTx);
        /*Determine edge configuration*/
        ret |= INTx_edge_interrupt_cnfg(_INTx);
        /*Initialize pin configuration*/
        ret |= gpio_pin_direction_intialize(&(_INTx->_pin));
        /*Setting interrupt handler*/
        ret |= INTx_setting_interrupt_handler(_INTx);
        /*Enable interrupt*/
        ret |= INTx_enable_interrupt_bit(_INTx);
    }
    return ret;

}
Std_ReturnType mcal_INTx_deinitialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK; 
    if(_INTx == NULL )
    {
        ret = E_NOT_OK;
    }
    else
    {   
        /*Clear flag*/
        ret |= INTx_clear_interrupt_flag(_INTx);
        /*Disable INTx enable bit*/
        ret |= INTx_disable_interrupt_bit(_INTx);
    }

    return ret;
    
}


static Std_ReturnType INTx_clear_interrupt_flag(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                CLEAR_INT0_FLAG();
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                CLEAR_INT1_FLAG();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                CLEAR_INT2_FLAG();
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    
    
    }
    return ret;



}
static Std_ReturnType INTx_enable_interrupt_bit(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
         switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                ENABLE_INT0_INTERRUPT();
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                ENABLE_INT1_INTERRUPT();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                ENABLE_INT2_INTERRUPT();
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;

}
static Std_ReturnType INTx_disable_interrupt_bit(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                DISABLE_INT0_INTERRUPT();
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                DISABLE_INT1_INTERRUPT();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                DISABLE_INT2_INTERRUPT();
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;

}

static Std_ReturnType INTx_edge_interrupt_cnfg(const INTx_Interrupt_config * _INTx )
{
     Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        switch(_INTx->interrupt_edge)
        {
            case  FALLING_EDGE:
                ret |= INTx_falling_edge_intialize(_INTx);
                break;
            case  RISING_EDGE:
                ret |= INTx_rising_edge_intialize(_INTx);
                break;
            default: 
                ret |= E_NOT_OK;    
        }
    }
    return ret;
}

static Std_ReturnType INTx_falling_edge_intialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
         switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                ENABLE_INT0_FALLING_EDGE();
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                ENABLE_INT1_FALLING_EDGE();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                ENABLE_INT2_FALLING_EDGE();
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;


}
static Std_ReturnType INTx_rising_edge_intialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
         switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                ENABLE_INT0_RSISING_EDGE();
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                ENABLE_INT1_RSISING_EDGE();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                ENABLE_INT2_RSISING_EDGE();  
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;
}

#if PIRIORITY_ENABLE == PIRIORITY_ON
static Std_ReturnType INTx_setting_interrupt_priority(const INTx_Interrupt_config *_INTx )
{
     Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        switch (_INTx->priority)
        {
            case LOW_PRIORITY:
                ret = INTx_low_priority(_INTx);
                break;
            case HIGH_PRIORITY:
               ret = INTx_high_priority(_INTx);
                break;
            default : 
                ret = E_NOT_OK;
                
        }
    }
    return ret;
}

static Std_ReturnType INTx_low_priority(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
         switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                /*INT0 interrupt always high priority*/
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                SET_INT1_LOW_PRIORITY();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                SET_INT2_LOW_PRIORITY();  
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;
}

static Std_ReturnType INTx_high_priority(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
         switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                /*INT0 interrupt always high priority*/
                ret = E_OK;
                break;
            case INT1_INTERRUPT:
                SET_INT1_HIGH_PRIORITY();
                ret = E_OK;
                break;
            case INT2_INTERRUPT:
                SET_INT2_HIGH_PRIORITY();  
                ret = E_OK;
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;
}
#endif

static Std_ReturnType INTx_setting_interrupt_handler(const INTx_Interrupt_config *_INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
         switch(_INTx->interrupt_src)
        {
            case INT0_INTERRUPT:
                ret |= INT0_interrupt_handler_initialize(_INTx);
                break;
            case INT1_INTERRUPT:
                ret |= INT1_interrupt_handler_initialize(_INTx);
                break;
            case INT2_INTERRUPT:
                ret |= INT2_interrupt_handler_initialize(_INTx);  
                break;
            default : 
                ret |= E_NOT_OK; 
        }
    }
    return ret;
}
static Std_ReturnType INT0_interrupt_handler_initialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx->_Interrupt_Handler == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        INT0_handler = _INTx->_Interrupt_Handler;
    }
    return ret;

}
static Std_ReturnType INT1_interrupt_handler_initialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx->_Interrupt_Handler == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        INT1_handler = _INTx->_Interrupt_Handler;
    }
    return ret;
}
static Std_ReturnType INT2_interrupt_handler_initialize(const INTx_Interrupt_config * _INTx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_INTx->_Interrupt_Handler == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        INT2_handler = _INTx->_Interrupt_Handler;
    }
    return ret;
}



Std_ReturnType mcal_RBx_initialize(const RBx_Interrupt_config * _RBx )
{
    Std_ReturnType ret = E_NOT_OK;
    if(_RBx == NULL)
    {
        ret = E_NOT_OK;
    
    }
    else
    {
        
#if PIRIORITY_ENABLE == PIRIORITY_ON
        /*Enable priority bit IPEN*/
         ENABLE_PRIORITY_BIT(); 
        /*Enable high & low priority bits* GIEL & GIEH*/
         ENABLE_ALL_HIGH_PRIORITY();
         ENABLE_ALL_LOW_PRIORITY();
        /*Set priority bit */
         ret |= Setting_RBx_priority_bit(_RBx);
#else
        /*Disable priority bit IPEN */
         DISABLE_PRIORITY_BIT();
        /*Enable global interrupt bit GIE*/
         ENABLE_GLOBAL_INTERRUPT_BIT();
         ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif    
         /*Clear flag*/
         CLEAR_RBx_FLAG();
        /*Initialize pin*/
         ret = gpio_pin_direction_intialize(&(_RBx->_pin));
        /*Set handler*/
         ret |= Setting_RBx_interrupt_handler(_RBx);
        /*Enable interrupt bit*/
         ENABLE_RBx_INTERRUPT();
    }
    
    return ret;


}
Std_ReturnType mcal_RBx_deinitialize(const RBx_Interrupt_config * _RBx )
{
    Std_ReturnType ret = E_NOT_OK; 
    if(_RBx == NULL )
    {
        ret = E_NOT_OK;
    }
    else
    {   
        /*Clear flag*/
        CLEAR_RBx_FLAG();
        /*Disable INTx enable bit*/
        DISABLE_RBx_INTERRUPT();
        ret = E_OK;
    }

    return ret;

}
static Std_ReturnType Setting_RBx_interrupt_handler(const RBx_Interrupt_config *_RBx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_RBx == NULL)
    {
        ret = E_NOT_OK;
    
    }
    else
    {
        switch(_RBx->_pin.pin_index)
        {
            case PIN4:
                RB4_high_handler = _RBx->High_change_Interrupt_Handler;
                RB4_low_handler  = _RBx->Low_change_Interrupt_Handler;
                break;
            case PIN5:
                RB5_high_handler = _RBx->High_change_Interrupt_Handler;
                RB5_low_handler  = _RBx->Low_change_Interrupt_Handler;
                break;
            case PIN6:
                RB6_high_handler = _RBx->High_change_Interrupt_Handler;
                RB6_low_handler  = _RBx->Low_change_Interrupt_Handler;
                break;
            case PIN7:
                RB7_high_handler = _RBx->High_change_Interrupt_Handler;
                RB7_low_handler  = _RBx->Low_change_Interrupt_Handler;
                break;
        }
    }
    return ret;



}

#if PIRIORITY_ENABLE == PIRIORITY_ON
static Std_ReturnType Setting_RBx_priority_bit(const RBx_Interrupt_config *_RBx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_RBx == NULL)
    {
        ret = E_NOT_OK;
    
    }
    else
    {
        switch(_RBx->priority)
        {
            case LOW_PRIORITY:
                SET_RBx_HIGH_PRIORITY();
                ret = E_OK;
                break;
            case HIGH_PRIORITY:
                SET_RBx_LOW_PRIORITY();
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
    }
    return ret;
    


}
#endif
