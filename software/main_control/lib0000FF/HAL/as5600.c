#include "as5600.h"
#include "string.h"
#include "stdlib.h"
#include "bsp_time.h"
#include "common.h"

AS5600 as5600;

void AS5600_init(){
  memset(&as5600, 0, sizeof(as5600));
  as5600.i2c_id = 0;
  as5600.status = BSP_I2C_IsAvailable(as5600.i2c_id, AS5600_ADDR);

}
float dt = 0;
void AS5600_update(AS5600 *as5600){
  uint8_t data[2];
  float time = BSP_time_s();
  // read angle
  BSP_I2C_ReadLen(as5600->i2c_id, AS5600_ADDR << 1, AS5600_REG_ANGLE, data, 2);
  uint16_t angle = (data[0] << 8) | data[1];
  BSP_I2C_ReadLen(as5600->i2c_id, AS5600_ADDR << 1, AS5600_REG_RAW_ANGLE, data, 2);
  as5600->raw_angle = (data[0] << 8) | data[1];
  // calc round
  if (abs_i(angle - as5600->angle) > AS5600_MAX_OBERSATION_D_ANGLE) {
    if(angle < as5600->angle){
      as5600->round++;
    }
    else{
      as5600->round--;
    }
  }
  as5600->angle = angle;
  as5600->angle_nobound = as5600->angle + as5600->round * 4096;
  float deg = as5600->angle_nobound * 360.0f / 4096.0f;
  dt = time - as5600->last_update_time;
  as5600->velocity = (deg - as5600->deg) / dt; //(time - as5600->last_update_time);
  as5600->deg = deg;
  as5600->last_update_time = time;
}

void AS5600_update_all(){
  AS5600_update(&as5600);
}
