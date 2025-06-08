#include "bsp_gpio.h"

BSP_GPIOTypeDef gpio_ports[GPIO_CNT];

void BSP_GPIO_Init(){
  gpio_ports[0].port = GPIOA;
  gpio_ports[0].pin = GPIO_PIN_2;

  gpio_ports[1].port = GPIOA;
  gpio_ports[1].pin = GPIO_PIN_3;

  gpio_ports[2].port = GPIOB;
  gpio_ports[2].pin = GPIO_PIN_4;

  gpio_ports[3].port = GPIOB;
  gpio_ports[3].pin = GPIO_PIN_5;

  gpio_ports[4].port = GPIOB;
  gpio_ports[4].pin = GPIO_PIN_12;

  gpio_ports[5].port = GPIOB;
  gpio_ports[5].pin = GPIO_PIN_13;

  gpio_ports[5].port = GPIOB;
  gpio_ports[5].pin = GPIO_PIN_14;

  gpio_ports[5].port = GPIOB;
  gpio_ports[5].pin = GPIO_PIN_15;
}

void BSP_GPIO_WritePin(uint8_t gpio_id, uint8_t state){
 HAL_GPIO_WritePin(gpio_ports[gpio_id].port, gpio_ports[gpio_id].pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void BSP_GPIO_TogglePin(uint8_t gpio_id){
HAL_GPIO_TogglePin(gpio_ports[gpio_id].port, gpio_ports[gpio_id].pin);
}

void BSP_GPIO_High(uint8_t gpio_id){
  HAL_GPIO_WritePin(gpio_ports[gpio_id].port, gpio_ports[gpio_id].pin, GPIO_PIN_SET);
}

void BSP_GPIO_Low(uint8_t gpio_id){
  HAL_GPIO_WritePin(gpio_ports[gpio_id].port, gpio_ports[gpio_id].pin, GPIO_PIN_RESET);
}

uint8_t BSP_GPIO_ReadPin(uint8_t gpio_id){
  return HAL_GPIO_ReadPin(gpio_ports[gpio_id].port, gpio_ports[gpio_id].pin);
}