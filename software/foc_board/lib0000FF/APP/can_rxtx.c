#include "can_rxtx.h"

#include <stdlib.h>

#define CAN_ID_MYSELF 0x201

CAN_Device main_ctrl;

void app_CAN_rxtx_init() {
  main_ctrl.can_bus_id = 0;
  main_ctrl.device_id = 0x200;
  main_ctrl.rx_buf = 0;
  main_ctrl.tx_buf = 64;
  BSP_CAN_AddFilter(0, 0x200);

  BSP_CAN_RegisterRxCallback(0, CANDevice_rxCallBack);
}

void app_CAN_rxtx_main() {
  BSP_CAN_Send(main_ctrl.can_bus_id, 0x200, &main_ctrl.tx_buf, 1);
	main_ctrl.tx_buf++;
}

void CANDevice_rxCallBack(uint8_t can_id, uint32_t identifier, uint8_t *data,
                          uint32_t len) {
  if (can_id == 0 && identifier == CAN_ID_MYSELF) {
    main_ctrl.rx_buf = *data;
    main_ctrl.tx_buf = *data + 1;
  }
}
