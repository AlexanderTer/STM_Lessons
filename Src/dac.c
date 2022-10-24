#include "stm32f7xx.h"
#include "dac.h"

void init_dac(void){

	// Вкл тактирование ЦАП
	RCC ->APB1ENR |= RCC_APB1ENR_DACEN;

	// Вкл цап 1 и 2
	DAC->CR |= DAC_CR_EN1 | DAC_CR_EN2;
}
