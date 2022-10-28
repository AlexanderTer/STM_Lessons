#include "interrupt.h"
#include "dsp.h"
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

	// Установка приоритет прерыванийя
	NVIC_SetPriority(DMA2_Stream0_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 1));

	//
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void TIM8_UP_TIM13_IRQHandler(void) {

	TIM8->SR &= ~TIM_SR_UIF; // Сброс фалага прерывания
	__ISB(); // Ожидание выполнения всех инструкций в конвейере(pipeline)

	GPIOD->ODR ^= 1 << 6;

}

void DMA2_Stream0_IRQHandler(void) {

	// Сброс флага DMA2 по окончанию передачи данных
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	// Ожидание выполнения инструкций
	__ISB();

	const float av_slope = 2.5f * 0.001f;
	const float v25 = 0.76f;

	extern volatile unsigned int ADC_Buffer[];

	float v_sense = (float) ADC_Buffer[3] * (3.3f / 4095.f);

	TEMPERATURE = (v_sense - v25) / av_slope + 25.f;

	/* Вывод сигнала на цап*/
	unsigned int dac1, dac2;

	// Выводим температуру от 0 до 100 градусов на цап1
	dac1 = (TEMPERATURE * 4095 / 100.f);

	// Фильтруем температуру
	dac2 = MovingFloatFilter(&TEMPERATURE_MOV, TEMPERATURE) * (4095.f / 100.f);

	// Фильтруем температуру
	//dac2 = MedianFloatFilter(&TEMPERATURE_MOV, TEMPERATURE) * (4095.f / 100.f);

	// Запись чисел в ЦАП1 и ЦАП2
	DAC->DHR12RD = dac1 | (dac1 << 16);
}
