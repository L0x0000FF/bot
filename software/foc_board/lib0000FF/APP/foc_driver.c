#include "foc_driver.h"

#include "FreeRTOS.h"
#include "as5600.h"
#include "bsp_adc.h"
#include "bsp_can.h"
#include "bsp_i2c.h"
#include "bsp_time.h"
#include "cmsis_os.h"
#include "string.h"
#include "task.h"

extern AS5600 as5600_1;
extern AS5600 as5600_2;

uint16_t ADC_buffer[ADC1_MAX_BUFFER][ADC1_MAX_CHANNEL];
float voltage[ADC1_MAX_CHANNEL];

osThreadId ADCTaskHandle;
osThreadId EncoderTaskHandle;

void BSP_init() {
  BSP_ADC_Init();
  BSP_I2C_Init();
  BSP_CAN_Init();
  BSP_time_init();
  memset(ADC_buffer, 0, sizeof(ADC_buffer));
  BSP_ADC_RegisterConvCpltCallback(0, adc_callback);
  osDelay(2);
  BSP_ADC_Start(0);
}

void communication_init() {}

void adc_callback(uint8_t adc_id, uint16_t* data) {
  memcpy(ADC_buffer, data, sizeof(ADC_buffer));
  BSP_ADC_Start(adc_id);
  // Process the ADC data here
}

void StartADCTask(void const* argument) {
  // TODO: Implement ADC task functionality here
  for (;;) {
    if (BSP_ADC_CvtReady(0)) {
      BSP_ADC_ClearCvtState(0);
      for (uint8_t i = 0; i < ADC1_MAX_CHANNEL; i++) {
        voltage[i] = (float)(ADC_buffer[0][i] + ADC_buffer[1][i]) / 2.0f *
                     3.3f / 4096.0f;
      }

      // Process the ADC data here
    }
    // osDelay(1);
  }
}

void StartEncoderTask(void const* argument) {
  for (;;) {
    AS5600_update_all();
  }
}

void FOC_driver_init() {
  BSP_init();

  communication_init();
  AS5600_init();

  xTaskCreate(StartADCTask, "ADCTask", 128, NULL, osPriorityNormal,
              &ADCTaskHandle);

  xTaskCreate(StartEncoderTask, "EncoderTask", 128, NULL, osPriorityNormal,
              &EncoderTaskHandle);
}

void FOC_driver_main() {}
