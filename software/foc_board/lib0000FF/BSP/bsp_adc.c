#include "bsp_adc.h"

#include "string.h"

extern ADC_HandleTypeDef hadc1;
BSP_ADCTypeDef adc_ports[ADC_CNT];

void BSP_ADC_Init() {
  adc_ports[0].phadc = &hadc1;
  adc_ports[0].cvt_ready = 0;
  adc_ports[0].callback = NULL;
  memset(adc_ports[0].buffer, 0, sizeof(adc_ports[0].buffer));
  HAL_ADCEx_Calibration_Start(adc_ports[0].phadc);
}

void BSP_ADC_Start(uint8_t adc_id) {
  HAL_ADC_Start_DMA(adc_ports[adc_id].phadc, (uint32_t*)adc_ports[0].buffer,
                    ADC1_MAX_BUFFER * ADC1_MAX_CHANNEL);
}

void BSP_ADC_Stop(uint8_t adc_id) {
  HAL_ADC_Stop_DMA(adc_ports[0].phadc);
}

void BSP_ADC_RegisterConvCpltCallback(uint8_t adc_id,
                                      adc_callback_conv_cplt_callback func) {
  adc_ports[adc_id].callback = func;
}

uint8_t BSP_ADC_CvtReady(uint8_t adc_id) {
  return adc_ports[adc_id].cvt_ready;
}

void BSP_ADC_ClearCvtState(uint8_t adc_id){
  adc_ports[adc_id].cvt_ready = 0;
}

void HAL_ADC_ConvCpltCallback(
    ADC_HandleTypeDef*
        hadc) {  // 判断转换完成的AD号，同时判断转换完成标志位是否设置
                 // EOC :End Of Conversion
  if ((hadc == &hadc1) &&
      (HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))) {
    BSP_ADC_Stop(0);
    adc_ports[0].cvt_ready = 1;
    if (adc_ports[0].callback != NULL) {
      adc_ports[0].callback(0, adc_ports[0].buffer[0]);
    }
  }
}
