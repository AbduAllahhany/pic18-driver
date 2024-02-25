/* 
 * File:   mcal_eusart.c
 * Author: Honey
 *
 * Created on November 1, 2023, 4:53 PM
 */ 
#include"mcal_eusart.h"
static Std_ReturnType EUSART_Baud_rate_gen(const USART_t* usart_obj);
static Std_ReturnType EUSART_Select_Sync_Mode(const USART_t* usart_obj);

static Std_ReturnType EUSART_Tx_interrupt_config(const tx_cnfg_t *Tx_obj);
static Std_ReturnType EUSART_Tx_initalize(const tx_cnfg_t *Tx_obj);
static Std_ReturnType EUSART_Tx_nth_bit_config(const tx_cnfg_t *Tx_obj);
static Std_ReturnType EUSART_Tx_config(const tx_cnfg_t *Tx_obj);

static Std_ReturnType EUSART_Rx_initalize(const rx_cnfg_t *Rx_obj);
static Std_ReturnType EUSART_Rx_nth_bit_config(const rx_cnfg_t *Rx_obj);
static Std_ReturnType EUSART_Rx_config(const rx_cnfg_t *Rx_obj);
static Std_ReturnType EUSART_Rx_interrupt_config(const rx_cnfg_t *Rx_obj);
static Std_ReturnType EUSART_Restart_Reciever();

static Interrupt_Handler Transmit_interrupt_handler = NULL;
static Interrupt_Handler Recieve_interrupt_handler = NULL;
static Interrupt_Handler Overrun_interrupt_handler = NULL;
static Interrupt_Handler Frame_error_interrupt_handler = NULL;





static pin_config RC_6 = {
    .direction = INPUT_DIRECTION,
    .pin_index = PIN6,
    .port_index = PORTC_INDEX

}; 
static pin_config RC_7 = {
    .direction = INPUT_DIRECTION,
    .pin_index = PIN7,
    .port_index = PORTC_INDEX

}; 

Std_ReturnType mcal_EUSART_initialize(const USART_t *usart_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    if(usart_obj == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        
      /*disable serial port*/
       EUSART_SERIAL_PORT_DISABLE();
       /*initialize pins*/
       ret |= gpio_pin_direction_intialize(&RC_6);
       ret |= gpio_pin_direction_intialize(&RC_7);
      /*Baud rate initialize*/ 
       ret |= EUSART_Baud_rate_gen(usart_obj);
       switch (usart_obj->sync_mode)
       {
            EUSART_Select_Sync_Mode(usart_obj);
           /*Full duplex*/
           case EUSART_ASYNC_MODE :
               ret |= EUSART_Tx_initalize(&(usart_obj->transmit_cnfg));
               ret |= EUSART_Rx_initalize(&(usart_obj->receive_cnfg));
               break;
            /*half duplex*/
           case EUSART_SYNC_MODE :
               break;
       } 
      /*enable serial port*/
       EUSART_SERIAL_PORT_ENABLE();
    }
    return ret;
}



Std_ReturnType mcal_EUSART_ASYNC_Transmit_Byte_Blocking(const USART_t *usart_obj, uint8 data)
{
    Std_ReturnType ret  = E_NOT_OK;
    if(usart_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
      while(!TXSTAbits.TRMT);
      TXREG = data;
      ret = E_OK;
    }
    return ret;
}



