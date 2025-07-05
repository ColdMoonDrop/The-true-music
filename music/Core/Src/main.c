/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void play_tone(uint16_t freq, uint16_t ms);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

// C大调音符频率
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

// 低音区
#define NOTE_C3  131
#define NOTE_D3  147
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247

// 升音（#，sharp）
#define NOTE_CS3  139  // C#3/Db3
#define NOTE_DS3  156  // D#3/Eb3
#define NOTE_FS3  185  // F#3/Gb3
#define NOTE_GS3  208  // G#3/Ab3
#define NOTE_AS3  233  // A#3/Bb3

#define NOTE_CS4  277  // C#4/Db4
#define NOTE_DS4  311  // D#4/Eb4
#define NOTE_FS4  370  // F#4/Gb4
#define NOTE_GS4  415  // G#4/Ab4
#define NOTE_AS4  466  // A#4/Bb4

#define NOTE_CS5  554  // C#5/Db5
#define NOTE_DS5  622  // D#5/Eb5
#define NOTE_FS5  740  // F#5/Gb5
#define NOTE_GS5  831  // G#5/Ab5
#define NOTE_AS5  932  // A#5/Bb5

#define REST     0 // 空拍（休止符），用法如 {REST, QN}

#define QN 1000    // 四分音符
#define EN 500    // 八分音符
#define SN 250    // 十六分音符
#define DQN 1500   // 附点四分音符（1.5倍四分音符）
#define DEN 750   // 附点八分音符（1.5倍八分音符）
#define HN 2000   // 二分音符
#define DHN 3000   // 附点二分音符（1.5倍二分音符）
#define DN 4000   // 全音符

typedef struct {
    uint16_t freq;
    uint16_t duration;
} Note_t;

