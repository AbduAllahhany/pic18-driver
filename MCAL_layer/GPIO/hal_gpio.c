/* 
 * File:   hal_gpio.c
 * Author: Honey
 *
 * Created on August 26, 2023, 4:01 AM
 */
#include "hal_gpio.h"

// TRIS Registers addresses
volatile uint8 *TRIS_REG[] = {&TRISA,&TRISB,&TRISC,&TRISD,&TRISE};
// PORT Registers addresses
volatile uint8 *PORT_REG[] = {&PORTA,&PORTB,&PORTC,&PORTD,&PORTE};
// LAT Registers addresses
volatile uint8 *LAT_REG[] = {&LATA,&LATB,&LATC,&LATD,&LATE}; 


// PIN configuration
#if GPIO_PIN_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_pin_direction_intialize (const pin_config *_pin_config)
{
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL || _pin_config->port_index >= MAX_PORT_NUM )
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch(_pin_config->direction)
        {
                case (OUTPUT_DIRECTION):
                    CLEAR_BIT(*TRIS_REG[_pin_config->port_index],(_pin_config->pin_index));
                    break;
                case (INPUT_DIRECTION):
                    SET_BIT(*TRIS_REG[_pin_config->port_index],(_pin_config->pin_index));
                    break;
                default:
                    ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif

#if GPIO_PIN_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_pin_get_direction_status(const pin_config *_pin_config,
                                              direction_t  *_direction_status)
{
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL || _direction_status == NULL 
                           || _pin_config->port_index >= MAX_PORT_NUM)
    {
        ret = E_NOT_OK;
    }
    else
    {
       *_direction_status = READ_BIT(*TRIS_REG[_pin_config->port_index],_pin_config->pin_index);
    }
    return ret;
}
#endif

#if GPIO_PIN_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_write_pin(const pin_config *_pin_config,logic_t _logic)
{
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL || _pin_config->port_index >= MAX_PORT_NUM 
      || _pin_config->pin_index >= MAX_PIN_NUM 
      ||(_pin_config->pin_index >= MAX_PIN_NUM_PORTE && _pin_config->port_index == PORTE_INDEX))
    {
             ret = E_NOT_OK;
    }
    else
    {
        switch(_logic)
        {
            case STD_HIGH:
                SET_BIT(*LAT_REG[_pin_config->port_index],_pin_config->pin_index);
                break;
            case STD_LOW:
                CLEAR_BIT(*LAT_REG[_pin_config->port_index],_pin_config->pin_index);
                break;
            default:
                ret=E_NOT_OK;
        }
     
    }
    return ret;
}
#endif

#if GPIO_PIN_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_read_pin (const pin_config *_pin_config ,logic_t *_logic)
{
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL || _pin_config->port_index >= MAX_PORT_NUM || _logic == NULL
      || _pin_config->pin_index >= MAX_PIN_NUM 
      ||(_pin_config->pin_index >= MAX_PIN_NUM_PORTE && _pin_config->port_index == PORTE_INDEX))
    {
        ret = E_NOT_OK;
    }
    else
    {
       *_logic = READ_BIT(*PORT_REG[_pin_config->port_index],_pin_config->pin_index);
    }
    return ret;
}
#endif

#if GPIO_PIN_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_toggle_pin_logic(const pin_config *_pin_config)
{
    Std_ReturnType ret = E_OK;
   if(_pin_config == NULL || _pin_config->port_index >= MAX_PORT_NUM 
      || _pin_config->pin_index >= MAX_PIN_NUM 
      ||(_pin_config->pin_index >= MAX_PIN_NUM_PORTE && _pin_config->port_index == PORTE_INDEX))
    {
        ret = E_NOT_OK;
    }
    else
    {
       TOGGLE_BIT(*LAT_REG[_pin_config->port_index],_pin_config->pin_index);
    }
    return ret;
}
#endif

#if GPIO_PIN_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_pin_intialize  (const pin_config *_pin_config)
{
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL || _pin_config->port_index >= MAX_PORT_NUM )
    {
        ret = E_NOT_OK; 
    }
    else
    {
       ret = gpio_pin_direction_intialize(_pin_config);
       ret = gpio_write_pin(_pin_config,_pin_config->logic);
    }
    return ret;

}
#endif
// PORT configuration
#if GPIO_PORT_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_port_direction_intialize  (const port_index_t _port
                                               ,uint8 _direction)
{
    Std_ReturnType ret = E_OK;
    if(_port >= MAX_PORT_NUM  )
    {
        ret = E_NOT_OK; 
    
    }
    else
    {
        *TRIS_REG[_port] = _direction;
    
    }
    return ret;
}
#endif

#if GPIO_PORT_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_port_get_direction_status (const port_index_t _port
                                              ,direction_t  *_direction)
{
    Std_ReturnType ret = E_OK;
    if(_port >= MAX_PORT_NUM || _direction == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
      *_direction = *TRIS_REG[_port] ;
    }
    return ret;   
}
#endif

#if GPIO_PORT_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_write_port (const port_index_t _port ,uint8 logic)
{
    Std_ReturnType ret = E_OK;
    if(_port >= MAX_PORT_NUM)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        *LAT_REG[_port] = logic; 
    }
    return ret;   
}
#endif

#if GPIO_PORT_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_read_port (const port_index_t _port ,uint8 *logic)
{
       Std_ReturnType ret = E_OK;
    if(_port >= MAX_PORT_NUM || logic == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        *logic = *PORT_REG[_port] ; 
    }
    return ret;      
}
#endif

#if GPIO_PORT_CONFIG == CONFIG_ENABLE
Std_ReturnType gpio_toggle_port_logic (const port_index_t _port)
{
    Std_ReturnType ret = E_OK;
    if(_port >= MAX_PORT_NUM)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        *LAT_REG[_port] = ~(*LAT_REG[_port]) ; 
    }
    return ret;   
    


}
#endif




