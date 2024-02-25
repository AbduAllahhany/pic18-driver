/* 
 * File:   MSSP_I2C.h
 * Author: Honey
 *
 * Created on February 6, 2024, 2:26 AM
 */

#ifndef MSSP_I2C_H
#define	MSSP_I2C_H

/*Section include*/

#include "../../INTERRUPT/mcal_internal_interrupt.h"
#include "../../INTERRUPT/mcal_interrupt_cnfg.h"
#include "../../GPIO/hal_gpio.h"
#include "pic18f4620.h"
#include "../../mcal_std_types.h"


/*Section Macro declaration*/


/*Master Synchronous Serial Port Enable bit*/
#define I2C_IS_ENABLED                                                      0x01
#define I2C_IS_DISABLED                                                     0x00

/*Speed*/
#define I2C_HIGH_SPEED_MODE                                                 0x01
#define I2C_STANDARD_SPEED_MODE                                             0x00

#define I2C_HIGH_SPEED_VAL                                                  400000
#define I2C_STANDARD_SPEED_VAL                                              100000 

/*Slew rate control*/
#define I2C_SLEW_RATE_IS_ENABLED                                            0x01
#define I2C_SLEW_RATE_IS_DISABLED                                           0x00

/*SMbus control*/
#define I2C_SMBus_IS_ENABLED                                                0x01
#define I2C_SMBus_IS_DISABLED                                               0x00

/*I2C Modes*/
#define I2C_SLAVE_MODE_10_BIT_ADD_INTERRUPT_EN                              0b1111
#define I2C_SLAVE_MODE_7_BIT_ADD_INTERRUPT_EN                               0b1110
#define I2C_MASTER_MODE                                                     0b1000
#define I2C_SLAVE_MODE_10_BITS_ADD_NO_INTERRUPT                             0b0111
#define I2C_SLAVE_MODE_7_BITS_ADD_NO_INTERRUPT                              0b0110

/*Write/Read status*/
#define I2C_SLAVE_MODE_WRITE                                                0x00
#define I2C_SLAVE_MODE_READ                                                 0x01

/*Transmit status */
#define I2C_MASTER_MODE_TRANSMIT_IN_PROGRESS                                0x01
#define I2C_SLAVE_MODE_TRANSMIT_IN_PROGRESS                                 0x00

/*Start condition detection */
#define I2C_START_CONDITION_IS_DETECTED                                     0x01
#define I2C_RESTART_CONDITION_IS_DETECTED                                     0x01
#define I2C_STOP_CONDITION_IS_DETECTED                                      0x01

/*restart condition configuration*/
#define I2C_RESTART_IS_ENABLED                                              0x01
#define I2C_RESTART_IS_DISABLED                                             0x00

/*Address update status*/
#define I2C_SLAVE_10BIT_ADD_UPDATE_NEEDED                                   0x01
#define I2C_SLAVE_10BIT_ADD_UPDATE_NO_NEED                                  0x00

#define I2C_START_CONDITION_INPROGRESS                                      0x01
#define I2C_RESTART_CONDITION_INPROGRESS                                    0x01
#define I2C_STOP_CONDITION_INPROGRESS                                       0x01


/*Buffer Full Status bit*/
#define I2C_BUFFER_ISFULL                                                   0x01
#define I2C_BUFFER_ISEMPTY                                                  0x00

/*write collision bit WCOL*/
#define I2C_WRITE_NO_COLLISION                                              0x00
#define I2C_WRITE_COLLISION                                                 0x01

/*Receive Overflow Indicator bit */
#define I2C_OVERFLOW_RECEIVE                                                0x01
#define I2C_NO_OVERFLOW_RECEIVE                                             0x00

/*SCK Release Control bit */
#define I2C_SLAVE_CLK_IS_RELEASED                                           0x01
#define I2C_SLAVE_CLK_IS_HOLD                                               0x00

/*General Call Enable bit (Slave mode only)*/
#define I2C_SLAVE_GENERAL_CALL_IS_ENABLED                                   0x01
#define I2C_SLAVE_GENERAL_CALL_IS_DISABLED                                  0x00

/*ACK*/
#define I2C_MASTER_ACK_SEQUENCE_IS_IDLE                                     0x00
#define I2C_ACK                                                             0x00
#define I2C_NACK                                                            0x01

/*Section Macro function declaration*/

/*SELECT I2C MODE */
#define I2C_SELECT_MODE(_mode)                                              SSPCON1bits.SSPM = _mode 

/*Slew rate control*/
#define I2C_SLEW_RATE_ENABLE()                                              SSPSTATbits.SMP = 0
#define I2C_SLEW_RATE_DISABLE()                                             SSPSTATbits.SMP = 1

/*SMbus control*/
#define I2C_SMBus_ENABLE()                                                  SSPSTATbits.CKE = 1
#define I2C_SMBus_DISABLE()                                                 SSPSTATbits.CKE = 0

/* Data/Address bit 
  1 = Indicates that the last byte received or transmitted was data
  0 = Indicates that the last byte received or transmitted was address
 */
#define I2C_SLAVE_GET_LAST_BYTE_REC_TYPE()                                  SSPSTATbits.DA

/*Stop and Start condition status*/
#define I2C_GET_STOP_BIT_STATUS()                                           SSPSTATbits.P
#define I2C_GET_START_BIT_STATUS()                                          SSPSTATbits.S

