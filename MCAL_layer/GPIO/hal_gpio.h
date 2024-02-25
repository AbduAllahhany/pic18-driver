/* 
 * File:   hal_gpio.h
 * Author: Honey
 *
 * Created on August 26, 2023, 4:01 AM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/*Section include*/

#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "hal_gpio_config.h"
/*Section Macro declaration*/
#define BIT_MASK                        (uint8)0x01

#define MAX_PORT_NUM                    (uint8)0x05  
#define MAX_PIN_NUM                     (uint8)0x08
#define MAX_PIN_NUM_PORTE               (uint8)0x04 

#define GPIO_LOW                        0x0
#define GPIO_HIGH                       0x1

#define GPIO_PIN_CONFIG                 CONFIG_ENABLE
#define GPIO_PORT_CONFIG                CONFIG_ENABLE


/*Section Macro function declaration*/

#define HWREG8(_x)                      (*((volatile uint8 *)(_x)))

#define SET_BIT(REG,BIT_POSN)           (REG |= (BIT_MASK << BIT_POSN))   
#define CLEAR_BIT(REG,BIT_POSN)         (REG &= ~(BIT_MASK << BIT_POSN))   
#define TOGGLE_BIT(REG,BIT_POSN)        (REG ^= (BIT_MASK << BIT_POSN))   
#define READ_BIT(REG,BIT_POSN)          ((REG >> BIT_POSN) & BIT_MASK)

/*Section Data type declaration*/

typedef enum{
    LOW = 0,
    HIGH        
}logic_t;

typedef enum{
    OUTPUT_DIRECTION = 0,
    INPUT_DIRECTION
}direction_t;

typedef enum{
    PIN0 = 0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7
}pin_index_t;
typedef enum{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX
}port_index_t;

typedef struct{
    uint8 port_index           :3; //@ref port_index_t
    uint8 pin_index            :3; //@ref pin_index_t
    uint8 direction            :1; //@ref direction_t
    uint8 logic                :1; //@ref logic_t
    
}pin_config;


/*Section fun prototype*/

Std_ReturnType gpio_pin_direction_intialize  (const pin_config *_pin_config);
Std_ReturnType gpio_pin_get_direction_status (const pin_config *_pin_config,
                                              direction_t  *_direction_status);
Std_ReturnType gpio_write_pin                (const pin_config *_pin_config,
                                              logic_t logic);
Std_ReturnType gpio_read_pin                 (const pin_config *_pin_config ,
                                              logic_t *logic);
Std_ReturnType gpio_toggle_pin_logic         (const pin_config *_pin_config);
Std_ReturnType gpio_pin_intialize            (const pin_config *_pin_config);


Std_ReturnType gpio_port_direction_intialize  (const port_index_t _port
                                              ,uint8  _direction);
Std_ReturnType gpio_port_get_direction_status (const port_index_t _port
                                              ,direction_t  *_direction);
Std_ReturnType gpio_write_port                (const port_index_t _port
                                              ,uint8 logic);
Std_ReturnType gpio_read_port                 (const port_index_t _port 
                                              ,uint8 *logic);
Std_ReturnType gpio_toggle_port_logic         (const port_index_t _port);





#endif	/* HAL_GPIO_H */

