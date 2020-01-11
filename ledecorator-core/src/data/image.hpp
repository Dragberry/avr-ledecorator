#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <avr/pgmspace.h>
#include <stdint.h>
#include "lib/screen/colors.h"

struct Image
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

	inline const Color get_pixel(const uint8_t x, const uint8_t y) const
	{
		const uint8_t* real_data = (uint8_t*) pgm_read_ptr(&data);
		return pgm_read_byte(&(real_data[y * get_width() + x]));
	}

} PROGMEM;

#endif
