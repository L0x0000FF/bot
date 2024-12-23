#include "bsp_usb.h"

#include <stdio.h>
#include <stdlib.h>

#include "circular_queue.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
circular_queue* USB_txQueue;
typedef struct _usb_tx_pack
{
  uint8_t* pData;
  uint32_t len;
} usb_tx_pack;

usb_rx_callback usbRxCallback;
uint8_t usb_printf_buffer[64];

void BSP_USB_Init() {
  USB_txQueue = create_circular_queue(sizeof(usb_tx_pack), 5);
}

void BSP_USB_Loop(){
  if(USB_txQueue != NULL && USB_txQueue->cq_len > 0){
    usb_tx_pack* tmp = (usb_tx_pack*)circular_queue_front(USB_txQueue);
    BSP_USB_AddTxRequestBlocking(tmp->pData,tmp->len);
    free(tmp->pData);
    circular_queue_pop(USB_txQueue);
  }
}

void BSP_USB_AddTxRequestUnblocking(uint8_t* data, uint32_t len) {
  uint8_t* pbuffer = (uint8_t*)malloc(sizeof(uint8_t) * len);
  usb_tx_pack tmp;
  tmp.len = len;
  tmp.pData = pbuffer;
  memcpy(pbuffer, data, len);
  circular_queue_push(USB_txQueue, &tmp);
}

void BSP_USB_AddTxRequestBlocking(uint8_t* data, uint32_t len) {
  while (CDC_Transmit_FS(data, len) != USBD_OK) {
  }
}

void BSP_USB_RegisterCallback(usb_rx_callback callback) {
  usbRxCallback = callback;
}

void BSP_USB_printf(const char* format, ...) {
  uint8_t len;
  va_list args;
  va_start(args, format);
  len = vsnprintf((char*)usb_printf_buffer, sizeof(usb_printf_buffer),
                  (char*)format, args);
  va_end(args);
  BSP_USB_AddTxRequestBlocking(usb_printf_buffer,len);
}
