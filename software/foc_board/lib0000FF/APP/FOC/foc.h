#ifndef FOC_H
#define FOC_H

#include "stdint.h"
#include "as5600.h"
#include "bsp_tim.h"
#include "bsp_adc.h"
#include "bsp_gpio.h"

typedef struct FOC_Config {
  AS5600* encoder;
  uint8_t adc_id[3];
  uint8_t pwm_id[3];
  uint8_t gpio_en_id;
  uint8_t gpio_reset_id;
  uint8_t gpio_fault_id;
  uint8_t gpio_sleep_id;
  float VM;
} FOC_Config;

typedef struct FOC_{
  float voltage[3];
  float current[3];
  float angle;
  float speed;
  uint8_t mode;
  uint8_t state;
  FOC_Config config;
} FOC;


void FOC_init(FOC* foc, FOC_Config* config);
void FOC_update(FOC* foc);



#endif
