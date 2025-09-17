#ifndef CURRENT_MEASURE_H
#define CURRENT_MEASURE_H

#include <stdint.h>
#include "vectors.h"
#include "low_pass_filter.h"

typedef enum {
  CM_STATE_IDLE = 0,
  CM_STATE_CALIBRATING,
  CM_STATE_READY
} CurrentMeasureState;

typedef struct CurrentMeasure_t {
  CurrentMeasureState state;
  Vector3f I_abc;      // 三相电流
  Vector3f I_abc_bias; // 三相电流偏置
  Vector2f I_alphabeta; // αβ静止坐标系电流
  Vector2f I_dq;       // dq旋转坐标系电流
  float I;             // 电流幅值
  float I_offset;      // 电流偏置
  LowPassFilter lpf_ia;
  LowPassFilter lpf_ib;
  LowPassFilter lpf_ic;
} CurrentMeasure;

void Current_Measure_Init(CurrentMeasure* cm, float lpf_Tf);
void Current_Measure_Update(CurrentMeasure* cm, float ia, float ib, float ic, float theta);
void Current_Measure_Calibrate(CurrentMeasure* cm, float ia, float ib, float ic);

#endif
