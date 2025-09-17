#include "current_measure.h"

#include <string.h>

void Current_Measure_Init(CurrentMeasure* cm, float lpf_Tf){
  memset(cm, 0, sizeof(CurrentMeasure));
  cm->state = CM_STATE_IDLE;

  LPF_Init(&cm->lpf_ia, lpf_Tf);
  LPF_Init(&cm->lpf_ib, lpf_Tf);
  LPF_Init(&cm->lpf_ic, lpf_Tf);
}

void Current_Measure_Update(CurrentMeasure* cm, float ia, float ib, float ic,
                            float theta){

                            }

void Current_Measure_Calibrate(CurrentMeasure* cm, float ia, float ib,
                               float ic){

                               }