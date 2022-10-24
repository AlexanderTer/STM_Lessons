#include "gpio.h"
#include "stm32f7xx.h"
void init_GPIO_Output(GPIO_TypeDef *gpio, unsigned int pin);
void init_GPIO_AFunction(GPIO_TypeDef *gpio, unsigned int pin, unsigned int AF);

void init_GPIO(void) {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

	init_GPIO_Output(GPIOD, 1);
	init_GPIO_Output(GPIOD, 6);
	init_GPIO_AFunction(GPIOC, 6, 3);

	init_GPIO_Analog(GPIOA, 0); // Выходное напряжение
	init_GPIO_Analog(GPIOC, 2); // Выходной ок
	init_GPIO_Analog(GPIOC, 3); // Входное напряжение
	init_GPIO_Analog(GPIOF, 4); // Ток дросселя
	init_GPIO_Analog(GPIOA, 3); // Сигнал инжекции

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
	gpio->OSPEEDR |= 3 << (2 * pin);
}

void init_GPIO_Analog(GPIO_TypeDef *gpio, unsigned int pin) {

	gpio->MODER |= 3 << (2 * pin);
}

