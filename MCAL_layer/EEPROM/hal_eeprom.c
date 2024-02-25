/* 
 * File:   hal_eeprom.c
 * Author: Honey
 *
 * Created on September 22, 2023, 5:01 PM
 */
#include "hal_eeprom.h"

Std_ReturnType hal_read_EEPROM_data_byte(uint16 address ,uint8* data)
{
     Std_ReturnType ret = E_NOT_OK;
    if(address > MAX_SIZE_OF_EPPROM -1 || data == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
     /*Write address in EEADRH:EEADR Registers*/ 
      EEADRH = (address >> 8) & 0x3;
      EEADR  =  (address) & 0xff;
     /*Enable EEPROM Memory Select bits (EPGD = 0 & CFGS = 0)*/
      ACCESS_DATA_EEPROM_MEMORY();
      ENABLE_DATA_EEPROM_MEMORY_SELECT_BIT();
      /*Enable READINF operation (set RD) */
       ENABLE_READ_CONTROL_BIT();
       
       NOP(); /* NOPs may be required for latency at high frequencies */
       NOP(); /* NOPs may be required for latency at high frequencies */
       
       *data = EEDATA;
       ret = E_OK;
    }
     return ret;


}
Std_ReturnType hal_write_EEPROM_data_byte(uint16 address ,uint8 data)
{
    Std_ReturnType ret = E_NOT_OK;
    
    if(address > MAX_SIZE_OF_EPPROM -1){
        ret = E_NOT_OK;
    }
    else
    {
    /*Read the interrupt BIT*/

#if PIRIORITY_ENABLE == PIRIORITY_ON 
    uint8 high_priority_interrpt_status = READ_ALL_HIGH_PRIORITY_BIT() ;  
    uint8 low_priority_interrpt_status = READ_ALL_LOW_PRIORITY_BIT(); 
#else
    uint8 global_interrupt_status = READ_GLOBAL_INTERRUPT_BIT();
    uint8 peripherial_interrupt_status = READ_PERIPHERAL_INTERRUPT_BIT(); 
#endif
    /*Write address in EEADRH:EEADR Registers*/ 
     EEADRH = (address >> 8) & 0x3;
     EEADR  =  (address) & 0xff;
    /*Write data in EEDATA register */
     EEDATA = data; 
    /*Enable EEPROM Memory Select bits (EPGD = 0 & CFGS = 0)*/
     ACCESS_DATA_EEPROM_MEMORY();
     ENABLE_DATA_EEPROM_MEMORY_SELECT_BIT();
    /*Enable write operation (set WREN) */
     ENABLE_EEPROM_WRITRING_OPERATION();
    /*Disable EEPROM interrupt*/
#if PIRIORITY_ENABLE == PIRIORITY_ON 
  DISABLE_ALL_HIGH_PRIORITY();     
  DISABLE_ALL_LOW_PRIORITY();
#else
    DISABLE_GLOBAL_INTERRUPT_BIT();
    DISABLE_PERIPHERAL_INTERRUPT_BIT();
#endif
    /*write 55h to EECON2, then write 0AAh to EECON2 */
     EECON2 = 0x55;
     EECON2 = 0x0AA; 
    /*Enable write control bits (WR bit)*/
     ENABLE_WRITE_CONTROL_BIT(); 
    /*Wait for writing process*/
     while(EECON1bits.WR);
    /*Return interrupt status*/
#if PIRIORITY_ENABLE == PIRIORITY_ON 
   INTCONbits.GIEH =  high_priority_interrpt_status ;
   INTCONbits.GIEL =  low_priority_interrpt_status;
#else
     INTCONbits.GIE = global_interrupt_status;
     INTCONbits.PEIE =  peripherial_interrupt_status; 
#endif
    /*Disable write operation (set WREN) */
     DISABLE_EEPROM_WRITRING_OPERATION();
     ret = E_OK;
   }
    return ret;
}



