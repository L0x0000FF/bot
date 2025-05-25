#include "usb_test.h"

#include "bsp_usb.h"
#include "string.h"
#include "bmi088.h"
#include "as5600.h"

char test_string1[] = "waiting\n";
char test_string2[] = "good\n";
char rx_buffer[16];
extern BMI088_IMU IMU;
extern AS5600 as5600;

void app_usb_rxCallback(uint8_t* data, uint32_t len) {

}

void app_usb_init() { BSP_USB_RegisterCallback(app_usb_rxCallback); }
void app_usb_loop() {
  BSP_USB_printf("%d,%d,%f\n", as5600.angle, as5600.angle_nobound, as5600.velocity);
}
