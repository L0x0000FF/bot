#include "buzzer.h"

Buzzer_t buzzer;

uint8_t test[] = {DO, RE, MI, FA, SO, LA, SI, DO2};

void buzzer_init() {
  buzzer.state = 0;
  buzzer.bpm = 0;
  buzzer.note_index = 0;
  buzzer.music = NULL;
  BSP_TIM_RegisterCallback(BUZZER_TIM_ID, buzzer_next_note);
}

void buzzer_start(uint8_t* music, uint8_t bpm, uint8_t beat) {
  if (buzzer.state == 1 || music == NULL) {
    return;
  }
  buzzer.state = 1;
  buzzer.note_index = 0;
  buzzer.music = music;
  if (bpm == 0 || beat == 0) {
    buzzer.bpm = 60;
    buzzer.beat = beat4;
  } else {
    buzzer.bpm = bpm;
    buzzer.beat = beat;
  }
  // duration = 60 / bpm / beat = 1 / (MAIN_FREQ / PSC / ARR)
  // PSC = MAIN_FREQ * duration / ARR
  float duration = 60.0f / buzzer.bpm / buzzer.beat;
  BSP_TIM_SetARR(BUZZER_TIM_ID, 10000);
  BSP_TIM_SetPrescaler(BUZZER_TIM_ID, TIM_MAIN_FREQ * duration / 10000);
  // PWM start freq = music[0] = MAIN_FREQ / PSC / ARR
  BSP_TIM_SetARR(BUZZER_PWM_ID, 10000);
  BSP_TIM_SetCCR(BUZZER_PWM_ID, 5000);
  BSP_TIM_SetPrescaler(BUZZER_PWM_ID, TIM_MAIN_FREQ / music[0] / 10000);
  BSP_TIM_PWM_Start(BUZZER_PWM_ID);
  BSP_TIM_Start_IT(BUZZER_TIM_ID);
}

void buzzer_stop() {
  if (buzzer.state == 0) {
    return;
  }
  buzzer.state = 0;
  buzzer.bpm = 0;
  buzzer.note_index = 0;
}

void buzzer_next_note(uint8_t tim_id, uint8_t* data, uint32_t len) {}
