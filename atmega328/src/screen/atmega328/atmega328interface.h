#ifndef ATMEGA328INTERFACE_H_
#define ATMEGA328INTERFACE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

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
private:
	const uint8_t ubrr;

public:
	Atmega328Interface(uint8_t ubrr) : ubrr(ubrr) {}

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
		while (!(UCSR0A & (1<<RXC0)));
		return UDR0;
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


		UBRR0H = (uint8_t) (ubrr >> 8);
		UBRR0L = (uint8_t) ubrr;
		UCSR0B |= (1<<RXEN0);
		// 1 stop bit
		UCSR0C |= (0<<USBS0);
		// 8 bit
		UCSR0C |= (1<<UCSZ00);
		UCSR0C |= (1<<UCSZ01);

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
