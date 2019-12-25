#ifndef ATMEGA328DEVICE_HPP_
#define ATMEGA328DEVICE_HPP_

#include <avr/interrupt.h>

#include "lib/avr/hardware/spi.hpp"
#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"

#include "../deviceinterface.hpp"

#include "../../screen/screeninterface.hpp"

class Atmega328Device :
		public DeviceInterface,
		public Timers::T1::Handler,
		public UART::RxHandler
{
public:
	Atmega328Device() { }

	~Atmega328Device() { }

	void init()
	{
		SPI::init();
		UART::init(UART::BaudRate::B_2_500_000);
		UART::set_rx_handler(this);
		Timers::T1::start(4, Timers::Prescaller::F_1024, this);
		sei();
	}

	void stop()
	{
		cli();
		Timers::T1::stop();
		UART::stop();
		SPI::stop();
	}

	void start_row() const
	{
		SPI::reset();
	}

	void send_byte(const uint8_t byte) const
	{
		SPI::send_byte(byte);
	}

	void complete_row() const
	{
		SPI::latch();
	}

	void on_timer1_event()
	{
		timer_interface->on_timer_event();
	}

	void on_uart_rx_event(const uint8_t byte)
	{
		data_interface->handle_byte(byte, UART::send_byte);
	}

};

#endif
