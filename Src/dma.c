#include "dma.h"
#include "stm32f7xx.h"

volatile unsigned int ADC_Buffer[6];

void init_dma(void) {

	// Тактирование DMA 2
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	// Настраиваем адрес источника данных
	DMA2_Stream0->PAR = (unsigned int) &(ADC->CDR);

	// Настраиваем адрес места назначения (массив)
	DMA2_Stream0->M0AR = (unsigned int) &ADC_Buffer[0];

	// Количество пересылаемых данных
	DMA2_Stream0->NDTR = 6;

	// Выбираем канал 0
	DMA2_Stream0->CR |= 0 << 25;

	// Установка приоритета при выборе потоков DMA для передачи по шине данных = very high
	DMA2_Stream0->CR |= DMA_SxCR_PL_0 | DMA_SxCR_PL_1;

	// Размер данных в источнике (АЦП) = 32 бита
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_1;

	// Размер данных в месте назначения (массив) = 32 бита
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_1;

	// Включение инкримента адреса в месте назначения
	DMA2_Stream0->CR |= DMA_SxCR_MINC;

	// Включение циклической передачи
	DMA2_Stream0->CR |= DMA_SxCR_CIRC;

	// Прерывание по окончанию преобразования
	DMA2_Stream0->CR |= DMA_SxCR_TCIE;

	// Вкл  DMA
	DMA2_Stream0->CR |= DMA_SxCR_EN;



}
