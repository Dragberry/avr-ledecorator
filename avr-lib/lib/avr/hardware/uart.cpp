#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "uart.hpp"
#include "../software/operators.hpp"

UART::RxHandler::~RxHandler() {}

void UART::set_rx_handler(RxHandler* handler)
{
	rx_handler = handler;
	if (handler != NULL)
	{
		sbi(UCSR0B, RXCIE0);
	}
	else
	{
		cbi(UCSR0B, RXCIE0);
	}
}

UART::TxHandler::~TxHandler() {}

void UART::set_tx_handler(TxHandler* handler)
{
	tx_handler = handler;
}

void UART::init(UART::BaudRate baud_rate)
{
	UBRR0H = (uint8_t) (baud_rate >> 8);
	UBRR0L = (uint8_t) (baud_rate);

	sbi(UCSR0A, U2X0);

	sbi(UCSR0B, RXEN0);
	sbi(UCSR0B, TXEN0);
	// 1 stop bit
	cbi(UCSR0C, USBS0);
	// 8 bit
	sbi(UCSR0C, UCSZ00);
	sbi(UCSR0C, UCSZ01);
}

void UART::stop()
{
	cbi(UCSR0B, RXEN0);
	cbi(UCSR0B, TXEN0);
	set_rx_handler(NULL);
	set_tx_handler(NULL);
}

void UART::send_byte(const uint8_t byte)
{
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = byte;
}

void UART::send_string(const char* string)
{
	uint8_t index = 0;
	while (string[index] != '\0')
	{
		send_byte(string[index++]);
	}
	send_byte('\n');
}

uint8_t UART::receive_byte()
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

uint8_t UART::receive_byte_ack(uint8_t ack)
{
	while (!(UCSR0A & (1<<RXC0)));
	uint8_t byte = UDR0;
	UDR0 = ack;
	return byte;
}

ISR(USART_RX_vect)
{
	UART::rx_handler->handle_rx(UDR0);
}

