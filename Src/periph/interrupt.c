#include "interrupt.h"

#include "stm32f7xx.h"
#include "uart.h"
#include "timer.h"
#include "contril.h"
void init_interrupt(void) {

	// Выбор варианта прерывания 3(16 групп по 16 прерываний)
	NVIC_SetPriorityGrouping(3);

	// Установка приоритет прерыванийя TIM8_UP_TIM13: группа 1, приоритет 1
	//NVIC_SetPriority(TIM8_UP_TIM13_IRQn,
	//		NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 1));

	// Включаем прерывание TIM8_UP_TIM13 в NVIC
	//NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

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

void TIM8_UP_TIM13_IRQHandler(void) {

	TIM8->SR &= ~TIM_SR_UIF; // Сброс фалага прерывания
	__ISB(); // Ожидание выполнения всех инструкций в конвейере(pipeline)

	GPIOD->ODR ^= 1 << 6;

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

    // Прерывание по таймауту отсутствия байтов на приём.
    if (USART1->ISR & USART_ISR_RTOF)
    {
        // Очищаем соответствующий бит статуса UART.
        USART1->ICR |= USART_ICR_RTOCF;

        USART1_DATA.buffer_tx[0] = 200;

        // Отправляем 1 байт
        transmit_USART1(1);

        if(USART1_DATA.buffer_rx[0] == 10){
        	timer_PWM_On();
        	GPIOD->ODR &= ~((1 << 2) | (1 << 3) | (1 << 4) | (1 << 5));
        }
        else if (USART1_DATA.buffer_rx[0] == 20) timer_PWM_Off();
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

