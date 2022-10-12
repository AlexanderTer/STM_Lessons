#include "timer.h"
#include "stm32f7xx.h"

void init_timer8(void){
	// Включение тактирования модуля таймер8
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

	// Предделитель таймера 1
	// Макс счёт 1079 (100кГц)
	TIM8->PSC = 0;
	TIM8->ARR = 216000000 / 100000 / 2 - 1;

	// Регистр сравнения: 540 (q 50%)
	TIM8->CCR1 = TIM8->ARR / 4;
	TIM8->CCR2 = TIM8->ARR - TIM8->CCR1;
	// Симметричный счёт (режим 2)
	TIM8->CR1 |= TIM_CR1_CMS_1;

	// Включаем канал сравнения
	TIM8->CCER |= TIM_CCER_CC1E;
	TIM8->CCER |= TIM_CCER_CC2E;
	// Режим ШИМ 1
	TIM8->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_3;
	TIM8->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_3;

	// Вкл. буферизации
	TIM8->CCMR1 |= TIM_CCMR1_OC1PE;
	TIM8->CCMR1 |= TIM_CCMR1_OC2PE;
	// Вкл физ. канал выхода
	TIM8->BDTR |= TIM_BDTR_MOE;

	// Вкл счёт таймера
	TIM8->CR1 |= TIM_CR1_CEN;

}
