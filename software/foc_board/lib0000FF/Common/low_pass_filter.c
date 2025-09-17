#include "low_pass_filter.h"

void LPF_Init(LowPassFilter* filter, float Tf){
  filter->Tf = Tf;
  filter->last_output = 0.0f;
  filter->last_timestamp = 0;
  filter->in = 0.0f;
}

float LPF_Calc(LowPassFilter* filter, float input, uint32_t timestamp) {
  float dt = (timestamp - filter->last_timestamp) * 1e-6f;

  if (dt < 0.0f)
    dt = 1e-3f;
  else if (dt > 0.3f) {
    filter->last_output = input;
    filter->last_timestamp = timestamp;
    return input;
  }

  float alpha = filter->Tf / (filter->Tf + dt);
  float y = alpha * filter->last_output + (1.0f - alpha) * input;
  filter->last_output = y;
  filter->last_timestamp = timestamp;
  return y;
}
