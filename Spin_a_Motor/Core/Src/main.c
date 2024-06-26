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
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "motor_driver.h"

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
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

//Initialize all common variables
int char_flag = 0;
char buff[20] = {0};
char char_in = 0;
int pos = 0;
int wait_flag = 0;

// Initialize motor objects
motor_t mot1 = {.pwm_val    = 0,
		.channel1 = TIM_CHANNEL_1,
		.channel2 = TIM_CHANNEL_2,
		.hal_tim = &htim2
};
motor_t mot2 = {.pwm_val    = 0,
		.channel1 = TIM_CHANNEL_1,
		.channel2 = TIM_CHANNEL_2,
		.hal_tim = &htim3
};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

// Task 1 - KEY INPUTS
void task1(void) {
	static int currentState = 0;
	static char print_buff[150];
	static int n;

	// Task 0 - INIT
	if (currentState == 0) {

		printf("Task 1, State 0\n");

		//set up the first receive
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&char_in,1);
		currentState = 1;

		//initialize buffer with all zeros
		for (int i = 0; i < 20; i++) {
			buff[i] = 0;
		}
		wait_flag = 0;

	}

	//Task 1 - Wait for Key input
	else if (currentState == 1) {

		printf("Task 1, State 1\n");

		if(char_flag == 1){

			// Transition to state 2
			currentState = 2;

			//fill the buffer with the current key entry
			buff[pos] = char_in;
		}
	}

	//Task 2 - ECHO
	else if (currentState == 2) {

		printf("Task 1, State 1\n");

		//send key entry and set up the receiving for next key
		HAL_UART_Transmit(&huart2, (uint8_t*)&char_in,1,1);
		HAL_UART_Receive_IT(&huart2, (uint8_t*)&char_in,1);
		char_flag = 0;
		currentState = 1;
		pos++;
		if (pos>20){

			n = sprintf(print_buff,"\n\rInvalid Entry. Please try again.\n\r",pos);
			HAL_UART_Transmit(&huart2,print_buff,n,400);
			for (int i = 0; i < 20; i++) {
				buff[i] = 0;
			}
			pos = 0;
			currentState = 1;
			char_in = 0;
			wait_flag = 0;
		}

		if(wait_flag == 1){
			currentState = 3;
		}
	}

	//Task 3 - WAIT
	else if (currentState == 3) {

		if(wait_flag == 0){
			currentState = 1;
		}
	}

	else {
		// Invalid state
		currentState = 0; // Reset state
	}
}


