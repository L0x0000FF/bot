#include "bsp_i2c.h"

#include "bsp_time.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

BSP_I2CTypeDef can_bus[CAN_BUS_CNT];

void BSP_I2C_Init() {}

uint8_t BSP_I2C_GetDeviceList(uint8_t i2c_id, uint8_t *dev_list) {}
void BSP_I2C_ReadByte(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                      uint8_t *data) {}
void BSP_I2C_ReadLen(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                     uint8_t *data, uint32_t len) {}
void BSP_I2C_WriteByte(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                       uint8_t *data) {}
void BSP_I2C_WriteLen(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                      uint8_t *data, uint32_t len) {}
