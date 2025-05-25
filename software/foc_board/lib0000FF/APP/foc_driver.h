#ifndef FOC_DRIVER_H
#define FOC_DRIVER_H

#include "stdint.h"

void FOC_driver_init();
void FOC_driver_main();
void adc_callback(uint8_t adc_id, uint16_t* data);

#endif