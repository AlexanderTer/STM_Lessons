#ifndef __UART_H__
#define __UART_H__

#define USART_BUFFER_RX_SIZE (100)
#define USART_BUFFER_TX_SIZE (100)

void init_uart(void);

typedef struct {

	// Буфер приёма данных
	uint8_t buffer_rx[USART_BUFFER_RX_SIZE];

	// Счётчик принятых данных
	uint32_t counter_rx;

	// Буфер передачи данных
	uint8_t buffer_tx[USART_BUFFER_TX_SIZE];

	// Счётчик переданных данных
	uint32_t counter_tx;

	// Количество данных для передачи
	uint32_t size_tx;

} USART_Struct;

extern USART_Struct USART1_DATA;

 void init_uart(void);
 void transmit_USART1(uint32_t size);

#endif

