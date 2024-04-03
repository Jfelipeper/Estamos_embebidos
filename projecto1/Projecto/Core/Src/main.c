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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEMP_RESOLUTION 10000
#define MINUTE_TICK_OFFSET 600
#define WAIT_OFFSET 5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

enum state{lectura,espera,botonEspera};
enum event{start,wait,wait2};
FATFS filesystem;
FIL file;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */
void RTC_GetTimeStr(char *buffer);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void RTC_GetTimeStr(char *buffer) {
    RTC_DateTypeDef sDate;
    RTC_TimeTypeDef sTime;

    // Obtiene la fecha y hora actual del RTC
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    // Formatea la hora como una cadena de caracteres y la guarda en el buffer
    sprintf(buffer, "\n\r%02d:%02d:%02d\n\r", sTime.Hours, sTime.Minutes, sTime.Seconds);
}

void GetTemperatura( char *timeStr ){
	int parte_entera;
	int parte_decimal;
	int adc_value1;
	int adc_value;
	char Cadena[50];

	 HAL_ADC_Start(&hadc1);
		    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
		      adc_value = HAL_ADC_GetValue(&hadc1);
		    }
		  HAL_ADC_Stop(&hadc1);
		  HAL_Delay(1000);
		  adc_value1=(((adc_value*TEMP_RESOLUTION)/4096)*3.3*100)-50*TEMP_RESOLUTION;
		  parte_entera=adc_value1/TEMP_RESOLUTION;
		  parte_decimal=abs(adc_value1%(parte_entera*TEMP_RESOLUTION));
		  parte_decimal=parte_decimal/1000;

		 sprintf(Cadena,"%s %d,%0.2d °C\n\r", timeStr, parte_entera, parte_decimal);

		  // Enviar la cadena por UART
		 HAL_UART_Transmit(&huart1, (uint8_t *)Cadena, strlen(Cadena), HAL_MAX_DELAY);
}

void iniciosd(FATFS fs,FIL fil){
	char header[] = "|Hora|Temperatura|\n\r|-----|-----|\n\r";
	f_mount(&fs, "", 0);
	f_open(&fil, "tabla.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	f_lseek(&fil, fil.fsize);
	f_puts(header, &fil);
	f_close(&fil);
}

void flectura(FATFS fs, FIL fil){
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef sTime;
	int32_t adcValue;
	int32_t entero;
	int32_t decimal;
	char buffer[50];

	// Obtiene la fecha y hora actual del RTC
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
		adcValue = HAL_ADC_GetValue(&hadc1);
	}
	HAL_ADC_Stop(&hadc1);
    adcValue=(((adcValue*TEMP_RESOLUTION)/4096)*330)-50*TEMP_RESOLUTION;
    entero=adcValue/TEMP_RESOLUTION;
    decimal=(adcValue%(entero*TEMP_RESOLUTION))/100;

	// Formatea la hora como una cadena de caracteres y la guarda en el buffer
	sprintf(buffer, "| %02d:%02d:%02d | %d,%d °C |\r\n", sTime.Hours, sTime.Minutes, sTime.Seconds, entero, decimal);
	f_open(&fil, "tabla.txt", FA_OPEN_EXISTING | FA_WRITE | FA_READ);
	f_lseek(&fil,fil.fsize);
	f_puts(buffer,&fil);
	f_close(&fil);
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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  iniciosd(filesystem,file);
  char timeStr[50];
  uint8_t botonA,botonB,botonAold,botonBold,changePin;
  uint32_t startTick;
  enum state maquina = espera;
  enum event event = start;

  while (1)
  {

	  switch(maquina){
	  case lectura:
		  flectura(filesystem,file);
		  maquina = espera;
		  break;

	  case espera:
		  switch(event){
		  case start:
			  startTick = HAL_GetTick();
			  event = wait;
			  break;
		  case wait:
			  botonAold = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
			  botonBold = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
			  if( HAL_GetTick() > startTick + MINUTE_TICK_OFFSET ) maquina = lectura;
			  if( botonAold == GPIO_PIN_SET || botonBold == GPIO_PIN_SET )
			  {
				  maquina = botonEspera;
				  event = wait;
			  }
			  break;
		  default:
			  event = wait;
			  break;
		  }
		  break;

		  case botonEspera:
			  switch(event){
			  case start:
				  startTick = HAL_GetTick();
				  event = wait2;
				  break;
			  case wait:
				  botonA = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
				  botonB = (uint8_t) HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
				  if( (botonA != botonAold) && botonB == GPIO_PIN_RESET ){
					  RTC_GetTimeStr(timeStr);
					  GetTemperatura(timeStr);
					  maquina = espera;
				  }
				  else if( (botonB != botonBold) && botonA == GPIO_PIN_RESET ){
					  flectura(filesystem,file);
					  maquina = espera;
				  }
				  else if( (botonA && botonB) == GPIO_PIN_SET ) event=start;
				  break;

			  case wait2:
				  if(( HAL_GetTick() > startTick + WAIT_OFFSET ) &&  (botonA && botonB == GPIO_PIN_SET))
				  {
					  //fborrar();
					  maquina = espera;
					  event = start;
				  }
				  if (HAL_GetTick()%500 == 0 && changePin == 0 ){
					  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
					  changePin = 1;
				  }
				  if ( HAL_GetTick()%500 != 0 ) changePin = 0;
				  break;
			  }
		  break;

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
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

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_APRIL;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_SD_GPIO_Port, CS_SD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_SD_Pin */
  GPIO_InitStruct.Pin = CS_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_SD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB11 PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
