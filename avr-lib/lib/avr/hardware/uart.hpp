#ifndef UART_HPP_
#define UART_HPP_

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#define UBRR_1X(BAUDRATE) ((unsigned int) ((F_CPU / (BAUDRATE * 16UL)) - 1UL))
#define UBRR_2X(BAUDRATE) ((unsigned int) ((F_CPU / (BAUDRATE * 8UL)) - 1UL))

namespace UART
{
	enum BaudRate : unsigned int
	{
		B_4_800 = UBRR_2X(4800UL),
		B_9_600 = UBRR_2X(9600UL),
		B_250_000 = UBRR_2X(250000UL),
		B_500_000 = UBRR_2X(500000UL),
		B_1_250_000 = UBRR_2X(1250000UL),
		B_2_500_000 = UBRR_2X(2500000UL),
	};

	class RxHandler
	{
	public:
		virtual ~RxHandler();

		virtual void handle_rx(const uint8_t byte) = 0;
	};

	static RxHandler* rx_handler = NULL;

	void set_rx_handler(RxHandler* rx_handler);

	class TxHandler
	{
	public:
		virtual ~TxHandler();

		virtual void handle_tx() = 0;
	};

	static TxHandler* tx_handler = NULL;

	void set_tx_handler(TxHandler* tx_handler);

	void init(BaudRate baud_rate);

	void stop();

	inline void send_byte(const uint8_t byte)
	{
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = byte;
	}

	void send_string(const char* string);

	inline uint8_t receive_byte()
	{
		while (!(UCSR0A & (1<<RXC0)));
		return UDR0;
	}

	uint8_t receive_byte_ack(uint8_t ack);
}

#endif
