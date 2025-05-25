#ifndef BSP_TIME_H
#define BSP_TIME_H

#include <stdint.h>
#include "tim.h"

#define BSP_TIME_HTIM htim4
#define BSP_TIME_HCLK 72000000

uint32_t BSP_sys_time_ms();
void BSP_time_init();
void BSP_time_update();
float BSP_time_s();

#endif