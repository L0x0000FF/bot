#include "bsp_i2c.h"

#include "bsp_time.h"

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

uint8_t i2c1_dev_list[128];
uint8_t i2c2_dev_list[128];
uint8_t i2c1_dev_cnt;
uint8_t i2c2_dev_cnt;

void BSP_I2C_Init() {
  for (uint8_t i = 0; i < 128; i++) {
    i2c1_dev_list[i] = 0;
    i2c2_dev_list[i] = 0;
  }
  i2c1_dev_cnt = BSP_I2C_GetDeviceList(0, i2c1_dev_list);
  i2c2_dev_cnt = BSP_I2C_GetDeviceList(1, i2c2_dev_list);
}

uint8_t BSP_I2C_GetDeviceList(uint8_t i2c_id, uint8_t *dev_list) {
  I2C_HandleTypeDef *phi2c;
  if (i2c_id == 0) {
    phi2c = &hi2c1;
  } else if (i2c_id == 1) {
    phi2c = &hi2c2;
  } else {
    return 0;
  }
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < 128; i++) {
    if (HAL_I2C_IsDeviceReady(phi2c, i << 1, 1, 10) == HAL_OK) {
      dev_list[i] = 1;
      cnt++;
    } else
      dev_list[i] = 0;
  }
  return cnt;
}

uint8_t BSP_I2C_IsAvailable(uint8_t i2c_id, uint8_t dev_addr) {
  if (i2c_id == 0) {
    return i2c1_dev_list[dev_addr];
  } else if (i2c_id == 1) {
    return i2c2_dev_list[dev_addr];
  } else {
    return 0;
  }
}

void BSP_I2C_ReadByte(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                      uint8_t *data) {
  I2C_HandleTypeDef *phi2c;
  if (i2c_id == 0) {
    phi2c = &hi2c1;
  } else if (i2c_id == 1) {
    phi2c = &hi2c2;
  } else {
    return;
  }
  HAL_I2C_Mem_Read(phi2c, dev_addr, reg_addr, 1, data, 1, 1000);
}

void BSP_I2C_ReadLen(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                     uint8_t *data, uint32_t len) {
  I2C_HandleTypeDef *phi2c;
  if (i2c_id == 0) {
    phi2c = &hi2c1;
  } else if (i2c_id == 1) {
    phi2c = &hi2c2;
  } else {
    return;
  }
  HAL_I2C_Mem_Read(phi2c, dev_addr, reg_addr, 1, data, len, 1000);
}

void BSP_I2C_WriteByte(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                       uint8_t *data) {
  I2C_HandleTypeDef *phi2c;
  if (i2c_id == 0) {
    phi2c = &hi2c1;
  } else if (i2c_id == 1) {
    phi2c = &hi2c2;
  } else {
    return;
  }
  HAL_I2C_Mem_Write(phi2c, dev_addr, reg_addr, 1, data, 1, 1000);
}

void BSP_I2C_WriteLen(uint8_t i2c_id, uint8_t dev_addr, uint8_t reg_addr,
                      uint8_t *data, uint32_t len) {
  I2C_HandleTypeDef *phi2c;
  if (i2c_id == 0) {
    phi2c = &hi2c1;
  } else if (i2c_id == 1) {
    phi2c = &hi2c2;
  } else {
    return;
  }
  HAL_I2C_Mem_Write(phi2c, dev_addr, reg_addr, 1, data, len, 1000);
}
