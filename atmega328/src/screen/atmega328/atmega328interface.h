#ifndef ATMEGA328INTERFACE_H_
#define ATMEGA328INTERFACE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/avr/hardware/uart.hpp"

#include "../interfaces/coreinterface.h"
#include "../interfaces/datainterface.h"
#include "../interfaces/displayinterface.h"

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SCK PB5
#define SPI_MOSI PB3
#define SPI_SS PB2

class Atmega328Interface :
		public DataInterface,
		public DisplayInterface,
		public CoreInterface
{
public:
	Atmega328Interface() {}

	inline void start_row() const
	{
		SPI_PORT &= ~(1<<SPI_SS);
	}

	inline void send_byte(const uint8_t byte) const
	{
		SPDR = byte;
		while (!(SPSR & (1<<SPIF)));
	}

	inline void complete_row()  const
	{
		SPI_PORT |= (1<<SPI_SS);
	}

	inline uint8_t get_data_byte() const
	{
		return UART::receive_byte_ack('A');
	}

	inline void send_data_byte(const uint8_t byte) const
	{
	}

	inline void launch() const
	{
		SPI_DDR |= (1<<SPI_SS) | (1<<SPI_MOSI) | (1<<SPI_SCK);
		SPI_PORT |= (1<<SPI_SS);
		SPCR |= (1<<SPE) | (1<<MSTR);
		SPSR |= (1<<SPI2X);

		UART::init(UART::BaudRate::B_4_800);

		// CTC
		TCCR0A |= (0<<WGM00);
		TCCR0A |= (1<<WGM01);
		TCCR0B |= (0<<WGM02);
		// 001 - f
		// 010 - f/8
		// 011 - f/64
		// 100 - f/256
		// 101 - f/1024
		TCCR0B |= (1<<CS02);
		TCCR0B |= (0<<CS01);
		TCCR0B |= (1<<CS00);
		TIMSK0 |= (1<<OCIE0A);
		// 0.2s
		OCR0A = 4;

		sei();
	}

	inline void stop() const
	{
		TIMSK0 &= ~(1<<OCIE0A);
	}
};

#endif