/*Read /Write Information bit */
#define I2C_SLAVE_GET_READ_WRITE_STATUS()                                   SSPSTATbits.RW

/*Transmit status*/
#define I2C_MASTER_GET_TRANSMIT_STATUS_PROGRESS()                           SSPSTATbits.RW

/*Update Address bit (10-Bit Slave mode only) status*/
#define I2C_SLAVE_GET_UPDATE_ADD_STATUS()                                   SSPSTATbits.UA

/*Buffer Full Status bit*/
#define I2C_GET_BUFFER_STATUS()                                             SSPSTATbits.BF

/*Write Collision Detect bit*/
#define I2C_CLEAR_WCOL_BIT()                                                SSPCON1bits.WCOL = 0

/*Receive Overflow Indicator bit */
#define I2C_CLEAR_OVERFLOW_BIT()                                            SSPCON1bits.SSPOV = 0

/*Enable /Disable serial port */
#define I2C_ENBALE_SERIAL_PORT()                                            SSPCON1bits.SSPEN = 1
#define I2C_DISABLE_SERIAL_PORT()                                           SSPCON1bits.SSPEN = 0

/*Slave Release/Hold clock*/
#define I2C_RELEASE_CLK()                                                   SSPCON1bits.CKP = 1
#define I2C_HOLD_CLK()                                                      SSPCON1bits.CKP = 0

/* General Call Enable bit*/
#define I2C_SLAVE_GENERAL_CALL_ENABLE()                                     SSPCON2bits.GCEN = 1
#define I2C_SLAVE_GENERAL_CALL_DISABLE()                                    SSPCON2bits.GCEN = 0

/*Master Acknowledge Status bits*/
#define I2C_MASTER_START_ACK_SEQUENCE()                                     SSPCON2bits.ACKEN = 1
#define I2C_MASTER_WRITE_ACK_DATA(_ACK)                                     SSPCON2bits.ACKDT = _ACK
#define I2C_MASTER_RECEVIED_ACK_STATUS()                                    SSPCON2bits.ACKSTAT

/*Receive Enable bit (Master mode only)*/
#define I2C_MASTER_RECEIVE_ENABLE()                                         SSPCON2bits.RCEN = 1

/* Stop Condition Enable bit (Master mode only)*/
#define I2C_MASTER_SEND_STOP_CONDITION()                                    SSPCON2bits.PEN = 1
#define I2C_GET_STOP_CONDITION_STATUS()                                     SSPCON2bits.PEN 

/* Repeated Start Condition Enable bit (Master mode only)*/
#define I2C_MASTER_SEND_RESTART_CONDITION()                                 SSPCON2bits.RSEN = 1
#define I2C_GET_RESTART_CONDITION_STATUS()                                  SSPCON2bits.RSEN 

/*Start Condition Enable bit (Master mode only)*/
#define I2C_MASTER_SEND_START_CONDITION()                                   SSPCON2bits.SEN = 1
#define I2C_GET_START_CONDITION_STATUS()                                    SSPCON2bits.SEN 

/* Stretch CLK Enable bit (Slave Mode)*/
#define I2C_SLAVE_CLK_STRETCH_ENABLE()                                      SSPCON2bits.SEN = 1
#define I2C_SLAVE_CLK_STRETCH_DISABLE()                                     SSPCON2bits.SEN = 0

/*Determine speed*/
#define I2C_MASTER_SPEED_CNFG(i2c_CLK_val)                                  SSPADD = (_XTAL_FREQ / (4 * i2c_CLK_val))-1

/*Select Slave address*/
#define I2C_SLAVE_SELECT_ADDRESS(_ADD)                                      SSPADD = _ADD

/*Section Data type declaration*/

typedef struct{
   /*interrupt handlers*/
     Interrupt_Handler Bus_Col_Handler;
     uint8 Bus_Col_interrupt_priority ;
     uint8 Speed_mode;

}I2C_Master_t;
typedef struct{
    uint16 Slave_Address      ;   
    uint8  i2c_General_Call : 1;		/* General Call Enable/Disable */

}I2C_Slave_t;
typedef struct{
    Interrupt_Handler I2C_Default_Handler; 
    uint8 I2C_Default_Priority;
   
    uint8 Operation_Mode    :4;
    
    uint8 i2c_slew_rate     : 1; 		/* Slew Rate Enable/Disable */
    uint8 i2c_SMBus_control : 1;    	/* SMBus Enable/Disable */
    
    I2C_Slave_t slave_obj;
    I2C_Master_t master_obj;
    
}I2C_t;

/*Section fun prototype*/

Std_ReturnType MSSP_I2C_Initialize(const I2C_t * i2c_obj);

Std_ReturnType MSSP_I2C_Send_Start_Condition(const I2C_t * i2c_obj);
Std_ReturnType MSSP_I2C_Send_ReStart_Condition(const I2C_t * i2c_obj);
Std_ReturnType MSSP_I2C_Send_Stop_Condition(const I2C_t * i2c_obj);


Std_ReturnType MSSP_I2C_Transmit_Byte(const I2C_t *i2c_obj,uint8 data,uint8 *ack);

Std_ReturnType MSSP_I2C_Recevie_Byte(const I2C_t *i2c_obj
                                 ,uint8 *data
                                 ,uint8 ack);


#endif	/* MSSP_I2C_H */

