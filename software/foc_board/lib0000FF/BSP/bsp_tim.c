#include "bsp_tim.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

BSP_TIMTypeDef tim_ports[TIM_CNT];

void BSP_TIM_Init() {
  tim_ports[0].phtim = &htim1;
  tim_ports[0].channel = TIM_CHANNEL_1;

  tim_ports[1].phtim = &htim1;
  tim_ports[1].channel = TIM_CHANNEL_2;

  tim_ports[2].phtim = &htim1;
  tim_ports[2].channel = TIM_CHANNEL_3;

  tim_ports[3].phtim = &htim2;
  tim_ports[3].channel = TIM_CHANNEL_1;

  tim_ports[4].phtim = &htim2;
  tim_ports[4].channel = TIM_CHANNEL_2;

  tim_ports[5].phtim = &htim3;
  tim_ports[5].channel = TIM_CHANNEL_1;

  tim_ports[6].phtim = &htim3;
  tim_ports[6].channel = TIM_CHANNEL_2;

  tim_ports[7].phtim = &htim3;
  tim_ports[7].channel = TIM_CHANNEL_3;

  // main frequency is 72MHz
  htim1.Instance->PSC = 2 - 1; // Prescaler for 36MHz
  htim1.Instance->ARR = 3600 - 1; // Auto-reload value for 10kHz
  htim1.Instance->CCR1 = 1200 - 1; // Initial duty cycle
  htim1.Instance->CCR2 = 1800 - 1; // Initial duty cycle
  htim1.Instance->CCR3 = 2400 - 1; // Initial duty cycle

  // Initialize the call_back function as NULL
  for (int i = 0; i < TIM_CNT; ++i) {
    tim_ports[i].call_back = NULL;
  }
  // BSP_TIM_PWM_Start(0); // Start PWM on TIM1 Channel 1
  // BSP_TIM_PWM_Start(1); // Start PWM on TIM1 Channel 2 
  // BSP_TIM_PWM_Start(2); // Start PWM on TIM1 Channel 3
}

uint32_t BSP_TIM_GetCount(uint8_t tim_id) {
  return __HAL_TIM_GET_COUNTER(tim_ports[tim_id].phtim);
}

void BSP_TIM_SetARR(uint8_t tim_id, uint32_t arr) {
  tim_ports[tim_id].phtim->Instance->ARR = arr;
}

void BSP_TIM_SetPrescaler(uint8_t tim_id, uint32_t psc) {
  tim_ports[tim_id].phtim->Instance->PSC = psc;
}

void BSP_TIM_SetCCR(uint8_t tim_id, uint32_t ccr) {
  switch (tim_ports[tim_id].channel) {
    case TIM_CHANNEL_1:
      tim_ports[tim_id].phtim->Instance->CCR1 = ccr;
      break;
    case TIM_CHANNEL_2:
      tim_ports[tim_id].phtim->Instance->CCR2 = ccr;
      break;
    case TIM_CHANNEL_3:
      tim_ports[tim_id].phtim->Instance->CCR3 = ccr;
      break;
    case TIM_CHANNEL_4:
      tim_ports[tim_id].phtim->Instance->CCR4 = ccr;
      break;
    default:
      break;
  }
}

void BSP_TIM_PWM_Start(uint8_t tim_id) {
  HAL_TIM_PWM_Start(tim_ports[tim_id].phtim, tim_ports[tim_id].channel);
}

void BSP_TIM_PWM_StOP(uint8_t tim_id) {
  HAL_TIM_PWM_Stop(tim_ports[tim_id].phtim, tim_ports[tim_id].channel);
}

void BSP_TIM_Start_IT(uint8_t tim_id) {
  HAL_TIM_Base_Start_IT(tim_ports[tim_id].phtim);
}

void BSP_TIM_RegisterCallback(uint8_t tim_id, tim_callback func) {
  tim_ports[tim_id].call_back = func;
}

void BSP_TIM_Callback(TIM_HandleTypeDef *htim) {
  for (int i = 0; i < TIM_CNT; ++i) {
    if (htim == tim_ports[i].phtim) {
      if (tim_ports[i].call_back != NULL) {
        tim_ports[i].call_back(i, NULL, 0);
        break;
      }
    }
  }
}