// 春日影主旋律
Note_t melody[] = {
    // 第一段
    {NOTE_E5, QN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_D5, EN},
    {NOTE_E5, DEN}, {NOTE_F5, SN}, {NOTE_E5, EN}, {NOTE_D5, DQN},
    {NOTE_E5, QN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_D5, EN},
    {NOTE_E5, DEN}, {NOTE_F5, SN}, {NOTE_E5, EN}, {NOTE_D5, DQN},
    {NOTE_E5, QN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_D5, EN},
    {NOTE_E5, DEN}, {NOTE_F5, SN}, {NOTE_E5, EN}, {NOTE_D5, DQN},
    {NOTE_E5, QN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_D5, EN},
    {NOTE_E5, DEN}, {NOTE_F5, SN}, {NOTE_E5, EN}, {NOTE_D5, DQN},

    // 第二段
    {NOTE_C4, SN}, {NOTE_D4, SN},
    {NOTE_E4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_C4, SN}, {NOTE_C4, SN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, QN}, {NOTE_C4, SN}, {NOTE_D4, SN}, {NOTE_E4, DQN},

    {REST, DQN}, {NOTE_E4, EN}, {NOTE_G4, EN}, {NOTE_C5, SN}, {NOTE_B4, SN},
    {NOTE_B4, QN}, {NOTE_C5, EN}, {NOTE_B4, QN}, {NOTE_C5, EN},
    {NOTE_B4, SN}, {NOTE_A4, SN}, {NOTE_G4, QN}, {NOTE_G4, EN}, {NOTE_D4, EN}, {NOTE_F4, EN},
    {NOTE_F4, QN}, {NOTE_E4, EN}, {NOTE_E4, QN}, {NOTE_G3, EN},
    {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_E4, QN}, {NOTE_G4, EN},
    {NOTE_C4, DQN}, {REST, QN}, {NOTE_C4, EN}, 
    {NOTE_D4, EN}, {NOTE_C4, DEN}, {NOTE_C4, SN}, {NOTE_C4, EN}, {NOTE_G4, EN}, {NOTE_C4, EN},
    {NOTE_F4, QN}, {NOTE_E4, EN}, {NOTE_D4, QN}, {NOTE_C4, EN}, {NOTE_C4, DQN}, {REST, QN},

    // 第三段
    {NOTE_C4, SN}, {NOTE_D4, SN},
    {NOTE_E4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_C4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, QN}, {NOTE_C4, SN}, {NOTE_D4, SN}, {NOTE_E4, DQN},

    {REST, DQN}, {NOTE_E4, EN}, {NOTE_G4, EN}, {NOTE_C5, SN}, {NOTE_B4, SN},
    {NOTE_B4, QN}, {NOTE_C5, EN}, {NOTE_B4, QN}, {NOTE_C5, EN},
    {NOTE_B4, SN}, {NOTE_A4, SN}, {NOTE_G4, QN}, {NOTE_G4, EN}, {NOTE_D4, EN}, {NOTE_F4, EN},
    {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_E4, EN}, {NOTE_E4, QN}, {NOTE_G3, EN},
    {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_E4, QN}, {NOTE_G4, EN},
    {NOTE_C4, DQN}, {REST, QN}, {NOTE_C4, SN}, {NOTE_C4, SN},
    {NOTE_D4, EN}, {NOTE_C4, QN}, {NOTE_C4, EN}, {NOTE_G4, EN}, {NOTE_C4, EN},
    {NOTE_F4, EN}, {NOTE_F4, SN}, {NOTE_F4, SN}, {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_D4, EN}, {NOTE_C4, EN}, {NOTE_B3, EN},
    {NOTE_C4, DQN}, {REST, DQN},

    // 第四段
    {NOTE_A4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_F4, EN}, {NOTE_F4, EN},
    {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_D4, QN}, {NOTE_G4, EN},
    {NOTE_G4, EN}, {NOTE_F4, SN}, {NOTE_F4, SN}, {NOTE_F4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, QN}, {NOTE_C4, SN}, {NOTE_B3, SN}, {NOTE_C4, DQN},
    {NOTE_A4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_F4, EN}, {NOTE_F4, EN},
    {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_D4, QN}, {NOTE_E4, EN},
    {NOTE_F4, EN}, {NOTE_E4, SN}, {NOTE_E4, SN}, {NOTE_E4, SN}, {NOTE_E4, SN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_E4, EN},
    {NOTE_D5, QN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_C5, EN},
    {NOTE_B4, QN}, {NOTE_A4, EN}, {NOTE_A4, DEN},
    {REST, QN}, {NOTE_A4, EN}, {NOTE_A4, SN}, {NOTE_G4, EN}, {NOTE_F4, SN}, {NOTE_F4, SN},
    {NOTE_F4, DQN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, 2500},  {REST, DQN},

    // 第五段
    {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, QN}, {NOTE_F4, SN}, {NOTE_G4, SN},
    {NOTE_A4, QN}, {NOTE_A4, SN}, {NOTE_B4, SN}, {NOTE_C5, QN}, {NOTE_D5, SN}, {NOTE_C5, SN}, 
    {NOTE_G4, QN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_F4, EN}, {NOTE_F4, EN},
    {NOTE_E4, QN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, DQN},
    {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, QN}, {NOTE_F4, SN}, {NOTE_G4, SN},
    {NOTE_A4, QN}, {NOTE_GS4, SN}, {NOTE_A4, SN}, {NOTE_B4, QN}, {REST, SN}, {NOTE_GS4, SN}, 
    {NOTE_E5, EN}, {NOTE_E5, EN}, {REST, SN}, {NOTE_A4, SN}, {NOTE_F5, EN}, {NOTE_E5, EN}, {NOTE_D5, EN},
    {NOTE_D5, DEN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_B4, SN}, {NOTE_C5, QN}, {NOTE_G4, SN}, {NOTE_C5, SN},
    {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_G4, EN},
    {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_G4, SN}, {NOTE_C5, SN},
    {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_G4, SN}, {NOTE_C5, SN},
    {NOTE_D5, DEN}, {NOTE_E5, SN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_C5, EN}, 
    {NOTE_B4, EN}, {NOTE_A4, EN}, {NOTE_A4, EN}, {NOTE_A4, QN}, {NOTE_G4, EN},
    {NOTE_G4, QN}, {NOTE_F4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_E4, DQN}, {REST, DQN},
    {NOTE_E4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, SN}, {NOTE_C4, SN},
    {NOTE_C4, 3000},
    {NOTE_E5, DEN}, {NOTE_F5, SN}, {NOTE_E5, EN}, {NOTE_D5, QN},

    //反复
    // 第三段
    {NOTE_C4, SN}, {NOTE_D4, SN},
    {NOTE_E4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_C4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, QN}, {NOTE_C4, SN}, {NOTE_D4, SN}, {NOTE_E4, DQN},

    {REST, DQN}, {NOTE_E4, EN}, {NOTE_G4, EN}, {NOTE_C5, SN}, {NOTE_B4, SN},
    {NOTE_B4, QN}, {NOTE_C5, EN}, {NOTE_B4, QN}, {NOTE_C5, EN},
    {NOTE_B4, SN}, {NOTE_A4, SN}, {NOTE_G4, QN}, {NOTE_G4, EN}, {NOTE_D4, EN}, {NOTE_F4, EN},
    {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_E4, EN}, {NOTE_E4, QN}, {NOTE_G3, EN},
    {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_E4, QN}, {NOTE_G4, EN},
    {NOTE_C4, DQN}, {REST, QN}, {NOTE_C4, SN}, {NOTE_C4, SN},
    {NOTE_D4, EN}, {NOTE_C4, QN}, {NOTE_C4, EN}, {NOTE_G4, EN}, {NOTE_C4, EN},
    {NOTE_F4, EN}, {NOTE_F4, SN}, {NOTE_F4, SN}, {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_D4, EN}, {NOTE_C4, EN}, {NOTE_B3, EN},
    {NOTE_C4, DQN}, {REST, DQN},

    // 第四段
    {NOTE_A4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_F4, EN}, {NOTE_F4, EN},
    {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_D4, QN}, {NOTE_G4, EN},
    {NOTE_G4, EN}, {NOTE_F4, SN}, {NOTE_F4, SN}, {NOTE_F4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_D4, QN}, {NOTE_C4, SN}, {NOTE_B3, SN}, {NOTE_C4, DQN},
    {NOTE_A4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_F4, EN}, {NOTE_F4, EN},
    {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_D4, EN}, {NOTE_D4, QN}, {NOTE_E4, EN},
    {NOTE_F4, EN}, {NOTE_E4, SN}, {NOTE_E4, SN}, {NOTE_E4, SN}, {NOTE_E4, SN}, {NOTE_E4, EN}, {NOTE_D4, EN}, {NOTE_E4, EN},
    {NOTE_D5, QN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_C5, EN},
    {NOTE_B4, QN}, {NOTE_A4, EN}, {NOTE_A4, DEN},
    {REST, QN}, {NOTE_A4, EN}, {NOTE_A4, SN}, {NOTE_G4, EN}, {NOTE_F4, SN}, {NOTE_F4, SN},
    {NOTE_F4, DQN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, 2500},  {REST, DQN},

    //第六段
    {NOTE_C5, 2500}, {NOTE_DS5, EN}, {NOTE_C5, DQN}, {NOTE_D5, DQN}, 
    {NOTE_E5, 3000}, {REST, EN}, {NOTE_DS5, EN}, {NOTE_E5, EN}, {NOTE_G5, QN}, {NOTE_D5, EN},
    {NOTE_C5, 2500}, {NOTE_DS5, EN}, {NOTE_C5, DQN}, {NOTE_D5, DQN}, 
    {NOTE_E5, EN}, {NOTE_DS5, QN}, {REST, EN}, {NOTE_AS5, EN}, {NOTE_A5, EN}, 
    {NOTE_DS5, SN}, {NOTE_D5, SN}, {NOTE_C5, QN}, {NOTE_C4, SN}, {NOTE_D4, SN}, {NOTE_C4, SN}, {NOTE_D4, SN}, {NOTE_C4, SN}, {NOTE_D4, SN},

    {NOTE_D5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_DS5, EN}, {NOTE_C5, QN},
    {NOTE_D5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, SN}, {NOTE_A4, SN},
    {NOTE_G4, EN}, {NOTE_E5, 2500}, {REST, 3000},

    {NOTE_D5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_DS5, EN}, {NOTE_C5, QN},
    {NOTE_D5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, SN}, {NOTE_A4, SN},
    {NOTE_G4, EN}, {NOTE_E5, EN}, {NOTE_D5, EN}, {NOTE_D5, DQN}, 
    {NOTE_D5, SN}, {NOTE_D5, SN}, {NOTE_D5, EN}, {NOTE_G4, SN}, {NOTE_G4, SN}, {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, 
    {NOTE_C5, DQN}, {NOTE_C5, EN}, {NOTE_C5, SN}, {NOTE_A4, SN}, {NOTE_C5, EN},
    {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_D5, SN}, {NOTE_D5, SN}, {NOTE_C5, QN},
    {REST, DQN}, {NOTE_D5, EN}, {NOTE_D5, SN}, {NOTE_C5, SN}, {NOTE_D5, EN}, 
    {NOTE_D5, SN}, {NOTE_D5, SN}, {NOTE_E5, EN}, {NOTE_D5, EN}, {NOTE_E5, QN}, {NOTE_F5, EN}, 
    {NOTE_E5, 6000},

    {NOTE_E5, QN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_D5, EN},
    {NOTE_E5, QN}, {NOTE_F5, SN}, {NOTE_E5, SN}, {NOTE_D5, EN}, {NOTE_E5, SN}, {NOTE_D5, SN}, {NOTE_C5, EN},
    {NOTE_E5, QN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_D5, EN},
    {NOTE_E5, QN}, {NOTE_F5, EN}, {NOTE_GS5, EN}, {NOTE_E5, EN}, {NOTE_D5, EN}, 
    {NOTE_D5, DQN}, {NOTE_GS5, QN}, {NOTE_B5, EN},
    {NOTE_A5, 3000},

    //第七段
    {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, QN}, {NOTE_F4, SN}, {NOTE_G4, SN},
    {NOTE_A4, QN}, {NOTE_A4, SN}, {NOTE_B4, SN}, {NOTE_C5, QN}, {NOTE_D5, SN}, {NOTE_C5, SN}, 
    {NOTE_G4, QN}, {NOTE_G4, EN}, {NOTE_G4, EN}, {NOTE_F4, EN}, {NOTE_F4, EN},
    {NOTE_E4, QN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, DQN},
    {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_D4, SN}, {NOTE_E4, SN}, {NOTE_F4, SN}, {NOTE_G4, QN}, {NOTE_F4, SN}, {NOTE_G4, SN},
    {NOTE_A4, QN}, {NOTE_GS4, SN}, {NOTE_A4, SN}, {NOTE_B4, QN}, {REST, SN}, {NOTE_GS4, SN}, 
    {NOTE_E5, EN}, {NOTE_E5, EN}, {REST, SN}, {NOTE_A4, SN}, {NOTE_F5, EN}, {NOTE_E5, EN}, {NOTE_D5, EN},
    {NOTE_D5, DEN}, {NOTE_C5, SN}, {NOTE_C5, SN}, {NOTE_B4, SN}, {NOTE_C5, QN}, {NOTE_G4, SN}, {NOTE_C5, SN},
    {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_G4, EN},
    {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_C5, SN}, {NOTE_C5, SN},
    {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_C5, EN}, {NOTE_C5, QN}, {NOTE_C5, SN}, {NOTE_C5, SN},
    {NOTE_D5, DEN}, {NOTE_E5, SN}, {NOTE_D5, EN}, {NOTE_C5, QN}, {NOTE_C5, EN}, 
    {NOTE_B4, EN}, {NOTE_A4, EN}, {NOTE_A4, EN}, {NOTE_A4, QN}, {NOTE_G4, EN},
    {NOTE_G4, QN}, {NOTE_F4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, EN},
    {NOTE_E4, DQN}, {REST, DQN},
    {NOTE_E4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_F4, EN}, {NOTE_E4, EN}, {NOTE_D4, SN}, {NOTE_C4, SN},
    {NOTE_C4, DQN}, {REST, DQN}, 
    {NOTE_F4, EN}, {NOTE_F4, QN}, {NOTE_GS4, QN}, {NOTE_C5, EN},
    {NOTE_C5, 6000},
    {NOTE_C5, EN}, {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_D5, EN}, {NOTE_C5, EN}, {NOTE_D5, SN}, {NOTE_D5, SN},
    {NOTE_C5, 6000}








};
const uint16_t melody_len = sizeof(melody) / sizeof(Note_t);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  while (1)
  {
    for (uint16_t i = 0; i < melody_len; i++)
    {
      play_tone(melody[i].freq, melody[i].duration);
      HAL_Delay(50); // 音符间隔
    }
    HAL_Delay(1000); // 一遍旋律后停1秒
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

// 产生freq频率的方波，持续ms毫秒
void play_tone(uint16_t freq, uint16_t ms)
{
  if (freq == 0) {
    HAL_Delay(ms);
    return;
  }
  uint32_t period_us = 1000000 / freq;
  uint32_t half_period = period_us / 2;
  uint32_t cycles = (ms * 1000) / period_us;
  for (uint32_t i = 0; i < cycles; i++)
  {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    for (volatile uint32_t j = 0; j < half_period * 16; j++);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
    for (volatile uint32_t j = 0; j < half_period * 16; j++);
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
