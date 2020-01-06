#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/avr/hardware/i2c.hpp"
#include "lib/avr/hardware/i2cslavehandler.h"
#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"

#define F_CPU 8000000UL

//class I2CHandler : public I2C::SlaveHandler
//{
//private:
//	volatile uint8_t value = 0;
//public:
//	void handle_recieve(uint8_t data_length, uint8_t* data)
//	{
//		UART::send_string("BYTE IS RECIEVED:");
//		UART::send_byte_as_binary(data[0]);
//		value = data[0];
//		PORTA = value;
//	}
//
//	uint8_t handle_transmit(uint8_t data_length, uint8_t* data)
//	{
//		UART::send_string("BYTE IS SENT:");
//		UART::send_byte_as_binary(value);
//		data[0] = value;
//		return 1;
//	}
//} i2c_handler;

void UART_init(uint16_t ubrr)
{
	UBRRH = (uint8_t) (ubrr >> 8);
	UBRRL = (uint8_t) (ubrr);

	sbi(UCSRA, U2X);

	sbi(UCSRB, RXEN);
	sbi(UCSRB, RXCIE);

	sbi(UCSRB, TXEN);
	// 1 stop bit
	cbi(UCSRC, USBS);
	// 8 bit
	sbi(UCSRC, UCSZ0);
	sbi(UCSRC, UCSZ1);
}

class Timer :
		public Timers::T1::Handler,
		public UART::RxHandler
{
public:
	void on_uart_rx_event(const uint8_t byte)
	{
		UART::send_byte(byte);
	}
	void on_timer1_event()
	{
		UART::send_string("TEST STRING");
	}
} handlers;

void setup()
{
	Timers::T1::start(0x3D09, Timers::Prescaller::F_1024, &handlers);
	UART::init(UART::B_250_000);
	UART::set_rx_handler(&handlers);
//	I2C::init();
	sei();
}

int main()
{
	setup();
	while(1);
}
