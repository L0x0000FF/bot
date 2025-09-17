#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "adc.h"
#include "cvector.h"
#include "stdint.h"

#define ADC_CNT 1
#define ADC1_MAX_CHANNEL 4
#define ADC1_MAX_BUFFER 2

typedef void (*adc_callback_conv_cplt_callback)(uint8_t adc_id, uint16_t* data);

typedef struct BSP_ADCTypeDef_t {
  ADC_HandleTypeDef* phadc;
  uint16_t buffer[ADC1_MAX_BUFFER][ADC1_MAX_CHANNEL];
  uint8_t cvt_ready;
  adc_callback_conv_cplt_callback callback;
} BSP_ADCTypeDef;

void BSP_ADC_Init();
void BSP_ADC_Start(uint8_t adc_id);
void BSP_ADC_Stop(uint8_t adc_id);
uint8_t BSP_ADC_CvtReady(uint8_t adc_id);
void BSP_ADC_ClearCvtState(uint8_t adc_id);
void BSP_ADC_RegisterConvCpltCallback(uint8_t adc_id,
                                      adc_callback_conv_cplt_callback func);

#endif
