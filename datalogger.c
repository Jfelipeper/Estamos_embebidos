#include "datalogger.h"
#include "main.h"


void GetTemperatura(char *timeStr, ADC_HandleTypeDef *hadc1, UART_HandleTypeDef *huart1){
int parte_entera;
int parte_decimal;
int adc_value;
int adc_value1;
char Cadena[100];

	HAL_ADC_Start(hadc1);
		if (HAL_ADC_PollForConversion(hadc1, 100) == HAL_OK) {
		      adc_value = HAL_ADC_GetValue(hadc1);
		}
		HAL_ADC_Stop(hadc1);
		adc_value1=(((adc_value*100000)/4096)*3.3*100)-5000000;
		parte_entera=adc_value1/100000;
		parte_decimal=abs(adc_value1%(parte_entera*100000));
		parte_decimal=parte_decimal/1000;
		sprintf(Cadena,"%s %d,%0.2d °C\n\r", timeStr, parte_entera, parte_decimal);
		HAL_UART_Transmit(huart1, (uint8_t *)Cadena, strlen(Cadena), HAL_MAX_DELAY);
}
