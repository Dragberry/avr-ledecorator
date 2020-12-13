#ifndef BITMAP8x8_HPP_
#define BITMAP8x8_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>

struct BitMap8x8
{
	const uint8_t data[8];

	const bool get_bit(const uint8_t x, const uint8_t y) const
	{
		return (0b10000000 >> x) & pgm_read_byte(&(data[y]));
	}
};


#endif
