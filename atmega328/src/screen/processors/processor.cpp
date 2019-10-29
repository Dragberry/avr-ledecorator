#include "processor.h"
#include <avr/io.h>

Processor::~Processor() {}

uint8_t Processor::read_byte()
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

uint8_t Processor::read_command()
{
	while (1)
	{
		uint8_t data = read_byte();
		if (data & COMMAND_MASK)
		{
			return data;
		}

	}
}
