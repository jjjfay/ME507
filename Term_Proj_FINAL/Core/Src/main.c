/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body for the ME 507 term project. The main contents of this file is the finite state machine that
  * 				  controls the prosthetic hand operation. This includes the tasks to open and close the hand and spin the hand
  * 				  from left to right. Lastly, there is a task to check for an emergency stop signal from a radio transmitter device.
  *
  * @authors		: Julia Fay & Jack Foxcroft
  *
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "encoder_reader.h"
#include "motor_driver.h"
#include "controller.h"
#include "myo.h"
#include "calibrate.h"
#include "radio.h"
#include <stdio.h>
#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

//create encoder objects

	encoder_t spin_enc = {.channel1 = TIM_CHANNEL_1,
						  .channel2 = TIM_CHANNEL_2,
						  .hal_tim = &htim3,
						  .mot_pos = 0,
						  .curr_count = 0,
						  .prev_count = 0,
						  .delta = 0};

//create motor objects

	motor_t spin_mot = {.pwm_val = 0,
				.channel1 = TIM_CHANNEL_1,
				.channel2 = TIM_CHANNEL_2,
				.hal_tim = &htim2
				};

	motor_t hand_mot = {.pwm_val = 0,
					.channel1 = TIM_CHANNEL_3,
					.channel2 = TIM_CHANNEL_4,
					.hal_tim = &htim2
					};


//create controller object
	controller_t spin_cont = {.p_mot = &spin_mot,
							  .p_enc = &spin_enc,
							  .gain = 300,
							  .setpoint = 0};

//create myo objects

	myo_t smyo = {.hal_adc = &hadc1,
				  .current_value = 0};


	myo_t hmyo = {.hal_adc = &hadc2,
				  .current_value = 0};

//create calibration objects

	calibrate_t scali = {.data_pts = 1000000,
			  	  	  	 .p_myo = &smyo
					  };

	calibrate_t hcali = {.data_pts = 1000000,
				  	  	  	 .p_myo = &hmyo
						  };

//initialize variables
uint32_t smyo_tst = 0;
uint32_t hand_count_tst = 0;
int32_t spos_tst = 0;
int32_t smyo_av = 0;
uint16_t ch1_val;
uint16_t ch2_val;
uint16_t ch1_p;
uint16_t ch2_p;
uint16_t radio_pulse = 1500;


char tst_buff[150];
int m;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/**
 * @brief Task 1 - SPIN TASK.
 *
 *   This function implements a state machine to control a motor based on input
 *   from a myoelectric sensor. The state machine has three states:
 *
 * - State 0: Initialization
 *   Initializes the motor driver, PWM channel, encoder channels, and zeroes the
 *   encoder value. Also, calibrates the myoelectric sensor to find an average
 *   value to be used as a threshold for determining motor direction.
 *
 * - State 1: Interpret Myoelectric Sensor
 *   Reads the current value from the myoelectric sensor and determines the
 *   direction of motor movement based on the sensor's value relative to a
 *   predefined threshold. Transitions to State 2 if a significant change in the
 *   sensor value is detected.
 *
 * - State 2: Move Motor
 *   Sets the motor's position based on the interpreted direction from State 1.
 *   If the sensor indicates a forward direction, the motor's setpoint is adjusted
 *   accordingly, and the motor is moved to the new setpoint. If the sensor indicates
 *   a backward direction, the motor is moved in the opposite direction. Once the
 *   motor reaches the setpoint, the state machine transitions back to State 1.
 *
 */
