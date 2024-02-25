/* 
 * File:   mcal_adc.h
 * Author: Honey
 *
 * Created on September 26, 2023, 2:08 AM
 */

#ifndef MCAL_ADC_H
#define	MCAL_ADC_H

/*Section include*/

#include "../mcal_std_types.h"
#include "mcal_adc_cnfg.h"
#include "../GPIO/hal_gpio.h"
#include "pic18f4620.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"
/*Section Macro declaration*/
#define MAX_Channels                                0x0d


#define ADC_ALL_CHANNELS_ANALOG                     0x02
#define ADC_AN12_CHANNELS_ANALOG                    0x02
#define ADC_AN11_CHANNELS_ANALOG                    0x03
#define ADC_AN10_CHANNELS_ANALOG                    0x04
#define ADC_AN9_CHANNELS_ANALOG                     0x05
#define ADC_AN8_CHANNELS_ANALOG                     0x06
#define ADC_AN7_CHANNELS_ANALOG                     0x07
#define ADC_AN6_CHANNELS_ANALOG                     0x08
#define ADC_AN5_CHANNELS_ANALOG                     0x09
#define ADC_AN4_CHANNELS_ANALOG                     0x0A
#define ADC_AN3_CHANNELS_ANALOG                     0x0B
#define ADC_AN2_CHANNELS_ANALOG                     0x0C
#define ADC_AN1_CHANNELS_ANALOG                     0x0D
#define ADC_AN0_CHANNELS_ANALOG                     0x0E
#define ADC_ALL_CHANNELS_DIGITAL                    0x0F


#define ADC_EXTERNAL_VOLTAGE_REF                    0x01
#define ADC_INTERNAL_VOLTAGE_REF                    0x00

#define ADC_RIGHT_JUSTIFIED_RES                     0x01
#define ADC_LEFT_JUSTIFIED_RES                      0x00




/*Section Macro function declaration*/

/*Enable & disable ADC module*/
#define ADC_ENABLE()                                 ADCON0bits.ADON = 1
#define ADC_DISABLE()                                ADCON0bits.ADON = 0

/*ADC start Conversion*/
#define ADC_START_CONVERSION()                       ADCON0bits.GO = 1 
#define ADC_GET_CONVERSION_STATUS()                  ADCON0bits.GO_DONE

/*ADC select ref voltage*/
#define ADC_SELECT_REF_VOLTAGE(_REF)                  do{ ADCON1bits.VCFG0 = _REF;\
                                                       ADCON1bits.VCFG1 = _REF;   \
                                                      }while(0);

/*ADC select analog & digital channels */
#define ADC_SELECT_CNFG_CHANNELS(_CNFG)             ADCON1bits.PCFG = _CNFG   

/*ADC select channels*/
#define ADC_SELECT_CHANNEL(_CHANNEL)                ADCON0bits.CHS  = _CHANNEL

/*ADC result position*/
#define ADC_SELECT_JUSTIFION(_DIR)                  ADCON2bits.ADFM = _DIR

/*select conversion frequency*/
#define ADC_SELECT_FREQ_CONV(_FREQ)                 ADCON2bits.ADCS = _FREQ
/*select acquisition time*/
#define ADC_SELECT_TAD(_TAD)                        ADCON2bits.ACQT = _TAD

/*Section Data type declaration*/

typedef enum{
   ADC_FOSC2,
   ADC_FOSC8,
   ADC_FOSC32,
   ADC_FRC,
   ADC_FOSC4,
   ADC_FOSC16,
   ADC_FOSC64
}ADC_conversion_clock_select;

typedef enum{
    ADC_MANUAL_TAD,
    ADC_2TAD,
    ADC_4TAD,
    ADC_6TAD,
    ADC_8TAD,
    ADC_12TAD,
    ADC_16TAD,
    ADC_20TAD,
}ADC_acquisition_time_select;

typedef enum{
    ADC_CHANNEL_AN0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12         
}ADC_Channel_Select;

typedef struct{
#if ADC_INTERRUPT_CONFG == INTERRUPT_ENABLE
   Interrupt_Handler _ADC_handler;
#if PIRIORITY_ENABLE == PIRIORITY_ON  
   interrupt_priority _priority;
#endif
#endif
   ADC_Channel_Select _channel;
   ADC_acquisition_time_select _TAD;
   ADC_conversion_clock_select  _FREQ;
   uint8 voltage_reference : 1;
   uint8 result_format : 1;
   uint8 ADC_Reserved : 6;
}ADC_config;
                      
typedef uint16 adc_result_t;

/*Section fun prototype*/

Std_ReturnType mcal_ADC_initialize(const ADC_config *_adc);
Std_ReturnType mcal_ADC_deinitialize(const ADC_config *_adc);
Std_ReturnType mcal_ADC_select_channel(const ADC_config *_adc, ADC_Channel_Select _channel);
Std_ReturnType mcal_ADC_get_result(const ADC_config *_adc, adc_result_t* result);
Std_ReturnType mcal_ADC_start_conversion_blocking(const ADC_config *_adc,ADC_Channel_Select _channel ,adc_result_t* result);
Std_ReturnType mcal_ADC_start_conversion_interrupt(const ADC_config *_adc,ADC_Channel_Select _channel);
Std_ReturnType mcal_ADC_is_conversion_done(const ADC_config *_adc, uint8 * conversion_status);








#endif	/* MCAL_ADC_H */

