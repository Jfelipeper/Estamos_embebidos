#ifndef INC_DATALOGGER_H_
#define INC_DATALOGGER_H_
#include "stm32f1xx_hal.h"

 void GetTemperatura(char *timeStr, ADC_HandleTypeDef *hadc1, UART_HandleTypeDef *huart1);
 #endif
