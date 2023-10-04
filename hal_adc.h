

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/*** SEction : Includes ***/

#include "../mcal_std_types.h"
#include "pic18f4620.h"
#include "../Interrupt/mcal_internal_interrupt.h"
#include "hal_adc_cfg.h"
#include "../GPIO/hal_gpio.h"

/*** SEction : Macros Decleration ***/

#define ADC_AN0_ANALOG_FUNCTIONALTY  0x0E
#define ADC_AN1_ANALOG_FUNCTIONALTY  0x0D
#define ADC_AN2_ANALOG_FUNCTIONALTY  0x0C
#define ADC_AN3_ANALOG_FUNCTIONALTY  0x0B
#define ADC_AN4_ANALOG_FUNCTIONALTY  0x0A
#define ADC_AN5_ANALOG_FUNCTIONALTY  0x09
#define ADC_AN6_ANALOG_FUNCTIONALTY  0x08
#define ADC_AN7_ANALOG_FUNCTIONALTY  0x07
#define ADC_AN8_ANALOG_FUNCTIONALTY  0x06
#define ADC_AN9_ANALOG_FUNCTIONALTY  0x05
#define ADC_AN10_ANALOG_FUNCTIONALTY 0x04
#define ADC_AN11_ANALOG_FUNCTIONALTY 0x03
#define ADC_AN12_ANALOG_FUNCTIONALTY 0x02
#define ADC_ALL_ANALOG_FUNCTIONALTY  0x00
#define ADC_ALL_DIGITAL_FUNCTIONALTY 0x0F

#define ADC_RESULT_RIGHT  0x01U /*A/D Result Format Select Right*/
#define ADC_RESULT_LEFT   0x00U /*A/D Result Format Select left*/

#define ADC_VOLTAGE_REFERENCE_DISABLE 0x00U /**voltage reference disable*/
#define ADC_VOLTAGE_REFERENCE_ENABLE  0x01U /**voltage reference enable*/

#define ADC_CONVERSION_ENABLE  1
#define ADC_CONVERSION_DISABLE 0
/**********A/D Conversion Status bit  conversion in progress OR  Idle -> GO/DONE*****/
#define  ADC_CONVERSION_STATUS() (ADCON0bits.GO_nDONE)
/*A/D Conversion Status bit conversion is start */
#define  ADC_START_CONVERSION() (ADCON0bits.GO_nDONE =1)

/**********A/D On bit Enable or Disable => ADON**********/
#define  ADC_CONVERTER_ENABLE()  (ADCON0bits.ADON =1) 
#define  ADC_CONVERTER_DISABLE()   (ADCON0bits.ADON =0)
/* Voltage Reference Configuration bit Enable  :Vref- ,Vref+
   Voltage Reference Configuration bit Disable :VSS ,VDD  
 */
#define  ADC_ENABLE_VOLTAGE_REFERENCE()   do{ADCON1bits.VCFG0 =1;\
                                             ADCON1bits.VCFG1 =1;\
                                            }while(0)
#define  ADC_DISABLE_VOLTAGE_REFERENCE()  do{ADCON1bits.VCFG0 =0;\
                                             ADCON1bits.VCFG1 =0;\
                                            }while(0)

/*** SEction : Macros Function ***/
#define ADC_ANALOG_DIGITAL_PORT_CONFIGRATION(_CONFIG)  (ADCON1bits.PCFG =_CONFIG)
/***** A/D Result Format Select Right OR Left bit ->ADFM*****/
#define ADC_RESULT_RIGHT_FORMAT()                         (ADCON2bits.ADFM =1)
#define ADC_RESULT_LEFT_FORMAT()                          (ADCON2bits.ADFM =0)

/*** SEction : Data Types Decleration ***/

/*****Analog Channel Select bits******/
typedef enum {
    ADC_CHANNEL_AN0 =0,
    ADC_CHANNEL_AN1 ,
    ADC_CHANNEL_AN2 ,
    ADC_CHANNEL_AN3 ,            
    ADC_CHANNEL_AN4 ,
    ADC_CHANNEL_AN5 ,
    ADC_CHANNEL_AN6 ,
    ADC_CHANNEL_AN7 ,
    ADC_CHANNEL_AN8 ,
    ADC_CHANNEL_AN9 ,
    ADC_CHANNEL_AN10 ,
    ADC_CHANNEL_AN11 ,
    ADC_CHANNEL_AN12 
} adc_channel_Select_t;

/**********************A/D Acquisition Time Select bits -> ACQT2:ACQT0**************/
typedef enum {
    ADC_0_TAD =0,
    ADC_2_TAD ,
    ADC_4_TAD ,
    ADC_6_TAD ,
    ADC_8_TAD ,
    ADC_12_TAD ,
    ADC_16_TAD ,
    ADC_20_TAD         
}adc_acquisition_time_t;

/******************* A/D Conversion Clock Select bits ->ADCS2:ADCS0************/
typedef enum {
    ADC_CONVERSION_CLOCK_FOSC_DIV_2 =0,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8 ,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32 ,
    ADC_CONVERSION_CLOCK_FOSC_DIV_FRC ,
    ADC_CONVERSION_CLOCK_FOSC_DIV_4 , 
    ADC_CONVERSION_CLOCK_FOSC_DIV_16 ,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64   
}adc_conversion_clock_t;

typedef struct {
#if ADC_INTERRUPT_ENABLE==INTERRUPT_FRATURE_ENABLE    
    void (* ADC_InterruptHandler)(void);
    interrupt_priorty_cfg priorty;
#endif    
    adc_acquisition_time_t acquisition_time;
    adc_conversion_clock_t conversion_clock;
    adc_channel_Select_t   channel_Select;
    uint8 voltage_reference :1;   /*Voltage Reference Configuration bit Enable or Disable: Vref-,Vref+,VSS,VDD*/ 
    uint8 result_format :1 ;      /*A/D Result Format Select Right OR Left bit ->ADFM*/ 
    uint8 ADC_Reserved :6;    
}adc_confg_t;

/*** SEction : Fuction Decleration ***/
std_ReturnType ADC_Init(const adc_confg_t *_adc);
std_ReturnType ADC_DeInit(const adc_confg_t *_adc);
std_ReturnType ADC_SelectChannel(const adc_confg_t *_adc ,adc_channel_Select_t channel_select);
std_ReturnType ADC_StartConversion(const adc_confg_t *_adc);
std_ReturnType ADC_IsConversionDone(const adc_confg_t *_adc ,uint8 *conversion_status);
std_ReturnType ADC_GetConversionResult(const adc_confg_t *_adc ,uint16 *conversion_result);
std_ReturnType ADC_GetConversion_Blocking(const adc_confg_t *_adc ,adc_channel_Select_t channel 
                                                                     ,uint16 *conversion_status);
std_ReturnType ADC_StartConversion_Interrupt(const adc_confg_t *_adc ,adc_channel_Select_t channel);
#endif	/* HAL_ADC_H */

