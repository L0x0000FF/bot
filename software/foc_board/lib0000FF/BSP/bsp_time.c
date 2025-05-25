#include "bsp_time.h"

#include "main.h"

float bsp_time_s;
uint64_t bsp_time_round;
uint32_t bsp_time_last_cnt;
float bsp_time_cnt2s;

// Use SysTick as time base
uint32_t BSP_sys_time_ms() { return HAL_GetTick(); }

void BSP_time_init() {
  bsp_time_s = 0.0f;
  bsp_time_round = 0;
  uint32_t psc = BSP_TIME_HTIM.Instance->PSC;
  bsp_time_cnt2s = 1.0f / (BSP_TIME_HCLK / (psc + 1));
  HAL_TIM_Base_Start(&BSP_TIME_HTIM);
}

// Make sure to call this function in the a loop whose period > 65535us(ARR)
void BSP_time_update() {
  uint32_t cnt = __HAL_TIM_GET_COUNTER(&BSP_TIME_HTIM);
  if (cnt < bsp_time_last_cnt) {
    bsp_time_round++;
  }

  bsp_time_s = (bsp_time_round * 65536 + cnt) * bsp_time_cnt2s;
  bsp_time_last_cnt = cnt;
}

float BSP_time_s() {
  BSP_time_update();
  return bsp_time_s;
}
