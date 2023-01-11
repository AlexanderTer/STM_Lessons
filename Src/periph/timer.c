#include "timer.h"
#include "stm32f7xx.h"

void init_timer8(void) {
	// Включение тактирования модуля таймер8
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

	// Предделитель таймера 1
	// Макс счёт 1079 (100кГц)
	TIM8->PSC = 0;
	TIM8->ARR = 216000000 / 100000 / 2;



	// Симметричный счёт (режим 2)
	TIM8->CR1 |= TIM_CR1_CMS_1;

	// Включаем канал сравнея
	TIM8->CCER |= TIM_CCER_CC1E;

	// Вкл ШИМ
	timer_PWM_On();

	// Вкл. буферизации
	TIM8->CCMR1 |= TIM_CCMR1_OC1PE;

	// Вкл физ. канал выхода
	TIM8->BDTR |= TIM_BDTR_MOE;

	// Вкл прерывания по обновлению
	TIM8->DIER |= TIM_DIER_UIE;

	//Включение генерации TRGO по событию Update
	TIM8->CR2 |= TIM_CR2_MMS_1;

	// Вкл счёт таймера
	TIM8->CR1 |= TIM_CR1_CEN;

}

void init_TIMER1(void){
	// Включение тактирования модуля таймер8
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

		// Предделитель таймера 2
		// Макс счёт 43200 (2.5 кгц)
		TIM1->PSC = 1;
		TIM1->ARR = 216000000/ 2 / 2500;
		// Вкл прерывания по обновлению
		TIM1->DIER |= TIM_DIER_UIE;
		// Вкл счёт таймера
		TIM1->CR1 |= TIM_CR1_CEN;
}

// Функция включения ШИМ таймера 8
void timer_PWM_On(void) {

	// Режим ШИМ 1
	TIM8->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
}

// Функция выключения ШИМ таймера 8
void timer_PWM_Off(void) {

	// Режим принудительного логического 0
	TIM8->CCMR1 &= ~TIM_CCMR1_OC1M_1;

}
