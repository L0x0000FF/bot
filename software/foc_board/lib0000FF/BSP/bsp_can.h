#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "can.h"
#include <stdint.h>
#include "cvector.h"

#define CAN_BUS_CNT 1
/**
 * @brief      :can通信过滤器的最大数目
 * @attention  :此宏定义按照标准帧定义，无需更改
 */
#define FILTER_MAX_CNT (4 * 14)
//can线上能用到的最大的ID号
#define ID_MAX 0x07FF
//未使用can通信过滤器时候的过滤器填充值
#define ID_NOTSET 0x800

typedef void (*can_rx_callback)(uint8_t can_id, uint32_t identifier,
                                uint8_t* data, uint32_t len);

typedef struct BSP_CANTypeDef_t {
  CAN_HandleTypeDef *phcan;  // 自定义总线编号
  uint32_t tx_mailbox;
  cvector *call_backs;
  uint32_t bank_prefix;  // 不同can对应的过滤器相关参数值
  uint16_t filters[FILTER_MAX_CNT];  // 按标准帧ID定义
} BSP_CANTypeDef;

void BSP_CAN_Init();
void BSP_CAN_AddFilter(uint8_t can_id, uint16_t filter);
void BSP_CAN_RemoveFilter(uint8_t can_id, uint16_t filter);
void BSP_CAN_Send(uint8_t can_id, uint16_t identifier, uint8_t* data,
                  uint32_t len);
void BSP_CAN_RegisterRxCallback(uint8_t can_id, can_rx_callback func);

#endif