// Task 2 MOTOR AND KEY INTERPRETATION
void task2(void) {

	//initialize all task variables
	static int currentState = 0;
	static int motor_num = 0;
	static int duty = 0;
	static char print_buff[150];
	static int n;
	static int duty_int;

	// State 0 - INIT MOTORS
	if (currentState == 0) {

		printf("Task 2, State 0\n");

		//start PWM generation for both motors
		start_PWM(&mot1);
		start_PWM(&mot2);
		set_duty(&mot1,0);
		set_duty(&mot2,0);
		printf("Task 2, State 1\n");

		n = sprintf(print_buff,"\n\rEnter Motor instructions in the format 'Mnxy' where n is the motor number and xy is the duty cycle in hex.\n\r");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		// Transition to state Always
		currentState = 1;
		wait_flag = 0;

	  //State 1 - Wait for Carriage return
	} else if (currentState == 1) {
		printf("Task 2, State 1\n");

		//create temp check variable
		char check = buff[pos-1];

		//handle when user presses enter
		if (check == '\r')
		{
			currentState = 2;
			wait_flag = 1;
			pos--;
			buff[pos] = 0;

		}
		//handle the backspace
		else if (check == 0x7F){
			pos = pos - 2;
			buff[pos] = 0;
		}
	}

	//State 2 - Interpret and Validate Input
	else if (currentState == 2) {

		printf("Task 2, State 1\n");

		//notify user of checking entry
		n = sprintf(print_buff,"\n\rChecking Entry...\n\r");
		HAL_UART_Transmit(&huart2,print_buff,n,400);

		if(buff[0] != 'M'){

			//notify user of invalid entry
			n = sprintf(print_buff,"\n\rInvalid char 1 Entry. Entry must be in the format Mnxy.\n\r");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			//reset the buffer and other variables
			for (int i = 0; i < 20; i++) {
				buff[i] = 0;
			}
			pos = 0;
			currentState = 1;
			char_in = 0;
			wait_flag = 0;

		}
		else if((buff[1] != '1') && (buff[1] != '2' ) ){

			//notify user of invalid entry
			n = sprintf(print_buff,"\n\rInvalid char 2 Entry. Entry must be in the format 'Mnxy'.\n\r");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			//reset the buffer and other variables
			for (int i = 0; i < 20; i++) {

				buff[i] = 0;
			}
			pos = 0;
			currentState = 1;
			char_in = 0;
			wait_flag = 0;

		}
		else if((isdigit(buff[2]) == 0 && isalpha(buff[2]) == 0) || buff[2]>'F'){

			//notify user of invalid entry
			n = sprintf(print_buff,"\n\rInvalid char 3 Entry. Entry must be in the format 'Mnxy'.\n\r");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			//reset the buffer and other variables
			for (int i = 0; i < 20; i++) {

				buff[i] = 0;
			}

			pos = 0;
			currentState = 1;
			char_in = 0;
			wait_flag = 0;

		}
		else if((isdigit(buff[3]) == 0 && isalpha(buff[3]) == 0) || buff[3]>'F'){

			//notify user of invalid entry
			n = sprintf(print_buff,"\n\rInvalid char 4 Entry. Entry must be in the format 'Mnxy'.\n\r");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			//reset the buffer and other variables
			for (int i = 0; i < 20; i++) {
				buff[i] = 0;
			}
			pos = 0;
			currentState = 1;
			char_in = 0;
			wait_flag = 0;

		}
		else if(buff[4]!= 0){

			//notify user of invalid entry
			n = sprintf(print_buff,"\n\rEntry too long. Entry must be in the format 'Mnxy'.\n\r");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			//reset the buffer and other variables
			for (int i = 0; i < 20; i++) {
				buff[i] = 0;
			}
			pos = 0;
			currentState = 1;
			char_in = 0;
			wait_flag = 0;

		}
		else{

			//notify user of valid entry
			n = sprintf(print_buff,"\n\rValid Entry.\n\r");
			HAL_UART_Transmit(&huart2,print_buff,n,400);

			//extract motor number
			motor_num = buff[1] - '0';
			char duty_str[2] = {buff[2],buff[3]};
			// Convert hexadecimal string to integer
			sscanf(duty_str, "%x", &duty_int);

			//If the input is a negative number, make sure it is negative
			if(duty_int >=128){
				duty_int ^= 128;
				duty_int -= 128;
			}

			//convert integer to be on the scale of the PWM compare value (640/127)
			duty_int = duty_int*5;

			//reset the buffer and other variables
			for (int i = 0; i < 20; i++) {
				buff[i] = 0;
			}
			pos = 0;
			currentState = 3;
			char_in = 0;
		}
	}


	//State 3 - Update Motors
	else if (currentState == 3) {
		printf("Task 2, State 1\n");

		if(motor_num == 1){

			//update motor duty cycle
			duty = duty_int;
			set_duty(&mot1,duty);

			//notify user of update
			n = sprintf(print_buff,"\n\rUpdated Motor 1 to a %d duty cycle.\n\r",duty);
			HAL_UART_Transmit(&huart2,print_buff,n,400);
			wait_flag = 0;

		}
		else if(motor_num == 2){

			//update motor duty cycle
			duty = duty_int;
			set_duty(&mot2,duty);

			//notify user of update
			n = sprintf(print_buff,"\n\rUpdated Motor 2 to a %d duty cycle.\n\r",duty);
			HAL_UART_Transmit(&huart2,print_buff,n,400);
			wait_flag = 0;
		}


		// Transition to state 0
		currentState = 1;
	} else {
		// Invalid state
		currentState = 1; // Reset state
	}
}


/* USER CODE END PFP */
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Function prototypes
void task1(void);
void task2(void);

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

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		// Execute task 1
		task1();

		// Execute task 2
		task2();

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
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
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
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1599;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
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
	sConfigOC.Pulse = 640;
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

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 1599;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 640;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

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
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : button_Pin */
	GPIO_InitStruct.Pin = button_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(button_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart2){
	char_flag = 1;
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
