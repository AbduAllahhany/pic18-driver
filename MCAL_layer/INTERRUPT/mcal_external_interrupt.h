/* 
 * File:   mcal_external_interrupt.h
 * Author: Honey
 *
 * Created on September 20, 2023, 5:29 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H
//*Section include*/

#include "mcal_interrupt_manger.h"

/*Section Macro declaration*/
/*Section Macro function declaration*/

/*clear INTx interrupts flags*/

#define CLEAR_INT0_FLAG()                          INTCONbits.INT0IF  = 0
#define CLEAR_INT1_FLAG()                          INTCON3bits.INT1IF = 0
#define CLEAR_INT2_FLAG()                          INTCON3bits.INT2IF = 0

/*enable INTx interrupts */

#define ENABLE_INT0_INTERRUPT()                    INTCONbits.INT0IE  = 1                   
#define ENABLE_INT1_INTERRUPT()                    INTCON3bits.INT1IE = 1
#define ENABLE_INT2_INTERRUPT()                    INTCON3bits.INT2IE = 1

/*disable INTx interrupts */

#define DISABLE_INT0_INTERRUPT()                   INTCONbits.INT0IE  = 0                   
#define DISABLE_INT1_INTERRUPT()                   INTCON3bits.INT1IE = 0
#define DISABLE_INT2_INTERRUPT()                   INTCON3bits.INT2IE = 0

/*set INTx interrupts as a rising edge*/

#define ENABLE_INT0_RSISING_EDGE()                 INTCON2bits.INTEDG0 = 1
#define ENABLE_INT1_RSISING_EDGE()                 INTCON2bits.INTEDG1 = 1
#define ENABLE_INT2_RSISING_EDGE()                 INTCON2bits.INTEDG2 = 1


/*set INTx interrupts as a falling edge*/
                                         
#define ENABLE_INT0_FALLING_EDGE()                 INTCON2bits.INTEDG0   = 0
#define ENABLE_INT1_FALLING_EDGE()                 INTCON2bits.INTEDG1   = 0
#define ENABLE_INT2_FALLING_EDGE()                 INTCON2bits.INTEDG2   = 0


/*Clear RBx flag*/

#define CLEAR_RBx_FLAG()                           INTCONbits.RBIF       = 0 

/*Enable RBx interrupt bit*/

#define ENABLE_RBx_INTERRUPT()                     INTCONbits.RBIE       = 1

/*Disable RBx interrupt bit*/

#define DISABLE_RBx_INTERRUPT()                   INTCONbits.RBIE       = 0 



#if PIRIORITY_ENABLE == PIRIORITY_ON   

/*INTx priority */

#define SET_INT1_HIGH_PRIORITY()                INTCON3bits.INT1IP = 1
#define SET_INT1_LOW_PRIORITY()                 INTCON3bits.INT1IP = 0
#define SET_INT2_HIGH_PRIORITY()                INTCON3bits.INT2IP = 1
#define SET_INT2_LOW_PRIORITY()                 INTCON3bits.INT2IP = 0

/*RBx priority*/

#define SET_RBx_HIGH_PRIORITY()                 INTCON2bits.RBIP = 1
#define SET_RBx_LOW_PRIORITY()                  INTCON2bits.RBIP = 0


#endif
/*Section Data type declaration*/

typedef enum{
    INT0_INTERRUPT = 0,
    INT1_INTERRUPT,
    INT2_INTERRUPT
}INTx_source;

typedef struct{
    pin_config _pin;
    Interrupt_Handler _Interrupt_Handler ;
    uint8 interrupt_src   :2;
    uint8 interrupt_edge  :1;
#if PIRIORITY_ENABLE == PIRIORITY_ON    
    uint8 priority       :1;
#endif
}INTx_Interrupt_config;

typedef struct {
    pin_config _pin;
#if PIRIORITY_ENABLE == PIRIORITY_ON    
    uint8 priority       :1;
#endif
    Interrupt_Handler High_change_Interrupt_Handler;
    Interrupt_Handler Low_change_Interrupt_Handler;
}RBx_Interrupt_config;

/*Section fun prototype*/

/*INTx interrupt*/
Std_ReturnType mcal_INTx_initialize(const INTx_Interrupt_config * _INTx );
Std_ReturnType mcal_INTx_deinitialize(const INTx_Interrupt_config * _INTx );

/*RBx interrupt*/
Std_ReturnType mcal_RBx_initialize(const RBx_Interrupt_config * _RBx );
Std_ReturnType mcal_RBx_deinitialize(const RBx_Interrupt_config * _RBx );



#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

