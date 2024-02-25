/*
 * File:   mcal_adc.c
 * Author: Honey
 *
 * Created on September 26, 2023, 2:08 AM
 */

#include "mcal_adc.h"
static Std_ReturnType ADC_channel_cnfg(uint8 _channel);

Interrupt_Handler ADC_InterruptHandler = NULL ;
Std_ReturnType mcal_ADC_initialize(const ADC_config *_adc)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
        /*ADC disable*/
         ADC_DISABLE();
        /*Select voltage reference*/
         ADC_SELECT_REF_VOLTAGE(_adc->voltage_reference);
        /*Select result format*/
         ADC_SELECT_JUSTIFION(_adc->result_format);
        /*configure pin*/
         mcal_ADC_select_channel(_adc,_adc->_channel);
        /*configure acquisition time*/
         ADC_SELECT_TAD(_adc->_TAD);
        /*configure frequency*/
         ADC_SELECT_FREQ_CONV(_adc->_FREQ);
        /*configure interrupt*/
#if ADC_INTERRUPT_CONFG == INTERRUPT_ENABLE
#if PIRIORITY_ENABLE == PIRIORITY_ON  
         ENABLE_PRIORITY_BIT();
         ENABLE_ALL_HIGH_PRIORITY();
         ENABLE_ALL_LOW_PRIORITY();
         switch(_adc->_priority)
         {
                 case LOW_PRIORITY:
                     ADC_SET_LOW_PRIORITY();
                     break;
                 case HIGH_PRIORITY:
                     ADC_SET_HIGH_PRIORITY();
                     break;
         }
#else
         ENABLE_PERIPHERAL_INTERRUPT_BIT();
         ENABLE_GLOBAL_INTERRUPT_BIT();
#endif
         ADC_CLEAR_INTERRUPT_FLAG();
         ADC_INTERRUPT_ENABLE();
         ADC_InterruptHandler = _adc->_ADC_handler ;
#endif
        /*Enable ADC*/
         ADC_ENABLE();
    }
    return ret;
}
Std_ReturnType mcal_ADC_deinitialize(const ADC_config *_adc)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ADC_DISABLE();
#if ADC_INTERRUPT_CONFG == INTERRUPT_ENABLE
        ADC_INTERRUPT_DISABLE();
#endif
        ret = E_OK;
    }
    return ret;
}
Std_ReturnType mcal_ADC_select_channel(const ADC_config *_adc, ADC_Channel_Select _channel)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL)
    {
    }
    else
    {
         /*select ADC channel */
         ADC_SELECT_CHANNEL(_adc->_channel);
         /*configure ADC channel as a input*/
         ADC_channel_cnfg(_adc->_channel);

    }
    return ret;
}

Std_ReturnType mcal_ADC_get_result(const ADC_config *_adc, adc_result_t* result)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        if(_adc->result_format == ADC_RIGHT_JUSTIFIED_RES)
        *result = (adc_result_t)((ADRESH << 8) + ADRESL);
        else if(_adc->result_format == ADC_LEFT_JUSTIFIED_RES)
        {
              *result = (adc_result_t)((ADRESL >> 6) +(ADRESH << 2)); 
        }
        else
        { 
           *result = (adc_result_t)((ADRESH << 8) + ADRESL);

        }
    }
    return ret;
}

Std_ReturnType mcal_ADC_start_conversion_blocking(const ADC_config *_adc,ADC_Channel_Select _channel , adc_result_t* result)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL || result == NULL)
    {
        ret = E_NOT_OK; 
    }
    else
    {
        /*select channel*/
        ret = mcal_ADC_select_channel(_adc,_channel);
        /*set the go bit*/
        ADC_START_CONVERSION();
        while( ADCON0bits.GO);
        ret = mcal_ADC_get_result(_adc, result);

    }
    return ret;
}

#if ADC_INTERRUPT_CONFG == INTERRUPT_ENABLE

Std_ReturnType mcal_ADC_start_conversion_interrupt(const ADC_config *_adc,ADC_Channel_Select _channel)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL)
    {
        ret = E_NOT_OK;
    }
    else
    {
        ADC_CLEAR_INTERRUPT_FLAG();
        /*select channel*/
        ret = mcal_ADC_select_channel(_adc,_channel);
        /*set the go bit*/
        ADC_START_CONVERSION();

    }
    return ret;
}

#endif
/* 1 ref to conversion_done
 * 0 ref to else 
 */
Std_ReturnType mcal_ADC_is_conversion_done(const ADC_config *_adc, uint8 *conversion_status)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_adc == NULL)
    {
         ret = E_NOT_OK;
    }
    else
    {
        *conversion_status = !(ADC_GET_CONVERSION_STATUS());
    }
    return ret;

}

static Std_ReturnType ADC_channel_cnfg(uint8 _channel)
{
    Std_ReturnType ret = E_NOT_OK;
    if(_channel > MAX_Channels)
    {
        ret = E_NOT_OK;
    }
    else
    {
        switch (_channel){
             case  ADC_CHANNEL_AN0:
                 SET_BIT(TRISA,PIN0);
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN0_CHANNELS_ANALOG);
                 break;
             case  ADC_CHANNEL_AN1:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN1_CHANNELS_ANALOG);
                 SET_BIT(TRISA,PIN1);
                 break;
             case  ADC_CHANNEL_AN2:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN2_CHANNELS_ANALOG);
                 SET_BIT(TRISA,PIN2);
                 break;
             case  ADC_CHANNEL_AN3:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN3_CHANNELS_ANALOG);
                 SET_BIT(TRISA,PIN3);
                 break;
             case  ADC_CHANNEL_AN4:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN4_CHANNELS_ANALOG);
                 SET_BIT(TRISA,PIN5);
                 break;
             case  ADC_CHANNEL_AN5:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN5_CHANNELS_ANALOG);
                 SET_BIT(TRISE,PIN0);
                 break;
             case  ADC_CHANNEL_AN6:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN6_CHANNELS_ANALOG);
                 SET_BIT(TRISE,PIN1);
                 break;
             case  ADC_CHANNEL_AN7:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN7_CHANNELS_ANALOG);
                 SET_BIT(TRISE,PIN2);
                 break;
             case  ADC_CHANNEL_AN8:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN8_CHANNELS_ANALOG);
                 SET_BIT(TRISB,PIN2);
                 break;
             case  ADC_CHANNEL_AN9:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN9_CHANNELS_ANALOG);
                  SET_BIT(TRISB,PIN3);
                 break;
             case  ADC_CHANNEL_AN10:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN10_CHANNELS_ANALOG);
                  SET_BIT(TRISB,PIN1);
                 break;
             case  ADC_CHANNEL_AN11:
                 ADC_SELECT_CNFG_CHANNELS(ADC_AN11_CHANNELS_ANALOG);
                  SET_BIT(TRISB,PIN4);
                 break;
             case  ADC_CHANNEL_AN12:
                  ADC_SELECT_CNFG_CHANNELS(ADC_AN12_CHANNELS_ANALOG);
                  SET_BIT(TRISB,PIN0);
                 break;
        }
    }
    return ret;
}

void ADC_ISR(void){
    ADC_CLEAR_INTERRUPT_FLAG();
    if(ADC_InterruptHandler){
        ADC_InterruptHandler();
    }
}





