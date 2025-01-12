#ifndef BUZZER_H
#define BUZZER_H

#include "bsp_tim.h"

#define BUZZER_PWM_ID 5
#define BUZZER_TIM_ID 6

typedef struct buzzer_t {
  uint8_t state;
  uint16_t note_index;
  uint8_t bpm;
  uint8_t beat;
  uint8_t* music;
} Buzzer_t;

void buzzer_init();
void buzzer_start(uint8_t*music,uint8_t bpm, uint8_t beat);
void buzzer_stop();
void buzzer_next_note(uint8_t tim_id, uint8_t* data, uint32_t len);

// define the frequency of the notes
#define DO 261
#define DO_SHARP 277
#define RE 293
#define RE_SHARP 311
#define MI 329
#define FA 349
#define FA_SHARP 369
#define SO 392
#define SO_SHARP 415
#define LA 440
#define LA_SHARP 466
#define SI 494
#define DO2 523
#define DO2_SHARP 554
#define RE2 587
#define RE2_SHARP 622
#define MI2 659
#define FA2 698
#define FA2_SHARP 739
#define SO2 784
#define SO2_SHARP 830
#define LA2 880
#define LA2_SHARP 932
#define SI2 988

#define beat4 4
#define beat8 4
#define beat16 16

#endif
