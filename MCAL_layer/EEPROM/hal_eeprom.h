/* 
 * File:   hal_eeprom.h
 * Author: Honey
 *
 * Created on September 22, 2023, 5:01 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/*Section include*/

#include "../mcal_std_types.h"
#include "pic18f4620.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/* Section Macro declaration*/
#define MAX_SIZE_OF_EPPROM                                        1024

/*Section Macro function declaration*/

/* Enable EEPROM Memory Select bits*/
#define ACCESS_DATA_EEPROM_MEMORY()                               EECON1bits.EEPGD = 0
#define ENABLE_DATA_EEPROM_MEMORY_SELECT_BIT()                    EECON1bits.CFGS  = 0

/*Enable write operation*/   
#define ENABLE_EEPROM_WRITRING_OPERATION()                        EECON1bits.WREN  = 1

/*Disable write operation*/   
#define DISABLE_EEPROM_WRITRING_OPERATION()                       EECON1bits.WREN  = 0

/* Enable write control bits 
 * It is automatically cleared in hardware 
 * WR is set during writing operation
 * when it is zero the write operation is completed
 */

#define ENABLE_WRITE_CONTROL_BIT()                                EECON1bits.WR    = 1

/*Enable read Control bit 
 * It is automatically cleared in hardware 
 * when RD is set the EEPROM is to read
 */

#define ENABLE_READ_CONTROL_BIT()                                 EECON1bits.RD    = 1


/*
 *  Read data error flag 
 *  The WRERR bit is set in hardware when the WREN bit is set
 *  and cleared when the internal programming timer expires
 *  and the write operation is complete. 
 */
#define READ_DATA_EEPROM_ERROR_FLAG()                             EECON1bits.WRERR


/*Section Data type declaration*/
                                        
/*Section fun prototype*/

Std_ReturnType hal_read_EEPROM_data_byte(uint16 address ,uint8 *data);
Std_ReturnType hal_write_EEPROM_data_byte(uint16 address ,uint8 data);



#endif	/* HAL_EEPROM_H */

