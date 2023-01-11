#include "interrupt.h"

#include "stm32f7xx.h"
#include "uart.h"
#include "timer.h"
#include "control.h"
#include "crc.h"




void init_interrupt(void) {

	// Выбор варианта прерывания 3(16 групп по 16 прерываний)
	NVIC_SetPriorityGrouping(3);

	// Установка приоритет прерыванийя TIM8_UP_TIM13: группа 3, приоритет 1
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn,
	NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 2));

	// Включаем прерывание TIM1_UP_TIM10 в NVIC
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	// Установка приоритет прерыванийя
	NVIC_SetPriority(DMA2_Stream0_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 1));

	//
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	// Установка приоритет прерывания группа 1 приоритет1
	NVIC_SetPriority(EXTI1_IRQn,
			NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));


	// Вкл прерывание EXTI1_IRQn
	NVIC_EnableIRQ(EXTI1_IRQn);

	// Установка приоритет прерывания группа 1 приоритет1
	NVIC_SetPriority(USART1_IRQn,
				NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 1));

		// Вкл прерывание EXTI1_IRQn
	NVIC_EnableIRQ(USART1_IRQn);
}

void TIM1_UP_TIM10_IRQHandler(void) {

	TIM1->SR &= ~TIM_SR_UIF; // Сброс фалага прерывания
	__ISB(); // Ожидание выполнения всех инструкций в конвейере(pipeline)

	master_transmit();

}



void USART1_IRQHandler()
{
    // Прерывание по приёму одного байта.
    if ((USART1->ISR & USART_ISR_RXNE) && (USART1->CR1 & USART_CR1_RXNEIE))
    {
        // Приём одного байта.
    	USART1_DATA.buffer_rx[USART1_DATA.counter_rx++] = USART1->RDR;
    }

    // Прерывание, когда TDR пуст и можно отправлять новый байт.
    if ((USART1->ISR & USART_ISR_TXE) && (USART1->CR1 & USART_CR1_TXEIE))
    {
        // Передача одного байта.
        USART1->TDR = USART1_DATA.buffer_tx[USART1_DATA.counter_tx++];

        // Если передали всё, что нужно...
        if (USART1_DATA.counter_tx >= USART1_DATA.size_tx)
        {
            // Выключаем прерывание по свободному TDR.
            USART1->CR1 &= ~USART_CR1_TXEIE;

            // Включаем прерывание по окончанию передачи.
            USART1->CR1 |= USART_CR1_TCIE;
        }
    }
    if (USART1->ISR & USART_ISR_RTOF)
        {
            // Очищаем соответствующий бит статуса UART.
            USART1->ICR |= USART_ICR_RTOCF;

            if(USART1_DATA.counter_rx > 2)
            {

            // Расчёт CRC-16.
            uint16_t crc = calc_CRC16(USART1_DATA.buffer_rx, USART1_DATA.counter_rx - 2);

            // Байты CRC расчитанного.
            uint8_t crc_1 = crc & 0xFF;
            uint8_t crc_2 = crc >> 8;

            // Байты CRC принятого.
            uint8_t crc_rx_1 = USART1_DATA.buffer_rx[USART1_DATA.counter_rx - 2];
            uint8_t crc_rx_2 = USART1_DATA.buffer_rx[USART1_DATA.counter_rx - 1];

            // Проверяем соответствие CRC.
            if ( (crc_1 == crc_rx_1) && (crc_2 == crc_rx_2))
            {
#ifdef CONTROL_MASTER
            	master_receive();
#else
            	slave_receive();
            	slave_transmit();
#endif
            }
            }

            /*

            ВСТАВИТЬ ОБРАБОТКУ ПРИНЯТЫХ БАЙТОВ И ФОРМИРОВАНИЕ ОТВЕТА.

            */

            // Очищаем счётчик принятых байт для следующего приёма.
            USART1_DATA.counter_rx = 0;
        }

    // Прерывание по окончанию передачи.
    if ((USART1->ISR & USART_ISR_TC) && (USART1->CR1 & USART_CR1_TCIE))
    {
        // Очищаем соответствующий бит статуса UART.
        USART1->ICR |= USART_ICR_TCCF;

        // Очищаем счётчи отправленных байт для следующей передачи.
        USART1_DATA.counter_tx = 0;
        USART1_DATA.size_tx = 0;

        // Выключаем прерывание по окончанию передачи.
        USART1->CR1 &= ~USART_CR1_TCIE;
    }
}

