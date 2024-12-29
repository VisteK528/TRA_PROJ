/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

#include "fir_filter.h"
#include "decimation_filter.h"
#include "liquid_crystal_i2c.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum {
  WAITING,
  STARTED,
  RECORDED,
  COMPLETED
} MIKE_Status;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#define SAMPLE_RATE 48000

#define OUTPUT_SAMPLE_RATE_16kHz
//#define OUTPUT_SAMPLE_RATE_8kHz

#if defined(OUTPUT_SAMPLE_RATE_8kHz) && !defined(OUTPUT_SAMPLE_RATE_16kHz)
  #define AUDIO_LENGTH_2 8000
  float audio2[AUDIO_LENGTH_2];

  #define INPUT_FIR_BUFFER_LENGTH 18
  float lp_input_buffer[INPUT_FIR_BUFFER_LENGTH];

  /* FIR Filter (low-pass antialiasing)
  * fs=48kHz
  * f_cutoff=4kHz (in design) (real cutoff = fs after decimation = 8kHz)
  * Suppresion in stopband ~ -50dB
  * Design method: Window-Sinc (Hamming window)
  * Order: 17
  */
  float lp_input_impulse_response[] = {-0.00299355, -0.00344797, -0.00262304, 0.00519037, 0.0257446, 0.0605127, 0.10397, 0.144531, 0.169115, 0.169115, 0.144531, 0.10397, 0.0605127, 0.0257446, 0.00519037, -0.00262304, -0.00344797, -0.00299355};

#endif

#if defined(OUTPUT_SAMPLE_RATE_16kHz) && !defined(OUTPUT_SAMPLE_RATE_8kHz)
  #define AUDIO_LENGTH_2 16000
  float audio2[AUDIO_LENGTH_2];

  #define INPUT_FIR_BUFFER_LENGTH 16
  float lp_input_buffer[INPUT_FIR_BUFFER_LENGTH];

  /* FIR Filter (low-pass antialiasing)
  * fs=48kHz
  * f_cutoff=8kHz (in design) (real cutoff = fs after decimation = 16kHz)
  * Suppresion in stopband ~ -50dB
  * Design method: Window-Sinc (Hamming window)
  * Order: 15
  */
  float lp_input_impulse_response[] = {0.00337896, 0.00291848, -0.00668689, -0.0280068, -0.0266131, 0.0487838, 0.192632, 0.313593, 0.313593, 0.192632, 0.0487838, -0.0266131, -0.0280068, -0.00668689, 0.00291848, 0.00337896};

#endif



uint32_t counter = 0;
uint32_t counter2 = 0;
uint8_t record_started = 0;


uint32_t adc_val = 0;
char message[32];
char message2[5];
MIKE_Status status = WAITING;

FIR_filter lp_input;
DecimationFilter decimation_filter;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim3)
  {

  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if(hadc == &hadc1) {
    adc_val = HAL_ADC_GetValue(&hadc1);
    const uint8_t decimation_status = DecimationFilterUpdate(&decimation_filter, (float)adc_val);
    if(status == STARTED && counter < AUDIO_LENGTH_2) {

      if(decimation_status == 1) {
        // Toggle the Green LED
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);


        audio2[counter] = decimation_filter.out;
        ++counter;
      }
    }
    else if(status == STARTED && counter >= AUDIO_LENGTH_2) {
      status = COMPLETED;
      counter = 0;
      HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);

      HD44780_Clear();
      HD44780_Home();
      HD44780_PrintStr("Transmitting...");

      if(HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
      }

    }
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  if(status == WAITING) {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
    counter = 0;
    status = STARTED;

    HD44780_Clear();
    HD44780_Home();
    HD44780_PrintStr("Recording...");

  }
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  SystemInit();

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  FIRFilterInit(&lp_input, lp_input_buffer, INPUT_FIR_BUFFER_LENGTH, lp_input_impulse_response, INPUT_FIR_BUFFER_LENGTH);
  DecimationFilterInit(&decimation_filter, &lp_input, 6);

  if(HAL_ADC_Start_IT(&hadc1) != HAL_OK) {
    Error_Handler();
  }

  // Start TIMER3
  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
    Error_Handler();
  }

  // start pwm generation
  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }


  /* Initialize */
  HD44780_Init(2);

  /* Clear buffer */
  HD44780_Clear();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_Delay(100);
    HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);

    if(status == COMPLETED) {
      HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);

      for(uint32_t i = 0; i < AUDIO_LENGTH_2; ++i) {
        const int length = snprintf(message, sizeof(message), "%05d,%05.5f\r\n", i, audio2[i]);
        if (length > 0 && length < sizeof(message)) {
          HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
          HAL_UART_Transmit_IT(&huart3, (uint8_t *)message, length);
          HAL_Delay(1);
        }
      }
      status = WAITING;
      if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
      }

      HD44780_Clear();
      HD44780_Home();
      HD44780_PrintStr("Waiting...");
      HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
