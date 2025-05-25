#include "bmi088.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "bmi088def.h"
#include "bmi088reg.h"

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim13;
// CS_GRYO PA1
// CS_ACCEL PA4

#define BMI088_CS_ACCEL_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define BMI088_CS_ACCEL_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define BMI088_CS_GYRO_L HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define BMI088_CS_GYRO_H HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)

////BMI088全局配置
#define BMI088_ACCEL_SEN BMI088_ACCEL_3G_SEN
#define BMI088_GYRO_SEN BMI088_GYRO_2000_SEN
#define BMI088_BIAS_INIT_DISCARD 1000
#define BMI088_BIAS_INIT_COUNT 2000
// BMI088加速度计配置数组
static uint8_t BMI088_accel_config[BMI088_WRITE_ACCEL_REG_NUM][3] = {
    {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
    {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE,
     BMI088_ACC_PWR_CONF_ERROR},
    {BMI088_ACC_CONF,
     BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set,
     BMI088_ACC_CONF_ERROR},
    {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, BMI088_ACC_RANGE_ERROR},
    {BMI088_INT1_IO_CTRL,
     BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP |
         BMI088_ACC_INT1_GPIO_LOW,
     BMI088_INT1_IO_CTRL_ERROR},
    {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT,
     BMI088_INT_MAP_DATA_ERROR}

};
// BMI088陀螺仪配置数组
static uint8_t BMI088_gyro_config[BMI088_WRITE_GYRO_REG_NUM][3] = {
    {BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},
    {BMI088_GYRO_BANDWIDTH,
     BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set,
     BMI088_GYRO_BANDWIDTH_ERROR},
    {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},
    {BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},
    {BMI088_GYRO_INT3_INT4_IO_CONF,
     BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW,
     BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},
    {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3,
     BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}

};

void BMI088_delay_us(uint16_t us) {
  htim13.Instance->ARR = 65535;
  HAL_TIM_Base_Start(&htim13);
  while (htim13.Instance->CNT < us * 2);
  HAL_TIM_Base_Stop(&htim13);
}

void BMI088_delay_ms(uint16_t ms) {
  for (uint16_t i = 0; i < ms; i++) {
    BMI088_delay_us(1000);
  }
}

uint8_t BMI088_init(BMI088_IMU *imu) {
  memset(imu, 0, sizeof(BMI088_IMU));
  BMI088_CS_ACCEL_H;
  BMI088_CS_GYRO_H;
  BMI088_delay_ms(1);

  // initialize accelerometer
  // dummy SPI read
  BMI088_accel_SPI_read(BMI088_ACC_CHIP_ID, &(imu->accel_chip_id), 1);
  BMI088_delay_ms(50);
  BMI088_accel_SPI_read(BMI088_ACC_CHIP_ID, &(imu->accel_chip_id), 1);
  if (imu->accel_chip_id == BMI088_ACC_CHIP_ID_VALUE) {
    uint8_t command;
    // configure power
    command = BMI088_ACC_PWR_ACTIVE_MODE;
    BMI088_accel_SPI_write(BMI088_ACC_PWR_CONF, &command, 1);
    // wait for 50ms
    BMI088_delay_ms(50);
    // power up
    command = BMI088_ACC_ENABLE_ACC_ON;
    BMI088_accel_SPI_write(BMI088_ACC_PWR_CTRL, &command, 1);
    // configure accelerometer
    command = BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set;
    BMI088_accel_SPI_write(BMI088_ACC_CONF, &command, 1);
    // set range
    command = BMI088_ACC_RANGE_3G;
    BMI088_accel_SPI_write(BMI088_ACC_RANGE, &command, 1);
    // configure interupt
    command = BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP |
              BMI088_ACC_INT1_GPIO_LOW;
    BMI088_accel_SPI_write(BMI088_INT1_IO_CTRL, &command, 1);
    // configure interupt
    command = BMI088_ACC_INT1_DRDY_INTERRUPT;
    BMI088_accel_SPI_write(BMI088_INT_MAP_DATA, &command, 1);
    imu->accel_ok = 1;
  } else {
    return 1;
  }

  // initialize gyroscope
  BMI088_gyro_SPI_read(BMI088_GYRO_CHIP_ID, &(imu->gyro_chip_id), 1);
  if (imu->gyro_chip_id == BMI088_GYRO_CHIP_ID_VALUE) {
    uint8_t command;
    // set range
    command = BMI088_GYRO_2000;
    BMI088_gyro_SPI_write(BMI088_GYRO_RANGE, &command, 1);
    // set bandwidth
    command = BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set;
    BMI088_gyro_SPI_write(BMI088_GYRO_BANDWIDTH, &command, 1);
    // set lpm
    command = BMI088_GYRO_NORMAL_MODE;
    BMI088_gyro_SPI_write(BMI088_GYRO_LPM1, &command, 1);
    // intrtupt control
    command = BMI088_DRDY_ON;
    BMI088_gyro_SPI_write(BMI088_GYRO_CTRL, &command, 1);
    // configure INT3/INT4
    command = BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW;
    BMI088_gyro_SPI_write(BMI088_GYRO_INT3_INT4_IO_CONF, &command, 1);
    // configure INT3/INT4 IO map
    command = BMI088_GYRO_DRDY_IO_INT3;
    BMI088_gyro_SPI_write(BMI088_GYRO_INT3_INT4_IO_MAP, &command, 1);
    imu->gyro_ok = 1;
  } else {
    return 2;
  }
  MahonyAHRS_init(&(imu->resolver), 2 * 0.1, 2 * 0, 500.0f);

  return 0;
}

void BMI088_update(BMI088_IMU *imu) {
  uint8_t buf[6] = {0};
  float acc_raw[3] = {0};
  float gyro_raw[3] = {0};
  int16_t tmp;

  // 加速度
  BMI088_accel_SPI_read(BMI088_ACCEL_XOUT_L, buf, 6);
  tmp = (int16_t)((buf[1]) << 8) | buf[0];
  acc_raw[0] = tmp * BMI088_ACCEL_SEN;
  tmp = (int16_t)((buf[3]) << 8) | buf[2];
  acc_raw[1] = tmp * BMI088_ACCEL_SEN;
  tmp = (int16_t)((buf[5]) << 8) | buf[4];
  acc_raw[2] = tmp * BMI088_ACCEL_SEN;

  // 陀螺仪
  BMI088_gyro_SPI_read(BMI088_GYRO_X_L, buf, 6);
  tmp = (int16_t)((buf[1]) << 8) | buf[0];
  gyro_raw[0] = tmp * BMI088_GYRO_SEN;
  tmp = (int16_t)((buf[3]) << 8) | buf[2];
  gyro_raw[1] = tmp * BMI088_GYRO_SEN;
  tmp = (int16_t)((buf[5]) << 8) | buf[4];
  gyro_raw[2] = tmp * BMI088_GYRO_SEN;

  for (uint8_t i = 0; i < 3; i++) {
    imu->data.accel[i] = acc_raw[i];
    imu->data.gyro[i] = gyro_raw[i];
    if (!imu->bias_ok && imu->bias_cnt > BMI088_BIAS_INIT_DISCARD) {
      imu->bias[i] += gyro_raw[i];
    } else {
      imu->data.gyro[i] -= imu->bias[i];
    }
  }
  if (!imu->bias_ok) imu->bias_cnt++;

  if (!imu->bias_ok && imu->bias_cnt >= BMI088_BIAS_INIT_COUNT) {
    imu->bias[0] /= BMI088_BIAS_INIT_COUNT;
    imu->bias[1] /= BMI088_BIAS_INIT_COUNT;
    imu->bias[2] /= BMI088_BIAS_INIT_COUNT;
    imu->bias_ok = 1;
  }

  // 温度
  BMI088_accel_SPI_read(BMI088_TEMP_M, buf, 2);
  tmp = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
  if (tmp > 1023) tmp -= 2048;
  imu->temp = tmp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;

  MahonyAHRS_update(&(imu->resolver), imu->data.gyro[0], imu->data.gyro[1],
                    imu->data.gyro[2], imu->data.accel[0], imu->data.accel[1],
                    imu->data.accel[2]);
}

void BMI088_accel_SPI_read(uint8_t reg_addr, uint8_t *data, uint16_t len) {
  reg_addr |= 0x80;
  BMI088_CS_ACCEL_L;
  HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100);
  HAL_SPI_Receive(&hspi1, data, 1, 100);  // read dummy byte
  HAL_SPI_Receive(&hspi1, data, len, 100);
  BMI088_CS_ACCEL_H;
}

void BMI088_accel_SPI_write(uint8_t reg_addr, uint8_t *data, uint16_t len) {
  reg_addr &= 0x7f;
  BMI088_CS_ACCEL_L;
  HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
  HAL_SPI_Transmit(&hspi1, data, len, 100);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
  BMI088_CS_ACCEL_H;
}

void BMI088_gyro_SPI_read(uint8_t reg_addr, uint8_t *data, uint16_t len) {
  reg_addr |= 0x80;
  BMI088_CS_GYRO_L;
  HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100);
  HAL_SPI_Receive(&hspi1, data, len, 100);
  BMI088_CS_GYRO_H;
}

void BMI088_gyro_SPI_write(uint8_t reg_addr, uint8_t *data, uint16_t len) {
  reg_addr &= 0x7f;
  BMI088_CS_GYRO_L;
  HAL_SPI_Transmit(&hspi1, &reg_addr, 1, 100);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
  HAL_SPI_Transmit(&hspi1, data, len, 100);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY);
  BMI088_CS_GYRO_H;
}
