#include "gpio.h"
#include "stm32f7xx.h"
void init_GPIO_Output(GPIO_TypeDef *gpio, unsigned int pin);
void init_GPIO_AFunction(GPIO_TypeDef *gpio, unsigned int pin, unsigned int AF);
void init_GPIO_Analog(GPIO_TypeDef * gpio, unsigned int pin);

void init_GPIO(void) {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Светодиоды
	init_GPIO_Output(GPIOD, 1); 	// HL1
	init_GPIO_Output(GPIOD, 2); 	// HL2
	init_GPIO_Output(GPIOD, 3); 	// HL3
	init_GPIO_Output(GPIOD, 4); 	// HL4
	init_GPIO_Output(GPIOD, 5); 	// HL5

	// Тестовые выводы
	init_GPIO_Output(GPIOD, 6);
	init_GPIO_Output(GPIOD, 7);
	init_GPIO_Output(GPIOG, 9);
	init_GPIO_Output(GPIOG, 10);
	init_GPIO_Output(GPIOG, 11);


	// Выход TIM8 CH1
	init_GPIO_AFunction(GPIOC, 6, 3);

	init_GPIO_Analog(GPIOA, 0); 	// Выходное напряжение
	init_GPIO_Analog(GPIOC, 2); 	// Выходной ток
	init_GPIO_Analog(GPIOC, 3); 	// Входное напряжение
	init_GPIO_Analog(GPIOF, 4); 	// Ток дросселя
	init_GPIO_Analog(GPIOA, 3); 	// Сигнал инжекции

	init_GPIO_Analog(GPIOA, 4); 	// ЦАП1
	init_GPIO_Analog(GPIOA, 5);		// ЦАП2

	SYSCFG->EXTICR[0] |= 1 << 4;	// Настраиваем PB1 на внешниее прерывание
	EXTI->IMR |= EXTI_IMR_IM1; 		//Значение прерывания по линии 1 (PB1)
	EXTI->FTSR |= EXTI_FTSR_TR1; 	// Прерывание по срезу
	EXTI->RTSR |= EXTI_RTSR_TR1; 	// Прерывание по фронту


	// Выводы UART
	init_GPIO_AFunction(GPIOA, 9, 7);  // TX
	init_GPIO_AFunction(GPIOA, 10, 7); // RX
	init_GPIO_AFunction(GPIOA, 12, 7); // DE

}

void init_GPIO_Output(GPIO_TypeDef *gpio, unsigned int pin) {

	gpio->MODER |= 1 << (2 * pin);
}

void init_GPIO_AFunction(GPIO_TypeDef *gpio, unsigned int pin, unsigned int AF) {
	if (pin < 8)
		gpio->AFR[0] |= AF << (4 * pin);
	else
		gpio->AFR[1] |= AF << (4 * (pin - 8));
	gpio->MODER |= 2 << (2 * pin);

}

void init_GPIO_Analog(GPIO_TypeDef *gpio, unsigned int pin) {

	gpio->MODER |= 3 << (2 * pin);
}

