/* 
 * File:   mcal_eusart.h
 * Author: Honey
 *
 * Created on November 1, 2023, 4:53 PM
 */

#ifndef MCAL_EUSART_H
#define	MCAL_EUSART_H

/*Section include*/

#include "../INTERRUPT/mcal_internal_interrupt.h"
#include "../GPIO/hal_gpio.h"
#include "pic18f4620.h"
#include "../mcal_std_types.h"

/*Section Macro declaration*/
#define EUSART_TRANSMIT_MODE_ON                                             0x00
#define EUSART_TRANSMIT_MODE_OFF                                            0x01
#define EUSART_RECEIVE_MODE_ON                                              0x00
#define EUSART_RECEIVE_MODE_OFF                                             0x01

#define EUSART_ASYNC_MODE                                                   0x00
#define EUSART_SYNC_MODE                                                    0x01

#define EUSART_SYNC_MASTER_MODE                                             0x01 
#define EUSART_SYNC_SLAVE_MODE                                              0x00

#define EUSART_SELECT_9BIT_TRANSMIT                                         0x01
#define EUSART_SELECT_8BIT_TRANSMIT                                         0x00

#define EUSART_SELECT_9BIT_RECEIVE                                          0x01
#define EUSART_SELECT_8BIT_RECEIVE                                          0x00

#define EUSART_HIGH_SPEED_BAUD_RATE                                         0x01
#define EUSART_LOW_SPEED_BAUD_RATE                                          0x00

#define EUSART_MANUAL_BAUD_RATE                                             0x00
#define EUSART_AUTO_BAUD_RATE                                               0x01



/*Section Macro function declaration*/

#define EUSART_SERIAL_PORT_ENABLE()                                         RCSTAbits.SPEN = 1
#define EUSART_SERIAL_PORT_DISABLE()                                        RCSTAbits.SPEN = 0

#define EUSART_ASYNC_MODE_ENABLE()                                          TXSTAbits.SYNC = 0
#define EUSART_SYNC_MODE_ENABLE()                                           TXSTAbits.SYNC = 1

#define EUSART_ASYNC_ENABLE_TRANSMITER()                                    TXSTAbits.TXEN = 1
#define EUSART_ASYNC_DISABLE_TRANSMITER()                                   TXSTAbits.TXEN = 0

#define EUSART_ASYNC_ENABLE_9BIT_TRANSMIT()                                 TXSTAbits.TX9 = 1                                         
#define EUSART_ASYNC_ENABLE_8BIT_TRANSMIT()                                 TXSTAbits.TX9 = 0

#define EUSART_ASYNC_ENABLE_9BIT_RECEIVE()                                  RCSTAbits.RX9 = 1                                         
#define EUSART_ASYNC_ENABLE_8BIT_RECEIVE()                                  RCSTAbits.RX9 = 0

#define EUSART_ASYNC_ENABLE_RECEIVER()                                      RCSTAbits.CREN = 1
#define EUSART_ASYNC_DISABLE_RECEIVER()                                     RCSTAbits.CREN = 0

/* can be cleared by reading RCREG register and receiving next valid byte */
#define EUSART_GET_FRAMMING_ERROR_STATUS()                                  RCSTAbits.FERR
#define EUSART_CLEAR_FRAMMING_ERROR_BIT()                                   RCSTAbits.FERR = 0

/*can be cleared by clearing bit, CREN) */
#define EUSART_GET_OVERRUN_ERROR_STATUS()                                   RCSTAbits.OERR
#define EUSART_CLEAR_OVERRUN_ERROR_BIT()                                    RCSTAbits.OERR = 0

/*EUSART Baud rate */
#define EUSART_BAUD_RATE_HIGH_SPEED()                                       TXSTAbits.BRGH = 1
#define EUSART_BAUD_RATE_LOW_SPEED()                                        TXSTAbits.BRGH = 0
#define EUSART_BAUD_RATE_8BIT()                                             BAUDCTLbits.BRG16 = 0
#define EUSART_BAUD_RATE_16BIT()                                            BAUDCTLbits.BRG16 = 1
#define EUSART_AUTO_BAUD_RATE_ENABLE()                                      BAUDCTLbits.ABDEN = 1
#define EUSART_AUTO_BAUD_RATE_DISABLE()                                     BAUDCTLbits.ABDEN = 0

/*Section Data type declaration*/
typedef enum{
    ASYNC_8BIT_LOW_SPEED = 0,
    ASYNC_8BIT_HIGH_SPEED,
    ASYNC_16BIT_LOW_SPEED,
    ASYNC_16BIT_HIGH_SPEED,
    SYNC_8BIT,
    SYNC_16BIT

}baud_rate_cnfg_t;

typedef struct{
    
#if EUSART_TRANSMIT_INTERRUPT_CONFG == INTERRUPT_ENABLE
    Interrupt_Handler Tx_handler;
#endif
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    uint8 priority;
#endif
    uint8 usart_tx_enable : 1;
    uint8 usart_tx_interrupt_enable : 1;
    uint8 usart_tx_9bit_enable : 1;
}tx_cnfg_t;

typedef struct{
#if EUSART_TRANSMIT_INTERRUPT_CONFG == INTERRUPT_ENABLE
    Interrupt_Handler Rx_handler;
    Interrupt_Handler Overrun_handler;
    Interrupt_Handler Frame_error_handler;
#endif
#if PIRIORITY_ENABLE == PIRIORITY_ON 
    uint8 priority;
#endif
    uint8 usart_rx_enable : 1;
    uint8 usart_rx_interrupt_enable : 1;
    uint8 usart_rx_9bit_enable : 1;
    

}rx_cnfg_t;

typedef struct{
    uint8 sync_mode             :1;
    uint8 baudrate_mode         :1; //auto or not
    uint16 baudrate_val           ;
    baud_rate_cnfg_t Baud_rate_confg;
    tx_cnfg_t transmit_cnfg;
    rx_cnfg_t receive_cnfg;
}USART_t;

/*Section fun prototype*/
Std_ReturnType mcal_EUSART_initialize(const USART_t *usart_obj);

Std_ReturnType mcal_EUSART_ASYNC_Transmit_Byte_Blocking(const USART_t *usart_obj, uint8 data);

Std_ReturnType mcal_EUSART_ASYNC_Receive_Byte_Blocking(const USART_t *usart_obj, uint8 *data);



#endif	/* MCAL_EUSART_H */

