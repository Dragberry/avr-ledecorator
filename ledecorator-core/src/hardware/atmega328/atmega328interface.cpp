#include "atmega328interface.h"

inline void Atmega328Interface::send_byte(const uint8_t data)
{
	is_confirmed = 0;
	UDR0 = data;
}

inline uint8_t Atmega328Interface::on_byte_confirmed()
{
	is_confirmed = 1;
	return UDR0;
}
