#ifndef DRAWABLESTRING_H_
#define DRAWABLESTRING_H_

#include <stdint.h>
#include "lib/screen/colors.h"
#include "screeninterface.h"
#include "../../data/imagemono8x8.h"

class DrawableString
{
private:
	bool is_big = 0;
	int16_t string_width = 0;
public:
	const char* string = 0;
	uint8_t string_length = 0;

	enum Align : uint8_t
	{
		LEFT, RIGHT
	}
	align = LEFT;
	Color color = WHITE;
	Color bg_color = BLACK;

	const uint8_t start_x;
	const uint8_t start_y;
	const int8_t width;
	const int8_t height;

	int16_t offset_x = 0;

	DrawableString(
			const uint8_t start_x,
			const uint8_t start_y,
			const int8_t width,
			const int8_t height);

	void increment();

	void set_string(const char* new_string, const uint8_t length);

	void draw(ScreenInterface& screen_interface) const;

};

#endif
