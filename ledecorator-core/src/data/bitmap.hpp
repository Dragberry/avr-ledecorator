#ifndef BITMAP_HPP_
#define BITMAP_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>

template <uint8_t size>
struct BitMap
{
	const uint8_t width;
	const uint8_t height;
	const uint8_t data[((size - 1) / 8) + 1];

	const uint8_t get_width() const
	{
		 return pgm_read_byte(&width);
	}

	const uint8_t get_height() const
	{
		 return pgm_read_byte(&height);
	}

	const uint8_t get_bit(const uint8_t x, const uint8_t y) const
	{
		const uint16_t flat_index = y * get_width() + x;
		const uint8_t byte_index = flat_index / 8;
		const uint8_t bit_index = flat_index % 8;
		return (0b10000000 >> bit_index) & pgm_read_byte(&(data[byte_index]));
	}
};


#endif
