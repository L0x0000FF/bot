#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdint.h>
#include "gpio.h"

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} BSP_GPIOTypeDef;

#define GPIO_CNT 16

void BSP_GPIO_Init();
void BSP_GPIO_WritePin(uint8_t gpio_id, uint8_t state);
void BSP_GPIO_TogglePin(uint8_t gpio_id);
void BSP_GPIO_High(uint8_t gpio_id);
void BSP_GPIO_Low(uint8_t gpio_id);
uint8_t BSP_GPIO_ReadPin(uint8_t gpio_id);

#endif
