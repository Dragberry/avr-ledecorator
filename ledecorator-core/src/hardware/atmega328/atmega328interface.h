#ifndef ATMEGA328INTERFACE_H_
#define ATMEGA328INTERFACE_H_

#include <avr/io.h>

#include "../screendatainterface.h"

class Atmega328Interface : public ScreenDataInterface
{
	void send_byte(const uint8_t data);

	uint8_t on_byte_confirmed();
};

#endif