static Std_ReturnType EUSART_Baud_rate_gen(const USART_t *usart_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    uint16 baud_rate_value;
    switch(usart_obj->Baud_rate_confg){
        case ASYNC_8BIT_LOW_SPEED : 
            EUSART_BAUD_RATE_LOW_SPEED();
            EUSART_BAUD_RATE_8BIT();
            baud_rate_value = ((_XTAL_FREQ/usart_obj->baudrate_val)/64)-1; 
            SPBRG = (uint8)(baud_rate_value & 0xff);
            ret = E_OK;
            break;
        case ASYNC_8BIT_HIGH_SPEED :
             EUSART_BAUD_RATE_8BIT();
             EUSART_BAUD_RATE_HIGH_SPEED();
            baud_rate_value = ((_XTAL_FREQ/usart_obj->baudrate_val)/16)-1; 
            SPBRG = (uint8)(baud_rate_value & 0xff);
             ret = E_OK;
            break;
        case ASYNC_16BIT_LOW_SPEED:
             EUSART_BAUD_RATE_LOW_SPEED();
             EUSART_BAUD_RATE_16BIT();
            baud_rate_value = ((_XTAL_FREQ/usart_obj->baudrate_val)/16)-1; 
            SPBRG = (uint8)(baud_rate_value & 0xff);
            SPBRGH = (uint8)((baud_rate_value >> 8) & 0xff);
             ret = E_OK;
            break;
        case ASYNC_16BIT_HIGH_SPEED:
             EUSART_BAUD_RATE_HIGH_SPEED();
             EUSART_BAUD_RATE_16BIT();
             baud_rate_value = ((_XTAL_FREQ/usart_obj->baudrate_val)/4)-1; 
             SPBRG =(uint8) (baud_rate_value & 0xff);
             SPBRGH = (uint8)((baud_rate_value >>8 ) & 0xff);
              ret = E_OK;
            break;
        default:
            ret = E_NOT_OK;
            break;
    }
    return ret;

}

static Std_ReturnType EUSART_Select_Sync_Mode(const USART_t* usart_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    switch(usart_obj->sync_mode)
    {
        case EUSART_ASYNC_MODE:
            EUSART_ASYNC_MODE_ENABLE();
             ret = E_OK;
            break;
        case EUSART_SYNC_MODE:
            EUSART_SYNC_MODE_ENABLE();
             ret = E_OK;
            break;
        default :
               ret = E_NOT_OK;
    
    }
    return ret;
}


