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
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

#include "fir_filter.h"
#include "decimation_filter.h"
#include "liquid_crystal_i2c.h"
#include "stft_solver.h"
#include "net_utils.h"

#include "word_classifier.h"
#include "word_classifier_data.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


typedef enum {
  WAITING,
  STARTED_RECORDING,
  STARTED_STFT,
  STARTED_TRANSMITTING_RESIZED,
  STARTED_TRANSMITTING_DATA,
  STARTED_TRANSMITTING_STFT,
  STARTED_NEURAL_NETWORK_PREDICTION,
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

#define CCMRAM_SECTION __attribute__((section(".ccmram")))

#define SAMPLE_RATE 48000

#define OUTPUT_SAMPLE_RATE_8kHz
//#define OUTPUT_SAMPLE_RATE_16kHz

#if defined(OUTPUT_SAMPLE_RATE_8kHz) && !defined(OUTPUT_SAMPLE_RATE_16kHz)
  #define AUDIO_LENGTH_2 8000
 float audio2[AUDIO_LENGTH_2];

  #define INPUT_FIR_BUFFER_LENGTH 61
  float lp_input_buffer[INPUT_FIR_BUFFER_LENGTH];

  /* FIR Filter (low-pass antialiasing)
  * fs=48kHz
  * f_cutoff=4kHz
  * Suppresion in stopband ~ -40/-60dB
  * Design method: Window-Sinc (Kaiser window) (beta=3)
  * Order: 60
  */
  float lp_input_impulse_response[] = {1.32113e-18, 0.00128676, 0.00262602, 0.00353702, 0.00354325, 0.00234971, -2.62847e-18, -0.00304789, -0.00597185, -0.00777285, -0.00756562, -0.00489789, 4.02847e-18, 0.0061298, 0.0118641, 0.01531, 0.0148296, 0.00959165, -5.27723e-18, -0.0121449, -0.0238367, -0.0314082, -0.0313327, -0.0211072, 6.14009e-18, 0.0305383, 0.0669324, 0.104033, 0.13606, 0.157747, 0.165415, 0.157747, 0.13606, 0.104033, 0.0669324, 0.0305383, 6.14009e-18, -0.0211072, -0.0313327, -0.0314082, -0.0238367, -0.0121449, -5.27723e-18, 0.00959165, 0.0148296, 0.01531, 0.0118641, 0.0061298, 4.02847e-18, -0.00489789, -0.00756562, -0.00777285, -0.00597185, -0.00304789, -2.62847e-18, 0.00234971, 0.00354325, 0.00353702, 0.00262602, 0.00128676, 1.32113e-18};
  const int decimation_factor = 6;

#endif

#if defined(OUTPUT_SAMPLE_RATE_16kHz) && !defined(OUTPUT_SAMPLE_RATE_8kHz)
  #define AUDIO_LENGTH_2 16000
  float audio2[AUDIO_LENGTH_2];

  #define INPUT_FIR_BUFFER_LENGTH 31
  float lp_input_buffer[INPUT_FIR_BUFFER_LENGTH];

  /* FIR Filter (low-pass antialiasing)
  * fs=48kHz
  * f_cutoff=8kHz
  * Suppresion in stopband ~ -40/-50dB
  * Design method: Window-Sinc (Kaiser window) (beta=3)
  * Order: 30
  */
  float lp_input_impulse_response[] = {2.64379e-18, 0.00525507, 0.00709057, -5.25997e-18, -0.0119506, -0.0151399, 8.06158e-18, 0.0237419, 0.0296762, -1.05605e-17, -0.0477009, -0.0627015, 1.22872e-17, 0.133942, 0.272277, 0.331021, 0.272277, 0.133942, 1.22872e-17, -0.0627015, -0.0477009, -1.05605e-17, 0.0296762, 0.0237419, 8.06158e-18, -0.0151399, -0.0119506, -5.25997e-18, 0.00709057, 0.00525507, 2.64379e-18};
  const int decimation_factor = 3;

#endif



uint32_t counter = 0;


uint32_t adc_val = 0;
char message[32];
MIKE_Status status = WAITING;

FIR_filter lp_input;
DecimationFilter decimation_filter;


#define FFT_SIZE 256
#define HOP_SIZE 128
#define STFT_ARRAY_LENGTH 7869
#define STFT_FILTER_MASK_SIZE 4

const float hann_window[FFT_SIZE] = {
    0.0000000, 0.0001518, 0.0006070, 0.0013654, 0.0024265, 0.0037897, 0.0054542, 0.0074189, 0.0096826, 0.0122440,
    0.0151015, 0.0182534, 0.0216978, 0.0254325, 0.0294554, 0.0337639, 0.0383554, 0.0432273, 0.0483764, 0.0537997,
    0.0594939, 0.0654555, 0.0716810, 0.0781664, 0.0849080, 0.0919015, 0.0991429, 0.1066275, 0.1143510, 0.1223086,
    0.1304955, 0.1389068, 0.1475372, 0.1563817, 0.1654347, 0.1746908, 0.1841445, 0.1937899, 0.2036212, 0.2136324,
    0.2238175, 0.2341703, 0.2446844, 0.2553535, 0.2661712, 0.2771308, 0.2882257, 0.2994492, 0.3107945, 0.3222546,
    0.3338226, 0.3454915, 0.3572542, 0.3691036, 0.3810324, 0.3930335, 0.4050995, 0.4172231, 0.4293969, 0.4416136,
    0.4538658, 0.4661460, 0.4784467, 0.4907605, 0.5030800, 0.5153975, 0.5277057, 0.5399971, 0.5522642, 0.5644996,
    0.5766958, 0.5888455, 0.6009412, 0.6129756, 0.6249415, 0.6368315, 0.6486384, 0.6603551, 0.6719745, 0.6834894,
    0.6948929, 0.7061782, 0.7173382, 0.7283663, 0.7392558, 0.7500000, 0.7605924, 0.7710267, 0.7812964, 0.7913953,
    0.8013173, 0.8110564, 0.8206067, 0.8299623, 0.8391176, 0.8480670, 0.8568051, 0.8653266, 0.8736263, 0.8816991,
    0.8895403, 0.8971449, 0.9045085, 0.9116265, 0.9184946, 0.9251086, 0.9314645, 0.9375585, 0.9433868, 0.9489460,
    0.9542326, 0.9592435, 0.9639755, 0.9684259, 0.9725919, 0.9764710, 0.9800608, 0.9833592, 0.9863641, 0.9890738,
    0.9914865, 0.9936009, 0.9954156, 0.9969296, 0.9981418, 0.9990517, 0.9996585, 0.9999621, 0.9999621, 0.9996585,
    0.9990517, 0.9981418, 0.9969296, 0.9954156, 0.9936009, 0.9914865, 0.9890738, 0.9863641, 0.9833592, 0.9800608,
    0.9764710, 0.9725919, 0.9684259, 0.9639755, 0.9592435, 0.9542326, 0.9489460, 0.9433868, 0.9375585, 0.9314645,
    0.9251086, 0.9184946, 0.9116265, 0.9045085, 0.8971449, 0.8895403, 0.8816991, 0.8736263, 0.8653266, 0.8568051,
    0.8480670, 0.8391176, 0.8299623, 0.8206067, 0.8110564, 0.8013173, 0.7913953, 0.7812964, 0.7710267, 0.7605924,
    0.7500000, 0.7392558, 0.7283663, 0.7173382, 0.7061782, 0.6948929, 0.6834894, 0.6719745, 0.6603551, 0.6486384,
    0.6368315, 0.6249415, 0.6129756, 0.6009412, 0.5888455, 0.5766958, 0.5644996, 0.5522642, 0.5399971, 0.5277057,
    0.5153975, 0.5030800, 0.4907605, 0.4784467, 0.4661460, 0.4538658, 0.4416136, 0.4293969, 0.4172231, 0.4050995,
    0.3930335, 0.3810324, 0.3691036, 0.3572542, 0.3454915, 0.3338226, 0.3222546, 0.3107945, 0.2994492, 0.2882257,
    0.2771308, 0.2661712, 0.2553535, 0.2446844, 0.2341703, 0.2238175, 0.2136324, 0.2036212, 0.1937899, 0.1841445,
    0.1746908, 0.1654347, 0.1563817, 0.1475372, 0.1389068, 0.1304955, 0.1223086, 0.1143510, 0.1066275, 0.0991429,
    0.0919015, 0.0849080, 0.0781664, 0.0716810, 0.0654555, 0.0594939, 0.0537997, 0.0483764, 0.0432273, 0.0383554,
    0.0337639, 0.0294554, 0.0254325, 0.0216978, 0.0182534, 0.0151015, 0.0122440, 0.0096826, 0.0074189, 0.0054542,
    0.0037897, 0.0024265, 0.0013654, 0.0006070, 0.0001518, 0.0000000
};

const float stft_filter_mask[] = {0.0010000, 0.1882551, 0.6112605, 0.9504844};
STFT_with_filter_solver stft_solver;

// Network setup

const char available_words[4][6] = {
        "Go",
        "Left",
        "Right",
        "Stop"
};

static ai_handle network = AI_HANDLE_NULL;
ai_u8 activations[AI_WORD_CLASSIFIER_DATA_ACTIVATIONS_SIZE];  // Memory for activations
ai_float input_data[AI_WORD_CLASSIFIER_IN_1_SIZE];         // Input buffer
ai_float output_data[AI_WORD_CLASSIFIER_OUT_1_SIZE];

float stft_complex_buffer[STFT_ARRAY_LENGTH*2];
float stft_buffer[AUDIO_LENGTH_2];

ai_buffer* ai_input;
ai_buffer* ai_output;

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
    if(status == STARTED_RECORDING && counter < AUDIO_LENGTH_2) {

      if(decimation_status == 1) {
        // Toggle the Green LED
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

        audio2[counter] = decimation_filter.out;
        ++counter;
      }
    }
    else if(status == STARTED_RECORDING && counter >= AUDIO_LENGTH_2) {
      status = STARTED_STFT;
      counter = 0;

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
    status = STARTED_RECORDING;

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
int main(void) {

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
    MX_X_CUBE_AI_Init();
    /* USER CODE BEGIN 2 */

    FIRFilterInit(&lp_input, lp_input_buffer, INPUT_FIR_BUFFER_LENGTH, lp_input_impulse_response,
                  INPUT_FIR_BUFFER_LENGTH);
    DecimationFilterInit(&decimation_filter, &lp_input, decimation_factor);
    STFT_Init(&stft_solver, FFT_SIZE, HOP_SIZE, AUDIO_LENGTH_2, hann_window, STFT_FILTER_MASK_SIZE, stft_filter_mask,
              stft_buffer);

    if (HAL_ADC_Start_IT(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    // Start TIMER3
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        Error_Handler();
    }

    // start pwm generation
    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }

    const ai_handle acts[] = { activations };
    ai_word_classifier_create_and_init(&network, acts, NULL);


    ai_input = ai_word_classifier_inputs_get(network, NULL);
    ai_input[0].data = AI_HANDLE_PTR(input_data);

    ai_output = ai_word_classifier_outputs_get(network, NULL);
    ai_output[0].data = AI_HANDLE_PTR(output_data);


    /* Initialize */
    HD44780_Init(2);

    HD44780_Clear();
    HD44780_Home();
    HD44780_PrintStr("Waiting...");

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        HAL_Delay(100);
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        uint8_t length;

        if (status == STARTED_STFT) {
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
            HD44780_Clear();
            HD44780_Home();
            HD44780_PrintStr("Started STFT...");
            STFT_Process_RowMajor(&stft_solver, audio2);
            resize_image(stft_buffer, 129, 61, input_data, 32, 32);


            status = STARTED_NEURAL_NETWORK_PREDICTION;
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
        }

//        if (status == STARTED_TRANSMITTING_DATA) {
//            HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
//
//            HD44780_Clear();
//            HD44780_Home();
//            HD44780_PrintStr("Transmitting");
//            HD44780_SetCursor(0, 1);
//            HD44780_PrintStr("data...");
//
//            length = snprintf(message, sizeof(message), "AUDIO_START\r\n");
//            HAL_UART_Transmit_IT(&huart3, (uint8_t *) message, length);
//            HAL_Delay(1);
//
//            for (uint32_t i = 0; i < AUDIO_LENGTH_2; ++i) {
//                length = snprintf(message, sizeof(message), "%05lu,%05.5f\r\n", i, audio2[i]);
//                if (length > 0 && length < sizeof(message)) {
//                    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//                    HAL_UART_Transmit_IT(&huart3, (uint8_t *) message, length);
//                    HAL_Delay(1);
//                }
//            }
//            length = snprintf(message, sizeof(message), "AUDIO_STOP\r\n");
//            HAL_UART_Transmit_IT(&huart3, (uint8_t *) message, length);
//            HAL_Delay(1);
//
//            status = STARTED_TRANSMITTING_STFT;
//
//        }
//
//        if (status == STARTED_TRANSMITTING_STFT) {
//            HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
//            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
//
//            HD44780_Clear();
//            HD44780_Home();
//            HD44780_PrintStr("Transmitting");
//            HD44780_SetCursor(0, 1);
//            HD44780_PrintStr("STFT...");
//
//            length = snprintf(message, sizeof(message), "STFT_START\r\n");
//            HAL_UART_Transmit_IT(&huart3, (uint8_t *) message, length);
//            HAL_Delay(1);
//
//            for (uint32_t i = 0; i < stft_solver.out_length; ++i) {
//                length = snprintf(message, sizeof(message), "%05lu,%05.5f\r\n", i, stft_solver.out[i]);
//                if (length > 0 && length < sizeof(message)) {
//                    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//                    HAL_UART_Transmit_IT(&huart3, (uint8_t *) message, length);
//                    HAL_Delay(1);
//                }
//            }
//
//            length = snprintf(message, sizeof(message), "STFT_STOP\r\n");
//            HAL_UART_Transmit_IT(&huart3, (uint8_t *) message, length);
//            HAL_Delay(1);
//
//            HD44780_Clear();
//            HD44780_Home();
//            HD44780_PrintStr("Waiting...");
//            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
//
//            status = STARTED_NEURAL_NETWORK_PREDICTION;
////            if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
////                Error_Handler();
////            }
//
//        }

        if (status == STARTED_NEURAL_NETWORK_PREDICTION) {
            HD44780_Clear();
            HD44780_Home();
            HD44780_PrintStr("Prediction: ");


            ai_word_classifier_run(network, &ai_input[0], &ai_output[0]);


            int predicted_class = argmax(output_data, AI_WORD_CLASSIFIER_OUT_1_SIZE);
            DisplayPredictedClass(predicted_class, AI_WORD_CLASSIFIER_OUT_1_SIZE, available_words);

            status = WAITING;
            if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) {
                Error_Handler();
            }
        }

        /* USER CODE END WHILE */

        MX_X_CUBE_AI_Process();
        /* USER CODE BEGIN 3 */
        /* USER CODE END 3 */
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
