#include <avr/io.h>
#include "worker.h"

Worker::~Worker() {}

uint8_t Worker::read_byte()
{
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

uint8_t Worker::is_command(uint8_t data)
{
	return data & COMMAND_MASK;
}

uint8_t Worker::read_command()
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
