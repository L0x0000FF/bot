#ifndef CAN_RXTX_H
#define CAN_RXTX_H

#include <stdint.h>

#include "bsp_can.h"

typedef struct _can_device {
  uint8_t can_bus_id;
  uint32_t device_id;
  float rx_buf[4];
  uint8_t tx_buf;
} CAN_Device;

void app_CAN_rxtx_init();
void app_CAN_rxtx_main();
void CANDevice_rxCallBack(uint8_t can_id, uint32_t identifier, uint8_t* data,
                          uint32_t len);

#endif
