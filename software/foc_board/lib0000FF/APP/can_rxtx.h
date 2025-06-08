#ifndef CAN_RXTX_H
#define CAN_RXTX_H

#include "bsp_can.h"
#include <stdint.h>

typedef struct _can_device {
  uint8_t can_bus_id;
  uint32_t device_id;
  uint8_t rx_buf;
  float* tx_buf;
  uint16_t tx_len;
} CAN_Device;

void app_CAN_rxtx_init(float* tx_buf, uint16_t tx_len);
void app_CAN_rxtx_main();
void CANDevice_rxCallBack(uint8_t can_id, uint32_t identifier, uint8_t *data,
                          uint32_t len);

#endif
