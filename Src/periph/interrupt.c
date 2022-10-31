#include "interrupt.h"
#include "stm32f7xx.h"



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


