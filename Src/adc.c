#include "adc.h"
#include "stm32f7xx.h"

void init_adc(void) {

	// Вкл тактирования ацп
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// Вкл ADC1
	ADC1->CR2 |= ADC_CR2_ADON;

	// Канал 18 (датчик температуры)
	ADC1->SQR3 |= 18;

	// Режим непрерывного преобразования
	ADC1->CR2 |= ADC_CR2_CONT;

	// Период выборки 480 циклов
	ADC1->SMPR1 |= ADC_SMPR1_SMP18_0 | ADC_SMPR1_SMP18_1 | ADC_SMPR1_SMP18_2;

	// Включение датчика температуры
	ADC->CCR |= ADC_CCR_TSVREFE;

	// Вкл прерывания по окончанию
	ADC1->CR1 |= ADC_CR1_EOCIE;

	// Запуск преобразования
	ADC1->CR2 |= ADC_CR2_SWSTART;
}
