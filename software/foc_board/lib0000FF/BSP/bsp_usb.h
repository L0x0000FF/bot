#ifndef BSP_USB_H
#define BSP_USB_H

#include <stdarg.h>
#include <stdint.h>

#include "usb_device.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

void BSP_USB_Init();
void BSP_USB_Loop();
void BSP_USB_AddTxRequestUnblocking(uint8_t* data, uint32_t len);
void BSP_USB_AddTxRequestBlocking(uint8_t* data, uint32_t len);
void BSP_USB_RegisterCallback(usb_rx_callback callback);
void BSP_USB_printf(const char* format,...);


#endif