static Std_ReturnType EUSART_Tx_interrupt_config(const tx_cnfg_t* Tx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    if(Tx_obj == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
#if EUSART_TRANSMIT_INTERRUPT_CONFG == INTERRUPT_ENABLE
        
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_LOW_PRIORITY();
    ENABLE_ALL_HIGH_PRIORITY(); 
    switch(Tx_obj->priority){
            case HIGH_PRIORITY:
               EUSART_TRANSMIT_SET_HIGH_PRIORITY(();
               break;
            case LOW_PRIORITY:
                EUSART_TRANSMIT_SET_LOW_PRIORITY();
              break;
    }
              
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    
    EUSART_TRANSMIT_CLEAR_INTERRUPT_FLAG();
    EUSART_TRANSMIT_INTERRUPT_ENABLE();
    Transmit_interrupt_handler = Tx_obj->Tx_handler;
    ret = E_OK;

#endif        
        
    }
   return ret;

}


static Std_ReturnType EUSART_Tx_initalize(const tx_cnfg_t* Tx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    if(Tx_obj == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        /* TX enable or disable*/
        ret|= EUSART_Tx_config(Tx_obj);
        /*nth bits configuration*/
        ret |= EUSART_Tx_nth_bit_config(Tx_obj);
        /*interrupt configuration*/
#if EUSART_INTERRUPT_CONFG == INTERRUPT_ENABLE
        ret |= EUSART_Tx_interrupt_config(Tx_obj);
#endif
    }
    return ret;

}

static Std_ReturnType EUSART_Tx_nth_bit_config(const tx_cnfg_t *Tx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    switch(Tx_obj->usart_tx_9bit_enable)
    {
        case EUSART_SELECT_9BIT_TRANSMIT:
            EUSART_ASYNC_ENABLE_9BIT_TRANSMIT();
            ret = E_OK;
            break;
        case EUSART_SELECT_8BIT_TRANSMIT:
             EUSART_ASYNC_ENABLE_8BIT_TRANSMIT();
             ret = E_OK;
            break;
        default:
            ret = E_NOT_OK;
          
    }
    return ret;

}

static Std_ReturnType EUSART_Tx_config(const tx_cnfg_t *Tx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    switch(Tx_obj->usart_tx_enable)
    {
        case  EUSART_TRANSMIT_MODE_ON:
            EUSART_ASYNC_ENABLE_TRANSMITER();
            ret = E_OK;
            break;
        case  EUSART_TRANSMIT_MODE_OFF:
             EUSART_ASYNC_DISABLE_TRANSMITER();
             ret = E_OK;
            break;
        default:
            ret = E_NOT_OK;
          
    }
    return ret;

}


static Std_ReturnType EUSART_Rx_nth_bit_config(const rx_cnfg_t *Rx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    switch(Rx_obj->usart_rx_9bit_enable)
    {
        case EUSART_SELECT_9BIT_RECEIVE:
            EUSART_ASYNC_ENABLE_9BIT_RECEIVE();
            ret = E_OK;
            break;
        case EUSART_SELECT_8BIT_RECEIVE:
             EUSART_ASYNC_ENABLE_8BIT_RECEIVE();
             ret = E_OK;
            break;
        default:
            ret = E_NOT_OK;
          
    }
    return ret;

}

static Std_ReturnType EUSART_Rx_initalize(const rx_cnfg_t *Rx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    if(Rx_obj == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        /* RX enable or disable*/
         ret |= EUSART_Rx_config(Rx_obj);
        /*nth bits configuration*/
        ret |= EUSART_Rx_nth_bit_config(Rx_obj);
        /*interrupt configuration*/
#if EUSART_INTERRUPT_CONFG == INTERRUPT_ENABLE
        ret |= EUSART_Rx_interrupt_config(Rx_obj);
#endif        
    }
    return ret;

    

}

static Std_ReturnType EUSART_Rx_config(const rx_cnfg_t *Rx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    switch(Rx_obj->usart_rx_enable)
    {
        case  EUSART_RECEIVE_MODE_ON:
            EUSART_ASYNC_ENABLE_RECEIVER();;
            ret = E_OK;
            break;
        case  EUSART_TRANSMIT_MODE_OFF:
              EUSART_ASYNC_DISABLE_RECEIVER();
             ret = E_OK;
            break;
        default:
            ret = E_NOT_OK;
          
    }
    return ret;
}

static Std_ReturnType EUSART_Rx_interrupt_config(const rx_cnfg_t *Rx_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    if(Rx_obj == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
#if EUSART_RECEIVE_INTERRUPT_CONFG == INTERRUPT_ENABLE
        
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_LOW_PRIORITY();
    ENABLE_ALL_HIGH_PRIORITY(); 
    switch(Tx_obj->priority)
            case HIGH_PRIORITY:
               EUSART_RECEIVE_SET_HIGH_PRIORITY(();
               break;
            case LOW_PRIORITY:
                EUSART_RECEIVE_SET_LOW_PRIORITY();
              break;
              
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    
     Recieve_interrupt_handler = Rx_obj->Rx_handler;
     Overrun_interrupt_handler = Rx_obj->Overrun_handler;
     Frame_error_interrupt_handler = Rx_obj->Frame_error_handler;


    EUSART_RECEIVE_CLEAR_INTERRUPT_FLAG();
    EUSART_RECEIVE_INTERRUPT_ENABLE();
    
    ret = E_OK;

#endif
    }
    return ret;
}

Std_ReturnType mcal_EUSART_ASYNC_Receive_Byte_Blocking(const USART_t *usart_obj, uint8 *data)
{
    Std_ReturnType ret = E_NOT_OK;
    if(data == NULL || usart_obj == NULL )
    {
        ret = E_NOT_OK;
    }
    else
    {
        /*RCIF (cleared when RCREG is read) */
         while(!PIR1bits.RCIF);
         *data = RCREG ;
    }
    return ret;
}

static Std_ReturnType EUSART_Restart_Reciever()
{
     Std_ReturnType ret = E_NOT_OK;
     /*clear overrun error*/
     EUSART_ASYNC_DISABLE_RECEIVER();   
     EUSART_ASYNC_ENABLE_RECEIVER();
     ret = E_OK;
     return ret;
}

void Rx_ISR(){
    if(Recieve_interrupt_handler)
        Recieve_interrupt_handler();
    
    if(Overrun_interrupt_handler)
        Overrun_interrupt_handler();
    
    if(Frame_error_interrupt_handler)
        Frame_error_interrupt_handler();
}
void Tx_ISR(){
    
    if(Transmit_interrupt_handler)
        Transmit_interrupt_handler();
}