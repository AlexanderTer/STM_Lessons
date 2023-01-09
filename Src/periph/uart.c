#include "stm32f7xx.h"
#include "uart.h"

// Структура с переменными USART1
USART_Struct USART1_DATA;

void init_uart(void) {

	// Включить тактирование UART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	// Длина данных = 8 бит
	USART1->CR1 &= ~(USART_CR1_M_0 | USART_CR1_M_1);

	// Oversampling = 16 бит
	USART1->CR1 &= ~USART_CR1_OVER8;

	// Включить контроль бита чётности
	USART1->CR1 &= ~USART_CR1_PCE;

	// Включение режима приёма и передачи
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Включение таймаута между посылками
	USART1->CR2 |= USART_CR2_RTOEN;

	// Величина таймаута между посылками
	// (8 бит + 1 старт бит + 1 стоп бит) * 3.5 = 35
	USART1->RTOR = 35;

	// Скорость обмена 460800 бод
	USART1->BRR = 0x0EA;

	//Количество стоп битов = 1
	USART1->CR2 &= ~(USART_CR2_STOP_0 | USART_CR2_STOP_1);

	// Вкл генерацию сигнала включения драйвера на передачу
	USART1->CR3 |= USART_CR3_DEM;

	// Включить прерывание по приёму одного байта
	USART1->CR1 |= USART_CR1_RXNEIE;

	// Включить прерывание по таймауту между посылками
	USART1->CR1 |= USART_CR1_RTOIE;
	// Включить интерфейс
	USART1->CR1 |= USART_CR1_UE;
}

/**
 * \brief Запуск передачи данных по USART
 *
 * \param size: кол-во байт для передачи из буфера
 */
void transmit_USART1(uint32_t size)
{
	USART1_DATA.counter_tx = 0;
	USART1_DATA.size_tx = size;

	// Вкл. прерывания по условию пустоты регистра TDR
	USART1->CR1 |= USART_CR1_TXEIE;
}
