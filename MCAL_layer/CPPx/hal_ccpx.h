/* 
 * File:   hal_ccpx.h
 * Author: Honey
 *
 * Created on October 15, 2023, 8:43 PM
 */

#ifndef HAL_CCPX_H
#define	HAL_CCPX_H
/*Section include*/

#include "../INTERRUPT/mcal_internal_interrupt.h"
#include "../GPIO/hal_gpio.h"
#include "pic18f4620.h"
#include "../mcal_std_types.h"
#include "hal_ccpx_cnfg.h"

/*Section Macro declaration*/
#define CCPx_DISABLE_MODULE                                          (uint8)0x00

#define CCPx_PWM_MODE                                                (uint8)0x0c

#define CCPx_COMPARE_MODE_TOGGLE_OUTPUT                              (uint8)0x02
#define CCPx_COMPARE_MODE_SET_OUTPUT                                 (uint8)0x08
#define CCPx_COMPARE_MODE_CLEAR_OUTPUT                               (uint8)0x09
#define CCPx_COMPARE_MODE_GENERATE_INTERRUPT                         (uint8)0x0A
#define CCPx_COMPARE_MODE_RESET_TIMER                                (uint8)0x0B

#define CCPx_CAPTURE_MODE_EVERY_FALLING_EDGE                         (uint8)0x04
#define CCPx_CAPTURE_MODE_EVERY_RISING_EDGE                          (uint8)0x05
#define CCPx_CAPTURE_MODE_EVERY_4RISING_EDGE                         (uint8)0x06
#define CCPx_CAPTURE_MODE_EVERY_16RISING_EDGE                        (uint8)0x07

#define CCPx_Timer1_base_for_CCP1_CCP2                               (uint8)0x00
#define CCPx_Timer3_base_for_CCP1_CCP2                               (uint8)0x01
#define CCPx_Timer1_base_for_CCP1_Timer3_for_CCP2                    (uint8)0x10


/*Section Macro function declaration*/

#define CCP1_SELECT_MODE(_MODE)                                     CCP1CONbits.CCP1M  = _MODE
#define CCP2_SELECT_MODE(_MODE)                                     CCP2CONbits.CCP2M  = _MODE


/*Section Data type declaration*/

typedef union{
    struct{
        uint8 ccpr_low;  /* CCPR1 low register */
        uint8 ccpr_high; /* CCPR1 high register */
    };
    struct{
        uint16 ccpr_16Bit; /* Read CCPR1 as a 16 Bit value */
    };
}CCP_REG_T;

typedef enum{
    ccp1_inst,
    ccp2_inst
}CPPx_inst;


typedef struct{

#if CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE || CCP1_INTERRUPT_CONFG == INTERRUPT_ENABLE
    Interrupt_Handler CCPx_interrupt_handler;
#if PIRIORITY_ENABLE == PIRIORITY_ON
    uint8 priority :1;
#endif
#endif
    CPPx_inst inst;
    pin_config _pin;
    uint8 varient;
#if (CCP1_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED || CCP2_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED)
    uint64 freq;
    uint8 PWM_prescaler;
#else
    uint8 timer_base     :2;
#endif

}CCPx_t;



/*Section fun prototype*/

Std_ReturnType hal_CCPx_initialize(const CCPx_t* ccpx);

/*-----------------------------------------------------------PWM Mode----------------------------------------------------------------------------*/
#if (CCP1_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED || CCP2_CNFG_SELECTED_MODE == CCP_CNFG_PWM_MODE_SELECTED)
Std_ReturnType hal_CCPx_start_PWM(const CCPx_t* ccpx);
Std_ReturnType hal_CCPx_set_duty_cyle(const CCPx_t* ccpx, uint8 val);
Std_ReturnType hal_CCPx_stop_PWM(const CCPx_t* ccpx);
#endif

/*---------------------------------------------------------Capture Mode---------------------------------------------------------------------------*/

#if (CCP1_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED)
 Std_ReturnType hal_CCP1_Read_capture_val(const CCPx_t* ccpx,uint16* val);
Std_ReturnType hal_CCP1_Iscapture_Ready(const CCPx_t* ccpx, uint8* status);
 
#endif
#if (CCP2_CNFG_SELECTED_MODE == CCP_CNFG_CAPTURE_MODE_SELECTED)
Std_ReturnType hal_CCP2_Read_capture_val(const CCPx_t* ccpx,uint16* val);
Std_ReturnType hal_CCP2_Iscapture_Ready(const CCPx_t* ccpx, uint8* status);
 
#endif


#endif	/* HAL_CCPX_H */

