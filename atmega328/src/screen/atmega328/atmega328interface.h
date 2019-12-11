#ifndef ATMEGA328INTERFACE_H_
#define ATMEGA328INTERFACE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/avr/hardware/spi.hpp"
#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"

#include "../interfaces/deviceinterface.hpp"

class Atmega328Interface :
		public DeviceInterface,
		public Timers::Handler
{
public:

	~Atmega328Interface() {	}

	void launch(RowDrawer* row_drawer)
	{
		this->row_drawer = row_drawer;
		SPI::init();
		UART::init(UART::BaudRate::B_9_600);
		Timers::T1::start(4, Timers::Prescaller::F_1024, this);
		sei();
	}

	void stop()
	{
		cli();
		Timers::T1::stop();
		UART::stop();
		SPI::stop();
		this->row_drawer = NULL;
	}

	inline void start_row() const
	{
		SPI::reset();
	}

	inline void send_byte(const uint8_t byte) const
	{
		SPI::send_byte(byte);
	}

	inline void complete_row()  const
	{
		SPI::latch();
	}

	inline uint8_t get_data_byte() const
	{
		return UART::receive_byte();
	}

	inline void send_data_byte(const uint8_t byte) const
	{
		UART::send_byte(byte);
	}

	inline void handle()
	{
		row_drawer->draw_row();
	}
};

#endif
