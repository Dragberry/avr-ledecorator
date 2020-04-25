#ifndef ATMEGA328DEVICE_HPP_
#define ATMEGA328DEVICE_HPP_

#include "lib/avr/hardware/spi.hpp"
#include "lib/avr/hardware/timers.hpp"
#include "uartbus.hpp"
#include "../deviceinterface.hpp"
#include "../../screen/screeninterface.hpp"

class Atmega328Device :
		public DeviceInterface,
		public Timers::T1::Handler,
		public UartBus::RxHandler
{
public:
	Atmega328Device() { }

	~Atmega328Device() { }

	void init()
	{
		SPI::init();
		UartBus::init(UartBus::BaudRate::B_500_000);
		UartBus::set_rx_handler(this);
		Timers::T1::start(4, Timers::Prescaller::F_1024, this);
		sei();
	}

	void stop()
	{
		cli();
		Timers::T1::stop();
		UartBus::set_rx_handler();
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
		data_interface->handle_byte(byte, UartBus::send_byte);
	}

};

#endif
