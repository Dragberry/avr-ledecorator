#ifndef ATMEGA328INTERFACE_H_
#define ATMEGA328INTERFACE_H_

#include <avr/io.h>

#include "../screendatainterface.h"

class Atmega328Interface : public ScreenDataInterface
{
	inline void send_byte(const uint8_t data)
	{
		is_confirmed = 0;
		UDR0 = data;
	}

	inline uint8_t byte_confirmed()
	{
		is_confirmed = 1;
		return UDR0;
	}
};

#endif
