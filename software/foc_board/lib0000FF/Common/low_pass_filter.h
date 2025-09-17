#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include <stdint.h>

typedef struct {
  uint32_t last_timestamp;
  float last_output;
  float Tf;
  float in;
}LowPassFilter;

void LPF_Init(LowPassFilter* filter, float Tf);
float LPF_Calc(LowPassFilter* filter, float input, uint32_t timestamp);

#endif
