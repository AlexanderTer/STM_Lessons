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
}

void TIM8_UP_TIM13_IRQHandler(void) {

	TIM8->SR &= ~TIM_SR_UIF; // Сброс фалага прерывания

	GPIOD->ODR ^= 1 << 6;

}
