#ifndef BSP_TIM_H
#define BSP_TIM_H

#include <stdint.h>

#include "cvector.h"
#include "tim.h"

#define TIM_MAIN_FREQ 72000000

#define TIM_CNT 8

typedef void (*tim_callback)(uint8_t tim_id, uint8_t* data, uint32_t len);

typedef struct BSP_TIMTypeDef_t {
  TIM_HandleTypeDef* phtim;  // 自定义总线编号
  uint32_t channel;
  tim_callback call_back;
} BSP_TIMTypeDef;

void BSP_TIM_Init();
uint32_t BSP_TIM_GetCount(uint8_t tim_id);
void BSP_TIM_SetARR(uint8_t tim_id, uint32_t arr);
void BSP_TIM_SetPrescaler(uint8_t tim_id, uint32_t psc);
void BSP_TIM_SetCCR(uint8_t tim_id, uint32_t ccr);
void BSP_TIM_PWM_Start(uint8_t tim_id);
void BSP_TIM_PWM_StOP(uint8_t tim_id);
void BSP_TIM_Start_IT(uint8_t tim_id);
void BSP_TIM_RegisterCallback(uint8_t tim_id, tim_callback func);
void BSP_TIM_Callback(TIM_HandleTypeDef* htim);

#endif
