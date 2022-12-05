#include "stm32f7xx.h"
#include "rcc.h"
void init_RCC(void) {
	// Сброс пеерфирии

	RCC->AHB1RSTR = 0xFFFFFFFF;
	RCC->AHB1RSTR = 0x00000000;

	RCC->AHB2RSTR = 0xFFFFFFFF;
	RCC->AHB2RSTR = 0x00000000;

	RCC->APB1RSTR = 0xFFFFFFFF;
	RCC->APB1RSTR = 0x00000000;

	RCC->APB2RSTR = 0xFFFFFFFF;
	RCC->APB2RSTR = 0x00000000;
// Кэш инструкций
	SCB_InvalidateICache();
	SCB_EnableICache();

	SCB_InvalidateDCache();
	SCB_EnableDCache();

	// Обновление переменной с частотой тактирования

	SystemCoreClockUpdate();

	// return;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// Включение внешнего генератора
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	// Выключение PLL
	RCC->CR &= ~RCC_CR_PLLON;
	while (RCC->CR & RCC_CR_PLLRDY)
		;

	// Настройка предделителей для шин переферии
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

	// Настройка предделителей PLL
	uint32_t pllcfgr = 0;

	pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE;
	pllcfgr |= 25 << RCC_PLLCFGR_PLLM_Pos;
	pllcfgr |= 432 << RCC_PLLCFGR_PLLN_Pos;
	pllcfgr |= 0 << RCC_PLLCFGR_PLLP_Pos;

	RCC->PLLCFGR = pllcfgr;

	// Настраиваем latancy на 7 для 216 МГц
	FLASH->ACR |= (7 << FLASH_ACR_LATENCY_Pos);

	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
		;

	// Over Drive enable
	PWR->CR1 |= (uint32_t) PWR_CR1_ODEN;
	PWR->CR1 |= (uint32_t) PWR_CR1_ODSWEN;

	// Выбор PLL как основного источника тактирования
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	SystemCoreClockUpdate();
}
