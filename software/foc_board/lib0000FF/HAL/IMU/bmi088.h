#ifndef _BMI088_H
#define _BMI088_H

#include <stdint.h>

#include "imu_data.h"
#include "MahonyAHRS.h"
#include "spi.h"
#include "tim.h"

typedef struct BMI088_imu_t {
  uint8_t bias_ok;
  uint16_t bias_cnt;
  uint8_t accel_chip_id;
  uint8_t gyro_chip_id;
  uint8_t accel_ok;
  uint8_t gyro_ok;
  imu_data data;
  MahonyAHRS resolver;
  float bias[3];
  float gyrobias[3];
  float temp;
} BMI088_IMU;

uint8_t BMI088_init(BMI088_IMU* imu);
void BMI088_update(BMI088_IMU *imu);
void BMI088_accel_SPI_read(uint8_t reg_addr, uint8_t *data, uint16_t len);
void BMI088_accel_SPI_write(uint8_t reg_addr, uint8_t *data, uint16_t len);
void BMI088_gyro_SPI_read(uint8_t reg_addr, uint8_t *data, uint16_t len);
void BMI088_gyro_SPI_write(uint8_t reg_addr, uint8_t *data, uint16_t len);

#endif