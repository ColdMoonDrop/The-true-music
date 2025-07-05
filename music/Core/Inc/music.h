/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    music.h
  * @brief   This file contains all the function prototypes for
  *          the music.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MUSIC_H__
#define __MUSIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "gpio.h"

// 音符频率定义 (Hz)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047

// 音符持续时间定义 (ms)
#define QUARTER_NOTE    500
#define HALF_NOTE       1000
#define WHOLE_NOTE      2000
#define EIGHTH_NOTE     250

typedef struct {
    uint16_t frequency;  // 频率
    uint16_t duration;   // 持续时间(ms)
} Note_t;

typedef enum {
    MUSIC_STOP = 0,
    MUSIC_PLAYING,
    MUSIC_PAUSE
} MusicState_t;

void Music_Init(void);
void Music_PlayNote(uint16_t frequency, uint16_t duration);
void Music_PlayMelody(const Note_t* melody, uint16_t length);
void Music_Stop(void);
void Music_Toggle(void);
MusicState_t Music_GetState(void);
void Music_Update(void);
void Music_PlayTwinkle(void);
void Music_PlayBirthday(void);
void Music_PlayScale(void);

#ifdef __cplusplus
}
#endif
#endif /*__ MUSIC_H__ */ 