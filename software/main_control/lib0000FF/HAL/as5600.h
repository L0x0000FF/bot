#ifndef _AS5600_H
#define _AS5600_H

#include <stdint.h>
#include "bsp_i2c.h"

#define AS5600_ADDR 0x36
#define AS5600_REG_RAW_ANGLE 0x0C
#define AS5600_REG_ANGLE 0x0E
#define AS5600_REG_MAGNITUDE 0x1A
#define AS5600_REG_STATUS 0x0B
#define AS5600_REG_AGC 0x1B
#define AS5600_REG_CONFIG 0x07
#define AS5600_REG_ZPOS 0x08
#define AS5600_REG_MPOS 0x09
#define AS5600_REG_PWM 0x1C
#define AS5600_MAX_OBERSVATION_RPM 400
#define AS5600_MAX_OBERSATION_D_ANGLE (4096 * AS5600_MAX_OBERSVATION_RPM / 500)

typedef struct AS5600_t {
  uint8_t i2c_id;
  uint8_t status;
  uint16_t raw_angle;
  uint16_t angle;
  int16_t round;
  int32_t angle_nobound;
  float last_update_time;
  float deg;
  float velocity;
} AS5600;

void AS5600_init();
void AS5600_update(AS5600 *as5600);
void AS5600_update_all();

#endif