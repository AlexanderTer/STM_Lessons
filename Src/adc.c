#include "adc.h"
#include "stm32f7xx.h"

void init_adc(void) {

	// Вкл тактирования ацп1 2 3
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

	// Вкл ADC1 2 3
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC2->CR2 |= ADC_CR2_ADON;
	ADC3->CR2 |= ADC_CR2_ADON;

	// Выбор каналов АЦП для первого преобразования
	ADC1->SQR3 |= 3;  // Сигнал инжекции
	ADC2->SQR3 |= 0;  // Выходное напряжение
	ADC1->SQR3 |= 14; // Ток реактора

	// Выбор каналов АЦП для второго преобразования
	ADC1->SQR3 |= 18 << 5;  // Температура
	ADC2->SQR3 |= 13 << 5;  // Входное напряжение
	ADC1->SQR3 |= 12 << 5;  // Выходной ток

	// Количество преобразований = 2
	ADC1->SQR1 |= 1 << 20;
	ADC2->SQR1 |= 1 << 20;
	ADC1->SQR1 |= 1 << 20;

	// Включение режима сканирвания
	ADC1->CR1 |= ADC_CR1SCAN;
	ADC2->CR1 |= ADC_CR1SCAN;
	ADC3->CR1 |= ADC_CR1SCAN;

	// Устанавливаем период выборки равным 15 циклов
	ADC1->SMPR1 |= ADC_SMPR1_SMP18_0;
	ADC1->SMPR1 |= ADC_SMPR2_SMP3_0;

	ADC2->SMPR1 |= ADC_SMPR2_SMP0_0;
	ADC2->SMPR1 |= ADC_SMPR1_SMP13_0;

	ADC3->SMPR1 |= ADC_SMPR1_SMP14_0;
	ADC3->SMPR1 |= ADC_SMPR1_SMP12_0;

	// Вкл запуска АЦП от TIM8 TRGO
	ADC1->CR2 |=ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2;

	// Запуск по нарастающему фронту
	ADC1->CR2 |= ADC_CR2_EXTEN_0;

	// Включение датчика температуры
	ADC->CCR |= ADC_CCR_TSVREFE;

	// Режим работы АЦП с ДМА 1
	ADC->CCR |= ADC_CCR_DMA_0;

	// Вкл. запуск запросов к DMA по окончанию преобразований
	ADC->CCR |= ADC_CCR_DDS;

	// Тройное преобразование
	ADC->CCR |= ADC_CCR_MULTI_1 | ADC_CCR_MULTI_2 | ADC_CCR_MULTI_4;
}
