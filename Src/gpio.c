#include "gpio.h"
#include "stm32f7xx.h"
void init_GPIO_Output(GPIO_TypeDef * gpio, unsigned int pin);
void init_GPIO_AFunction(GPIO_TypeDef *gpio, unsigned int pin, unsigned int AF);

void init_GPIO(void)
{

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	init_GPIO_Output(GPIOD,1);
	init_GPIO_AFunction(GPIOC,6,3);
	init_GPIO_AFunction(GPIOC,7,3);
}

void init_GPIO_Output(GPIO_TypeDef * gpio, unsigned int pin){

	gpio->MODER |= 1 << (2*pin);
}

void init_GPIO_AFunction(GPIO_TypeDef *gpio, unsigned int pin, unsigned int AF){
	if (pin < 8) gpio->AFR[0] |= AF <<(4*pin);
	else gpio->AFR[1] |= AF <<(4*(pin - 8));
	gpio->MODER |= 2 << (2*pin);
	gpio ->OSPEEDR |= 3 << (2*pin);
}

