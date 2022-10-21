#include "interrupt.h"
#include "stm32f7xx.h"

volatile float TEMPERATURE;

void init_interrupt(void) {

	// Выбор варианта прерывания 3(16 групп по 16 прерываний)
	NVIC_SetPriorityGrouping(3);

	// Установка приоритет прерыванийя TIM8_UP_TIM13: группа 1, приоритет 1
	NVIC_SetPriority(TIM8_UP_TIM13_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));

	// Включаем прерывание TIM8_UP_TIM13 в NVIC
	NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

	// Установка приоритет прерыванийя TIM8_UP_TIM13: группа 2, приоритет 1
	NVIC_SetPriority(ADC_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 1));

	// Включаем прерывание TIM8_UP_TIM13 в NVIC
	NVIC_EnableIRQ(ADC_IRQn);
}

void TIM8_UP_TIM13_IRQHandler(void) {

	TIM8->SR &= ~TIM_SR_UIF; // Сброс фалага прерывания
	__ISB(); // Ожидание выполнения всех инструкций в конвейере(pipeline)

	GPIOD->ODR ^= 1 << 6;

}

void ADC_IRQHandler(void) {

	// Сброс флага
	ADC1->SR &= ~ADC_SR_EOC;
	// Ожидание выполнения инструкций
	__ISB();

	const float av_slope = 2.5f * 0.001f;
	const float v25 = 0.76f;

	float v_sense = ADC1->DR * (3.3f / 4095.f);

	TEMPERATURE = (v_sense - v25) / av_slope + 25.f;

}
