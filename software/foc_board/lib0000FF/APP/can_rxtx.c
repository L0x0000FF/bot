#include "can_rxtx.h"

#include <stdlib.h>

#define CAN_ID_CONTROL 0x200
#define CAN_ID_STATUS 0x201

CAN_Device can_device_list[2];
void app_CAN_rxtx_init(float* tx_buf, uint16_t tx_len) {
  can_device_list[0].can_bus_id = 0;
  can_device_list[0].device_id = 0x201;
  can_device_list[0].rx_buf = 0;
  can_device_list[0].tx_buf = tx_buf;
  can_device_list[0].tx_len = tx_len;
  BSP_CAN_AddFilter(0, 0x201);
  BSP_CAN_AddFilter(0, 0x200);

  BSP_CAN_RegisterRxCallback(0, CANDevice_rxCallBack);
}

void app_CAN_rxtx_main() {
  for (uint8_t i = 0; i < 2; i++) {
    BSP_CAN_Send(can_device_list[i].can_bus_id, can_device_list[i].device_id,
                 (uint8_t*)can_device_list[i].tx_buf, can_device_list[i].tx_len * sizeof(float));
  }
}

void CANDevice_rxCallBack(uint8_t can_id, uint32_t identifier, uint8_t *data,
                          uint32_t len) {
  if (identifier == 0x201) {
    memcpy(&can_device_list[0].rx_buf, data, len);
  } else if (identifier == 0x200) {
    memcpy(&can_device_list[1].rx_buf, data, len);
  }
}
