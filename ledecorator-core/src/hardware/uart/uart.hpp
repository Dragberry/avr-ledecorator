#ifndef UART_HPP_
#define UART_HPP_

#include <stdint.h>

#define UBRR(BAUDRATE) ((F_CPU / (BAUDRATE * 16UL)) - 1)

namespace UART
{
	enum BaudRate
	{
		B_500_000 = UBRR(500000UL),
		B_1_000_000 = UBRR(1000000UL),
	};

	class RxHandler
	{
	public:
		virtual ~RxHandler();

		virtual void handle_rx(const uint8_t byte) = 0;
	};

	static RxHandler* rx_handler;

	void set_rx_handler(RxHandler* rx_handler);

	class TxHandler
	{
	public:
		virtual ~TxHandler();

		virtual void handle_tx() = 0;
	};

	static TxHandler* tx_handler;

	void set_tx_handler(TxHandler* tx_handler);

	void init(BaudRate baud_rate);

	void stop();

	void send_byte(const uint8_t byte);

	void send_string(const char* string);
}

#endif