void task1(void) {
	static int currentState = 0;
	static char print_buff[150];
	static int n;

	static int32_t smyo_curr = 0;
	static int32_t smyo_dir = 0;
	static int32_t spin_sp = 0;
	static int32_t spin_pos = 0;
	static int32_t spin_pwm = 0;
	static int32_t smyo_av = 0;

	// State 0 - INIT_________________________________________________________________________________________
	if (currentState == 0) {

		//n = sprintf(print_buff,"\n\rTask 1, State 0\n");
		//HAL_UART_Transmit(&huart2,print_buff,n,400);

		//init the motor driver PWM channel and the
		//the encoder channels. Zero the encoder value
		controller_init(&spin_cont);

		//initialize the motor to be at rest
		set_duty(&spin_mot,0);

		//calibrate the myo sensor by finding average value to use later as threshold
		smyo_av = find_average(&scali);

		currentState = 1;}

	//State 1 - INTERPRET MYO ________________________________________________________________________________
	else if (currentState == 1) {

		n = sprintf(print_buff,"\n\rTask 1, State 1\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		smyo_curr = read_current(&smyo);

			if(smyo_curr > 3000){

				smyo_dir = 1;
				currentState = 2;

			}
			else if(smyo_curr < 1000){

				smyo_dir = 0;
				currentState = 2;

		}

	}

	//State 2 - MOVE MYO__________________________________________________________________________________________
	else if (currentState == 2) {

		n = sprintf(print_buff,"\n\rTask 1, State 2\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		spin_pos = get_pos(&spin_enc);

		n = sprintf(print_buff,"\n\rThe motor position is: %d\n", spin_pos);
		HAL_UART_Transmit(&huart2,print_buff,n,400);


		if(smyo_dir == 1){

			n = sprintf(print_buff,"\n\rThe setpoint is 465");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			spin_sp = 200;

			if(abs(spin_pos-spin_sp)<10){

				currentState = 1;
				set_duty(&spin_mot,0);
				spin_pos = 200;
			}
			else{
				 set_setpoint(&spin_cont, spin_sp);
				 spin_pwm = move(&spin_cont,1);


				 n = sprintf(print_buff,"\n\rThe motor pwm value is: %d\n", spin_pwm);
				 HAL_UART_Transmit(&huart2,print_buff,n,400);

				}
		}
		else{

			spin_sp = 0;

			n = sprintf(print_buff,"\n\rThe setpoint is 0");
			HAL_UART_Transmit(&huart2,print_buff,n,400);


			if(abs(spin_pos-spin_sp)<10){
				currentState = 1;
				set_duty(&spin_mot,0);
				spin_pos = 0;
			}
			else{
				set_setpoint(&spin_cont, spin_sp);

				spin_pwm = move(&spin_cont,1);

				n = sprintf(print_buff,"\n\rThe motor pwm value is: %d\n", spin_pwm);
				HAL_UART_Transmit(&huart2,print_buff,n,400);

			}
		}
	}

	else {

		n = sprintf(print_buff,"\n\rTask 1, Invalid State. Reset to State 1\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		currentState = 1;}
	//_____________________________________________________________________________________________________________
}

/**
 * @brief Task 2 - HAND TASK.
 *
 * This function implements a state machine to control a hand motor based on input
 * from a myoelectric sensor. The state machine has three states:
 *
 * - State 0: Initialization
 *   - Initializes the motor driver PWM channel.
 *   - Sets the motor to be at rest.
 *   - Calibrates the myoelectric sensor by finding an average value to use as a threshold.
 *
 * - State 1: Interpret Myoelectric Sensor
 *   - Reads the current value from the myoelectric sensor.
 *   - Determines the direction of hand movement (open or close) based on the sensor's
 *     value relative to predefined thresholds.
 *   - Transitions to State 2 if a significant change in the sensor value is detected.
 *
 * - State 2: Move Hand
 *   - Moves the hand to the open or closed position based on the interpreted direction
 *     from State 1.
 *   - If the sensor indicates to open the hand and the hand is currently closed, the hand
 *     is gradually opened by setting a positive duty cycle to the motor. The hand position
 *     is updated, and a counter ensures the hand moves incrementally.
 *   - If the sensor indicates to close the hand and the hand is currently open, the hand is
 *     gradually closed by setting a negative duty cycle to the motor. The hand position is
 *     updated, and a counter ensures the hand moves incrementally.
 *   - Once the hand reaches the desired position (fully open or fully closed), the state
 *     machine transitions back to State 1.
 *
 */
void task2(void) {
	static int currentState = 0;
	static char print_buff[150];
	static int n;

	static int32_t hmyo_curr = 0;
	static int32_t hmyo_dir = 0;
	static int32_t hand_pos = 0;
	static int32_t hmyo_av = 0;
	static int32_t hand_count = 0;

	// State 0 - INIT
	if (currentState == 0) {

		n = sprintf(print_buff,"\n\rTask 2, State 0\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		//init the motor driver PWM channels
		start_PWM(&hand_mot);

		//initialize the motor to be at rest
		set_duty(&hand_mot,0);

		//calibrate the myo values
		hmyo_av = find_average(&hcali);

		currentState = 1;}

	//State 1 - INTERPRET MYO
	else if (currentState == 1) {

		n = sprintf(print_buff,"\n\rTask 2, State 1\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		hmyo_curr = read_current(&hmyo);
		n = sprintf(print_buff,"\n\rThe myo value is : %d\n", hmyo_curr);
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		if(hmyo_curr > 3000){

						hmyo_dir = 1;
						currentState = 2;

					}
					else if(hmyo_curr < 1000){

						hmyo_dir = 0;
						currentState = 2;

				}
	}

	//State 2 - MOVE MYO
	else if (currentState == 2) {

		n = sprintf(print_buff,"\n\rTask 2, State 2\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);


		if(hmyo_dir == 1 && hand_pos == 0){


			n = sprintf(print_buff,"\n\rOpen Hand");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			if(hand_count > 15){

				currentState = 1;
				set_duty(&hand_mot,0);
				hand_pos = 1000;
				hand_count = 0;

			}
			else{

				set_duty(&hand_mot,1);
				hand_count++;
			}


				}
		else if(hmyo_dir ==0 && hand_pos == 1000){

			n = sprintf(print_buff,"\n\rClose Hand");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			if(hand_count > 20){

					currentState = 1;
					set_duty(&hand_mot,0);
					hand_pos = 0;
					hand_count = 0;

						}
			else{
					set_duty(&hand_mot,-3);
					hand_count++;
				}
				}


		currentState = 1;

	}

	else {
		n = sprintf(print_buff,"\n\rTask 1, Invalid State. Reset to State 1\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		currentState = 1;}
}

/**
 * @brief Task 3 - WIRELESS E STOP TASK.
 *
 * This function implements a state machine to handle a wireless emergency stop (E-stop) signal.
 * The state machine has three states:
 *
 * - State 0: Initialization
 *   - Starts the timer input capture interrupt for channels 1 and 2.
 *   - Transitions to State 1 to wait for the E-stop signal.
 *
 * - State 1: Wait for Signal
 *   - Continuously monitors for an E-stop signal by checking the radio pulse.
 *   - If a valid E-stop signal is detected, it transitions to State 2 to perform the emergency stop.
 *
 * - State 2: Emergency Stop
 *   - Outputs an emergency stop message.
 *   - Stops the motors by setting their duty cycles to zero.
 *   - Deinitializes the motor controller and stops the PWM signals.
 *   - Remains in this state after performing the emergency stop.
 *
 */
void task3(void) {
	static int currentState = 0;
	static char print_buff[150];
	static int n;

	// State 0 - INIT__________________________________________________________________
	if (currentState == 0) {

		//n = sprintf(print_buff,"\n\rTask 3, State 0\n");
		//HAL_UART_Transmit(&huart2,print_buff,n,400);

		HAL_TIM_IC_Start_IT (&htim1, TIM_CHANNEL_1);
		HAL_TIM_IC_Start_IT (&htim1, TIM_CHANNEL_2);


		currentState = 1;}

	//State 1 - WAIT FOR SIG_________________________________________________________
	else if (currentState == 1) {

		n = sprintf(print_buff,"\n\rTask 3, State 1\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		n = sprintf(print_buff,"\n\rThe radio pulse is: %d\n",radio_pulse);
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		if(check_delta(radio_pulse) == 1)
			  {
				currentState = 2;
			  }
}

	//State 2 - STOP_________________________________________________________________
	else if (currentState == 2) {

		n = sprintf(print_buff,"\n\rTask 3, State 2 EMERGENCY STOP\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		//call deinit commands
		set_duty(&spin_mot,0);
		controller_deinit(&spin_cont);

		set_duty(&hand_mot,0);
		stop_PWM(&hand_mot);

		}

	else {
		n = sprintf(print_buff,"\n\rTask 3, Invalid State. Reset to State 1\n");
		HAL_UART_Transmit(&huart2,print_buff,n,400);
		currentState = 1;
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 task1();
	 task2();
	 task3();

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 79;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 3999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Input Capture callback in non-blocking mode. This callback routine calculates the
  * 		radio transmitted pulse width in ms.
  *
  * @param  htim TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
   *        the HAL_TIM_IC_CaptureCallback could be implemented in the user file.
   */
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
          //store the previous value and read the captured value from the input capture register
	  	  ch1_p = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	      ch1_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);


	      //calculate the width of the pwm pulse generated by the radio transmitter
	      radio_pulse = ch1_val - ch1_p;
  }

}

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
