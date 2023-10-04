#include "hal_adc.h"

#if ADC_INTERRUPT_ENABLE==INTERRUPT_FRATURE_ENABLE  
static InterruptHandler ADC_InterruptHandler =NULL;
#endif

static inline void adc_input_channel(adc_channel_Select_t channel_select);
static inline void adc_result_format(const adc_confg_t *_adc);
static inline void adc_voltage_reference(const adc_confg_t *_adc);

std_ReturnType ADC_Init(const adc_confg_t *_adc)
{
   std_ReturnType ret =E_NOT_OK;
   if(NULL ==_adc)
   {
       ret =E_NOT_OK;
   }
   else
   {
       /*Disable the ADC*/
        ADC_CONVERTER_DISABLE(); 
       /*configure Acquisition Time*/
        ADCON2bits.ACQT =_adc->acquisition_time;
       /*configure Conversion Clock*/
        ADCON2bits.ADCS =_adc->conversion_clock;
       /*configure Channel*/
        ADCON0bits.CHS =_adc->channel_Select;
        adc_input_channel(_adc->channel_Select); 
       /*configure the interrupt*/
  #if ADC_INTERRUPT_ENABLE==INTERRUPT_FRATURE_ENABLE  
      INTERRUPT_GLOBAL_ENABLE();
      INTERRUPT_PeripheralInterrupt_ENABLE();
      ADC_InterruptEnable();
      ADC_InterruptFlagClear();
#if INTERRUPT_PRIORTY_LEVELS_ENABLE==INTERRUPT_FRATURE_ENABLE
      if(INTERRUPT_HIGH_PRIORTY==_adc->priorty)
      {
       ADC_HighPriortySet();
      }
      else if (INTERRUPT_LOW_PRIORTY==_adc->priorty)
      {
          ADC_LowPriortySet();
      }
      else {/*nothing*/}
#endif      
      ADC_InterruptHandler =_adc->ADC_InterruptHandler;
  #endif
       /*configure the result format */
       adc_result_format(_adc);
       /*configure voltage reference*/
       adc_voltage_reference(_adc);
       /*Enable ADC*/
       ADC_CONVERTER_ENABLE();
     ret =E_OK;   
   }
   return ret;
}
std_ReturnType ADC_DeInit(const adc_confg_t *_adc)
{
   std_ReturnType ret =E_NOT_OK;
   if(NULL ==_adc)
   {
       ret =E_NOT_OK;
   }
   else
   {
       /*Disable the ADC*/
        ADC_CONVERTER_DISABLE(); 
        /*configure the interrupt*/
    #if ADC_INTERRUPT_ENABLE==INTERRUPT_FRATURE_ENABLE  
       ADC_InterruptDisable();
    #endif
       ret =E_OK; 
   }
   return ret;    
}
std_ReturnType ADC_SelectChannel(const adc_confg_t *_adc ,adc_channel_Select_t channel_select)
{
   std_ReturnType ret =E_NOT_OK;
   if(NULL ==_adc)
   {
       ret =E_NOT_OK;
   }
   else
   {
        /*configure Channel*/
        ADCON0bits.CHS =channel_select;
        adc_input_channel(channel_select);
    ret =E_OK;   
   }
   return ret;    
}
std_ReturnType ADC_StartConversion(const adc_confg_t *_adc)
{
   std_ReturnType ret =E_NOT_OK;
   if(NULL ==_adc)
   {
       ret =E_NOT_OK;
   }
   else
   {
       ADC_START_CONVERSION();
       ret =E_OK; 
   }
   return ret;    
}
std_ReturnType ADC_IsConversionDone(const adc_confg_t *_adc ,uint8 *conversion_status)
{
    std_ReturnType ret =E_NOT_OK;
   if((NULL ==_adc) &&(NULL == conversion_status))
   {
       ret =E_NOT_OK;
   }
   else
   {
     *conversion_status =(uint8)(!(ADCON0bits.GO_nDONE));
       ret =E_OK; 
   }
   return ret;   
}
std_ReturnType ADC_GetConversionResult(const adc_confg_t *_adc ,uint16 *conversion_result)
{
   std_ReturnType ret =E_NOT_OK;
   if(NULL ==_adc)
   {
       ret =E_NOT_OK;
   }
   else
   {
     if (_adc->result_format == ADC_RESULT_RIGHT)
    {
       *conversion_result = (uint16)((ADRESH<<8)+(ADRESL));
    }
    else if (_adc->result_format == ADC_RESULT_LEFT)
    {
        *conversion_result = (uint16)(((ADRESH<<8)+(ADRESL))>>6);
    }  
       ret =E_OK; 
   }
   return ret;    
}
std_ReturnType ADC_GetConversion_Blocking(const adc_confg_t *_adc ,adc_channel_Select_t channel ,uint16 *conversion_result)
{
    std_ReturnType ret =E_NOT_OK;
    uint8 l_conversion_status =ZERO_INIT;
    
   if((NULL ==_adc) &&(NULL == conversion_result))
   {
       ret =E_NOT_OK;
   }
   else
   {  
       /*select channel */
       ret =ADC_SelectChannel(_adc, channel);
       /*start conversion*/
       ret =ADC_StartConversion(_adc);
       /*check is conversion completed*/
       while(ADCON0bits.GO_nDONE);
       ret =ADC_GetConversionResult(_adc ,conversion_result);
   }
   return ret;     
}
std_ReturnType ADC_StartConversion_Interrupt(const adc_confg_t *_adc ,adc_channel_Select_t channel)
{
    std_ReturnType ret =E_NOT_OK;
    uint8 l_conversion_status =ZERO_INIT;
    
   if(NULL ==_adc)
   {
       ret =E_NOT_OK;
   }
   else
   {  
       /*select channel */
       ret =ADC_SelectChannel(_adc, channel);
       /*start conversion*/
       ret =ADC_StartConversion(_adc);
   }
   return ret;     
}
static inline void adc_input_channel(adc_channel_Select_t channel_select)
{
 switch (channel_select)
 {
     case ADC_CHANNEL_AN0:  SET_BIT(TRISA ,_TRISA_RA0_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN1:  SET_BIT(TRISA ,_TRISA_RA1_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN2:  SET_BIT(TRISA ,_TRISA_RA2_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN3:  SET_BIT(TRISA ,_TRISA_RA3_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN4:  SET_BIT(TRISA ,_TRISA_RA5_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN5:  SET_BIT(TRISE ,_TRISE_RE0_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN6:  SET_BIT(TRISE ,_TRISE_RE1_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN7:  SET_BIT(TRISE ,_TRISE_RE2_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN8:  SET_BIT(TRISB ,_TRISB_RB2_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN9:  SET_BIT(TRISB ,_TRISB_RB3_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN10: SET_BIT(TRISB ,_TRISB_RB1_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN11: SET_BIT(TRISB ,_TRISB_RB4_POSN); break; /*Disable the digital output driver*/
     case ADC_CHANNEL_AN12: SET_BIT(TRISB ,_TRISB_RB0_POSN); break; /*Disable the digital output driver*/
     default :/*nothing*/;
 }
}

static inline void adc_result_format(const adc_confg_t *_adc)
{
    if (_adc->result_format == ADC_RESULT_RIGHT)
    {
        ADC_RESULT_RIGHT_FORMAT();
    }
    else if (_adc->result_format == ADC_RESULT_LEFT)
    {
        ADC_RESULT_LEFT_FORMAT();
    }
    else {ADC_RESULT_RIGHT_FORMAT();}
}


static inline void adc_voltage_reference(const adc_confg_t *_adc)
{
 if (_adc->voltage_reference == ADC_VOLTAGE_REFERENCE_ENABLE)
 {
     ADC_ENABLE_VOLTAGE_REFERENCE();
 }
 else if (_adc->voltage_reference == ADC_VOLTAGE_REFERENCE_DISABLE)
 {
     ADC_DISABLE_VOLTAGE_REFERENCE();
 }
 else {ADC_DISABLE_VOLTAGE_REFERENCE();}
     
}
void ADC_ISR(void)
{
    ADC_InterruptFlagClear();
    if(ADC_InterruptHandler)
    {
        ADC_InterruptHandler();
    }
}
