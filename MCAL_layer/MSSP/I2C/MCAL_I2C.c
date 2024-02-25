/* 
 * File:   MSSP_I2C.h
 * Author: Honey
 *
 * Created on February 6, 2024, 2:26 AM
 */

#include "MSSP_I2C.h"

/*Pins configuration*/
static pin_config scl = {
            .direction = INPUT_DIRECTION,
            .port_index = PORTC_INDEX,
            .pin_index = PIN3,
};
static pin_config sda = {
            .direction = INPUT_DIRECTION,
            .port_index = PORTC_INDEX,
            .pin_index = PIN3,
};

static Std_ReturnType I2C_SMBus_Confg(const I2C_t *i2c_obj);
static Std_ReturnType I2C_SlewRate_Confg(const I2C_t *i2c_obj);
static Std_ReturnType I2C_Master_Confg(const I2C_t *i2c_obj);
static Std_ReturnType I2C_Slave_Confg(const I2C_t *i2c_obj);
static Std_ReturnType I2C_Interrupt_Cnfg(const I2C_t *i2c_obj);
static Std_ReturnType I2C_Mode_Configuration(const I2C_t *i2c_obj);



static Interrupt_Handler I2C_default_Interrupt_handler = NULL;
static Interrupt_Handler I2C_Bus_Col_Interrupt_handler = NULL;


Std_ReturnType MSSP_I2C_Initialize(const I2C_t * i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable I2C Mode */
         I2C_DISABLE_SERIAL_PORT();
        /*GPIO Configuration*/
        ret |= gpio_pin_direction_intialize(&sda);
        ret |= gpio_pin_direction_intialize(&scl);
        /*interrupt configuration*/
#if I2C_INTERRUPT_CONFG == INTERRUPT_ENABLE
        ret |= I2C_Interrupt_Cnfg(i2c_obj);
#endif 
         /*Select SMBus mode*/
         ret |= I2C_SMBus_Confg(i2c_obj);
         /*Select SlewRate mode*/
         ret |= I2C_SlewRate_Confg(i2c_obj);
         /*Select I2C Mode*/
         I2C_Mode_Configuration(i2c_obj);
        /*Enable I2C Mode */
         I2C_ENBALE_SERIAL_PORT();
         /*Release clock*/
         I2C_RELEASE_CLK();
    }
    return ret;

}


