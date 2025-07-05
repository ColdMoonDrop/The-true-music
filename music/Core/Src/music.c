/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    music.c
  * @brief   This file provides code for the music playing functionality
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

#include "music.h"

// 私有变量
static MusicState_t musicState = MUSIC_STOP;
static const Note_t* currentMelody = NULL;
static uint16_t melodyLength = 0;
static uint16_t currentNoteIndex = 0;
static uint32_t noteStartTime = 0;
static uint32_t currentNoteDuration = 0;

// 小星星
static const Note_t melody_twinkle[] = {
    {NOTE_C4, QUARTER_NOTE}, {NOTE_C4, QUARTER_NOTE},
    {NOTE_G4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE},
    {NOTE_A4, QUARTER_NOTE}, {NOTE_A4, QUARTER_NOTE},
    {NOTE_G4, HALF_NOTE},
    {NOTE_F4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE},
    {NOTE_E4, QUARTER_NOTE}, {NOTE_E4, QUARTER_NOTE},
    {NOTE_D4, QUARTER_NOTE}, {NOTE_D4, QUARTER_NOTE},
    {NOTE_C4, HALF_NOTE},
    {NOTE_G4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE},
    {NOTE_F4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE},
    {NOTE_E4, QUARTER_NOTE}, {NOTE_E4, QUARTER_NOTE},
    {NOTE_D4, HALF_NOTE},
    {NOTE_G4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE},
    {NOTE_F4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE},
    {NOTE_E4, QUARTER_NOTE}, {NOTE_E4, QUARTER_NOTE},
    {NOTE_D4, HALF_NOTE},
    {NOTE_C4, QUARTER_NOTE}, {NOTE_C4, QUARTER_NOTE},
    {NOTE_G4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE},
    {NOTE_A4, QUARTER_NOTE}, {NOTE_A4, QUARTER_NOTE},
    {NOTE_G4, HALF_NOTE},
    {NOTE_F4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE},
    {NOTE_E4, QUARTER_NOTE}, {NOTE_E4, QUARTER_NOTE},
    {NOTE_D4, QUARTER_NOTE}, {NOTE_D4, QUARTER_NOTE},
    {NOTE_C4, HALF_NOTE}
};
// 生日快乐
static const Note_t melody_birthday[] = {
    {NOTE_C4, EIGHTH_NOTE}, {NOTE_C4, EIGHTH_NOTE},
    {NOTE_D4, QUARTER_NOTE}, {NOTE_C4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE}, {NOTE_E4, HALF_NOTE},
    {NOTE_C4, EIGHTH_NOTE}, {NOTE_C4, EIGHTH_NOTE},
    {NOTE_D4, QUARTER_NOTE}, {NOTE_C4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE}, {NOTE_F4, HALF_NOTE},
    {NOTE_C4, EIGHTH_NOTE}, {NOTE_C4, EIGHTH_NOTE},
    {NOTE_C5, QUARTER_NOTE}, {NOTE_A4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE}, {NOTE_E4, QUARTER_NOTE}, {NOTE_D4, HALF_NOTE},
    {NOTE_A4, EIGHTH_NOTE}, {NOTE_A4, EIGHTH_NOTE},
    {NOTE_A4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE}, {NOTE_F4, HALF_NOTE}
};
// 音阶
static const Note_t melody_scale[] = {
    {NOTE_C4, QUARTER_NOTE}, {NOTE_D4, QUARTER_NOTE}, {NOTE_E4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE},
    {NOTE_G4, QUARTER_NOTE}, {NOTE_A4, QUARTER_NOTE}, {NOTE_B4, QUARTER_NOTE}, {NOTE_C5, QUARTER_NOTE},
    {NOTE_B4, QUARTER_NOTE}, {NOTE_A4, QUARTER_NOTE}, {NOTE_G4, QUARTER_NOTE}, {NOTE_F4, QUARTER_NOTE},
    {NOTE_E4, QUARTER_NOTE}, {NOTE_D4, QUARTER_NOTE}, {NOTE_C4, HALF_NOTE}
};

// 生成方波信号
static void GenerateSquareWave(uint16_t frequency, uint16_t duration) {
    if (frequency == 0) {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(duration);
        return;
    }
    uint32_t period = 1000000 / frequency;  // 周期(微秒)
    uint32_t halfPeriod = period / 2;
    uint32_t cycles = (duration * 1000) / period;
    for (uint32_t i = 0; i < cycles; i++) {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
        uint32_t delay_us = halfPeriod;
        uint32_t delay_ms = delay_us / 1000;
        uint32_t delay_remainder = delay_us % 1000;
        if (delay_ms > 0) HAL_Delay(delay_ms);
        if (delay_remainder > 0) for (volatile uint32_t j = 0; j < delay_remainder * 168; j++);
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        if (delay_ms > 0) HAL_Delay(delay_ms);
        if (delay_remainder > 0) for (volatile uint32_t j = 0; j < delay_remainder * 168; j++);
    }
}

void Music_Init(void) {
    musicState = MUSIC_STOP;
    currentMelody = NULL;
    melodyLength = 0;
    currentNoteIndex = 0;
    noteStartTime = 0;
    currentNoteDuration = 0;
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void Music_PlayNote(uint16_t frequency, uint16_t duration) {
    GenerateSquareWave(frequency, duration);
}

void Music_PlayMelody(const Note_t* melody, uint16_t length) {
    if (melody == NULL || length == 0) return;
    currentMelody = melody;
    melodyLength = length;
    currentNoteIndex = 0;
    musicState = MUSIC_PLAYING;
    noteStartTime = HAL_GetTick();
    currentNoteDuration = melody[0].duration;
}

void Music_Stop(void) {
    musicState = MUSIC_STOP;
    currentMelody = NULL;
    melodyLength = 0;
    currentNoteIndex = 0;
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

void Music_Toggle(void) {
    if (musicState == MUSIC_PLAYING) {
        musicState = MUSIC_PAUSE;
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
    } else if (musicState == MUSIC_PAUSE) {
        musicState = MUSIC_PLAYING;
        noteStartTime = HAL_GetTick();
    }
}

MusicState_t Music_GetState(void) {
    return musicState;
}

void Music_Update(void) {
    if (musicState != MUSIC_PLAYING || currentMelody == NULL) return;
    uint32_t currentTime = HAL_GetTick();
    uint32_t elapsedTime = currentTime - noteStartTime;
    if (elapsedTime >= currentNoteDuration) {
        currentNoteIndex++;
        if (currentNoteIndex >= melodyLength) currentNoteIndex = 0;
        Note_t currentNote = currentMelody[currentNoteIndex];
        GenerateSquareWave(currentNote.frequency, currentNote.duration);
        noteStartTime = HAL_GetTick();
        currentNoteDuration = currentNote.duration;
    }
}

void Music_PlayTwinkle(void) {
    Music_PlayMelody(melody_twinkle, sizeof(melody_twinkle) / sizeof(Note_t));
}
void Music_PlayBirthday(void) {
    Music_PlayMelody(melody_birthday, sizeof(melody_birthday) / sizeof(Note_t));
}
void Music_PlayScale(void) {
    Music_PlayMelody(melody_scale, sizeof(melody_scale) / sizeof(Note_t));
} 