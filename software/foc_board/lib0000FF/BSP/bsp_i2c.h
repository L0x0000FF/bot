#ifndef BSP_I2C_H
#define BSP_I2C_H

#include "i2c.h"
#include <stdint.h>
#include "cvector.h"

#define I2C_BUS_CNT 2

void BSP_I2C_Init();
uint8_t BSP_I2C_GetDeviceList(uint8_t i2c_id, uint8_t *dev_list);
uint8_t BSP_I2C_IsAvailable(uint8_t i2c_id, uint8_t dev_addr);
void BSP_I2C_ReadByte(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                      uint8_t *data);
void BSP_I2C_ReadLen(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                     uint8_t *data, uint32_t len);
void BSP_I2C_WriteByte(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                      uint8_t *data);
void BSP_I2C_WriteLen(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                     uint8_t *data, uint32_t len);

#endif
