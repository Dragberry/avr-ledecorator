#include "drawablestring.hpp"

DrawableString::DrawableString(
		const uint8_t start_x,
		const uint8_t start_y,
		const int8_t width,
		const int8_t height)
	: start_x(start_x),
	  start_y(start_y),
	  width(width),
	  height(height) {}

void DrawableString::update()
{
	if (is_big)
	{
		if (align == LEFT)
		{
			if (offset_x + string_width)
			{
				offset_x--;
			}
			else
			{
				offset_x = width;
			}
		}
		else
		{
			if (width - offset_x)
			{
				offset_x++;
			}
			else
			{
				offset_x = -string_width;
			}

		}
	}
}

void DrawableString::set_string(const char* new_string, const uint8_t length)
{
	string = new_string;
	string_length = length;
	string_width = 0;
	for (uint8_t i = 0; i < length; i++)
	{
		const ImageMono8x8* img = ImageMono8x8::for_character(string[i]);
		string_width  += (img->get_width() + 1);
	}
	if (string_width > width)
	{
		offset_x = width;
		is_big = true;
	}
	else
	{
		offset_x = 0;
		is_big = false;
	}
}

void DrawableString::draw(ScreenInterface& screen_interface) const
{
	uint8_t x = start_x;
	int16_t passed_width = offset_x;
	if (!is_big && align == Align::RIGHT)
	{
		passed_width += (width - string_width);
	}
	if (passed_width > 0)
	{
		screen_interface.draw_area(x, start_y, passed_width, height, bg_color);
		if (passed_width >= width)
		{
			return;
		}
		x += passed_width;
	}
	int16_t passed_string_width = 0;
	for (uint8_t i = 0; i < string_length; i++)
	{
		const ImageMono8x8* img = ImageMono8x8::for_character(string[i]);
		const uint8_t img_width = img->get_width();
		passed_string_width += img_width ;
		passed_width += img_width ;
		if (passed_width < -1)
		{
			passed_width++;
			continue;
		}
		else
		{
			int16_t char_offset_x = passed_width - img_width;
			if (char_offset_x > 0)
			{
				char_offset_x = 0;
			}
			screen_interface.draw_image(x, start_y, char_offset_x, 0, img, color, bg_color);
			x += (img_width  + char_offset_x);
		}
		if (passed_width >= width)
		{
			break;
		}
		passed_width++;
		if (passed_width < 0)
		{
			continue;
		}
		else if (passed_width >= width)
		{
			break;
		}
		else
		{
			screen_interface.draw_line_vertical(x++, start_y, height, bg_color);
		}
	}
	if (passed_width < width)
	{
		screen_interface.draw_area(x, start_y, width - passed_width, height, bg_color);
	}
}
