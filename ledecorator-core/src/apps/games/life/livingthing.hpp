#ifndef LIVINGTHING_HPP_
#define LIVINGTHING_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>

struct BitMap
{
	const uint8_t width;
	const uint8_t height;
	const uint8_t* const data;

	inline const uint8_t get_width() const
	{
		 return pgm_read_byte(&width);
	}

	inline const uint8_t get_height() const
	{
		 return pgm_read_byte(&height);
	}

	const uint8_t get_pixel(const uint8_t x, const uint8_t y) const
	{
		const uint16_t flat_index = y * get_width() + x;
		const uint8_t byte_index = flat_index / 8;
		const uint8_t bit_index = flat_index % 8;
		const uint8_t* real_data = (uint8_t*) pgm_read_ptr(&data);
		return (0b10000000 >> bit_index) & pgm_read_byte(&(real_data[byte_index]));
	}
};

const uint8_t GLIDER_DATA[2] PROGMEM =
{
		/*
		 * 111
		 * 001
		 * 010
		 */
		0b11100101,
		0b00000000
};

const BitMap GLIDER PROGMEM = { 3, 3, GLIDER_DATA };

const uint8_t SHIP_LIGHT_DATA[3] PROGMEM =
{
		/*
		 * 10010
		 * 00001
		 * 10001
		 * 01111
		 */
		0b10010000,
		0b01100010,
		0b11110000
};

const BitMap SHIP_LIGHT PROGMEM = { 5, 4, SHIP_LIGHT_DATA };

const uint8_t SHIP_MEDIUM_DATA[4] PROGMEM =
{
		/*
		 * 001000
		 * 100010
		 * 000001
		 * 100001
		 * 011111
		 */
		0b00100010,
		0b00100000,
		0b01100001,
		0b01111100
};

const BitMap SHIP_MEDIUM PROGMEM = { 6, 5, SHIP_MEDIUM_DATA };

const uint8_t SHIP_LARGE_DATA[5] PROGMEM =
{
		/*
		 * 0011000
		 * 1000010
		 * 0000001
		 * 1000001
		 * 1111111
		 */
		0b00110001,
		0b00001000,
		0b00001100,
		0b00010111,
		0b11100000,
};

const BitMap SHIP_LARGE PROGMEM = { 7, 5, SHIP_LARGE_DATA };


#endif
