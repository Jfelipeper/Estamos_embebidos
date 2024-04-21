#include "datalogger.h"


UART_HandleTypeDef *uart;
ADC_HandleTypeDef *adc;

void GetTemperatura(char * buffer){
int parte_entera;
int parte_decimal;
int adc_value;
int adc_value1;

	HAL_ADC_Start(adc);
		if (HAL_ADC_PollForConversion(adc, 100) == HAL_OK) {
		      adc_value = HAL_ADC_GetValue(adc);
		}
		HAL_ADC_Stop(adc);
		adc_value1=(((adc_value*100000)/4096)*330)-5000000;
		parte_entera=adc_value1/100000;
		parte_decimal=abs(adc_value1%(parte_entera*100000));
		parte_decimal=parte_decimal/1000;
		sprintf(buffer,"%d,%2.2d °C\n\r", parte_entera, parte_decimal);
}

void GetTemperatura_init(ADC_HandleTypeDef *hadc1, UART_HandleTypeDef *huart1){
	adc = hadc1;
	uart = huart1;
}