Std_ReturnType MSSP_I2C_Send_Start_Condition(const I2C_t * i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL ){
        ret = E_NOT_OK;
    }
    else{
        I2C_MASTER_SEND_START_CONDITION();      
        while(I2C_GET_START_CONDITION_STATUS() == I2C_START_CONDITION_INPROGRESS);
        if(I2C_GET_START_BIT_STATUS() == I2C_START_CONDITION_IS_DETECTED){
            ret |= E_OK;
        }
    }
    return ret;
}
Std_ReturnType MSSP_I2C_Send_ReStart_Condition(const I2C_t * i2c_obj){
        Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        I2C_MASTER_SEND_RESTART_CONDITION();      
        while(I2C_GET_START_CONDITION_STATUS() == I2C_START_CONDITION_INPROGRESS);
        if(I2C_GET_START_BIT_STATUS() == I2C_RESTART_CONDITION_IS_DETECTED){
            ret |= E_OK;
        }
    }
    return ret;
}
Std_ReturnType MSSP_I2C_Send_Stop_Condition(const I2C_t * i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL ){
        ret = E_NOT_OK;
    }
    else{
        I2C_MASTER_SEND_STOP_CONDITION();      
        while(I2C_GET_STOP_CONDITION_STATUS() == I2C_STOP_CONDITION_INPROGRESS);
        if(I2C_GET_STOP_BIT_STATUS() == I2C_STOP_CONDITION_IS_DETECTED){
            ret |= E_OK;
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Transmit_Byte(const I2C_t *i2c_obj,uint8 data,uint8 *ack){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        SSPBUF = data;
        while(I2C_GET_BUFFER_STATUS() == I2C_BUFFER_ISFULL);
        *ack |=  I2C_MASTER_RECEVIED_ACK_STATUS();
    }
    return ret;
}
Std_ReturnType MSSP_I2C_Recevie_Byte(const I2C_t *i2c_obj,uint8 *data,uint8 ack){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
     else{
        I2C_MASTER_RECEIVE_ENABLE();
        while(I2C_GET_BUFFER_STATUS() == I2C_BUFFER_ISEMPTY);
        *data = SSPBUF;
        I2C_MASTER_WRITE_ACK_DATA(ack);
        I2C_MASTER_START_ACK_SEQUENCE();
    }
    return ret;

}


static Std_ReturnType I2C_SMBus_Confg(const I2C_t * i2c_obj){
     Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(i2c_obj->i2c_SMBus_control == I2C_SMBus_IS_ENABLED ){
             I2C_SMBus_ENABLE();
             ret = E_OK;             
         }
         else{
             I2C_SMBus_DISABLE(); 
             ret = E_OK;
         }
    }
    return ret;
}
static Std_ReturnType I2C_SlewRate_Confg(const I2C_t * i2c_obj){
     Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
         if(i2c_obj->i2c_slew_rate == I2C_SLEW_RATE_IS_ENABLED){
            I2C_SLEW_RATE_ENABLE();
            ret = E_OK;
         }
         else{
            I2C_SLEW_RATE_DISABLE();
            ret = E_OK;
         }
    
    }
    return ret;
}
static Std_ReturnType I2C_Master_Confg(const I2C_t * i2c_obj){
     Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL || i2c_obj->Operation_Mode != I2C_MASTER_MODE){
        ret = E_NOT_OK;
    }
    else{
       if((i2c_obj->master_obj).Speed_mode == I2C_HIGH_SPEED_MODE){
            I2C_MASTER_SPEED_CNFG(I2C_HIGH_SPEED_VAL);
            ret = E_OK;
            
        }
        else if ((i2c_obj->master_obj).Speed_mode == I2C_STANDARD_SPEED_MODE){
            I2C_MASTER_SPEED_CNFG(I2C_STANDARD_SPEED_VAL);     
            ret = E_OK;   
        }
        
    }
    return ret;

}
static Std_ReturnType I2C_Slave_Confg(const I2C_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        uint16 address_bytes;
        if(i2c_obj->Operation_Mode == I2C_SLAVE_MODE_10_BIT_ADD_INTERRUPT_EN||
           i2c_obj->Operation_Mode ==  I2C_SLAVE_MODE_10_BITS_ADD_NO_INTERRUPT ){
             address_bytes = 0x3FF & i2c_obj->slave_obj.Slave_Address ;
             SSPADD = (uint8)(0xF0 | ((address_bytes >> 7)&0x06));
             while(I2C_SLAVE_GET_UPDATE_ADD_STATUS() == I2C_SLAVE_10BIT_ADD_UPDATE_NO_NEED);
             SSPADD = (uint8) (address_bytes & 0xFF);
             while(I2C_GET_BUFFER_STATUS() == I2C_BUFFER_ISEMPTY);
        }
        else{
            SSPADD = (uint8)(i2c_obj->slave_obj.Slave_Address) ;    
        }
        
        if(i2c_obj->slave_obj.i2c_General_Call == I2C_SLAVE_GENERAL_CALL_IS_ENABLED){
            I2C_SLAVE_GENERAL_CALL_ENABLE();
        }
        else{
         I2C_SLAVE_GENERAL_CALL_DISABLE();
        }
        
        
    }
    return ret;

}
static Std_ReturnType I2C_Mode_Configuration(const I2C_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
         I2C_SELECT_MODE(i2c_obj->Operation_Mode);  
        if(i2c_obj->Operation_Mode == I2C_MASTER_MODE){
           ret |= I2C_Master_Confg(i2c_obj);
        }
        else{
            ret |= I2C_Slave_Confg(i2c_obj);
        }
        
    
    
    }
    return ret;

}

#if I2C_INTERRUPT_CONFG == INTERRUPT_ENABLE
static Std_ReturnType  I2C_Interrupt_Cnfg(const I2C_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(i2c_obj == NULL){
        ret = E_NOT_OK;
    }
    else{        
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    ENABLE_PRIORITY_BIT();
    ENABLE_ALL_LOW_PRIORITY();
    ENABLE_ALL_HIGH_PRIORITY(); 
    switch(i2c_obj->I2C_Default_Priority){
            case HIGH_PRIORITY:
              I2C_SET_HIGH_PRIORITY();
               break;
            case LOW_PRIORITY:
              I2C_SET_LOW_PRIORITY();
              break;
    }
    switch(i2c_obj->master_obj->Bus_Col_interrupt_priority){
            case HIGH_PRIORITY:
              I2C_BUS_COL_SET_HIGH_PRIORITY();
               break;
            case LOW_PRIORITY:
                I2C_BUS_COL_SET_LOW_PRIORITY()  
               break;
    }
              
#else
    ENABLE_GLOBAL_INTERRUPT_BIT();
    ENABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    
    I2C_CLEAR_INTERRUPT_FLAG();
    I2C_INTERRUPT_ENABLE();
    I2C_BUS_COL_INTERRUPT_ENABLE();
    I2C_default_Interrupt_handler = i2c_obj->I2C_Default_Handler;
    I2C_Bus_Col_Interrupt_handler = i2c_obj->master_obj.Bus_Col_Handler;
    
    ret = E_OK;        
    }
    return ret;

}



void I2C_ISR(){
    I2C_CLEAR_INTERRUPT_FLAG();
    if(I2C_default_Interrupt_handler)
        I2C_default_Interrupt_handler();
}
void I2C_BUS_COL_ISR(){
    I2C_BUS_COL_CLEAR_INTERRUPT_FLAG();
    if(I2C_Bus_Col_Interrupt_handler)
        I2C_Bus_Col_Interrupt_handler();
}
#endif

