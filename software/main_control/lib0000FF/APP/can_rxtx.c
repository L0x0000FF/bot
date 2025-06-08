#include "can_rxtx.h"

#include <stdlib.h>

#include "bsp_usb.h"

#define CAN_ID_MYSELF 0x200

CAN_Device can_device_list[2];
void app_CAN_rxtx_init() {
  can_device_list[0].can_bus_id = 0;
  can_device_list[0].device_id = 0x201;
  // can_device_list[0].rx_buf = 0;
  can_device_list[0].tx_buf = 64;
  BSP_CAN_AddFilter(0, 0x201);

  can_device_list[1].can_bus_id = 1;
  can_device_list[1].device_id = 0x201;
  // can_device_list[1].rx_buf = 0;
  can_device_list[1].tx_buf = 0;
  BSP_CAN_AddFilter(1, 0x201);
  BSP_CAN_RegisterRxCallback(0, CANDevice_rxCallBack);
  BSP_CAN_RegisterRxCallback(1, CANDevice_rxCallBack);
}

void app_CAN_rxtx_main() {
  for (uint8_t i = 0; i < 2; i++) {
    BSP_CAN_Send(can_device_list[i].can_bus_id, can_device_list[i].device_id,
                 &can_device_list[i].tx_buf, 1);
    can_device_list[i].tx_buf--;
  }
}

void CANDevice_rxCallBack(uint8_t can_id, uint32_t identifier, uint8_t *data,
                          uint32_t len) {
  if (can_id == 0 && identifier == 0x201) {
    memcpy(can_device_list[0].rx_buf, data, 4 * sizeof(float));
  } else if (can_id == 1 && identifier == 0x200) {
    memcpy(can_device_list[1].rx_buf, data, 4 * sizeof(float));
  }
}
