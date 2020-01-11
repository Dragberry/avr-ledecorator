#ifndef DRAGBERRY_DRAWABLESTRING_HPP_
#define DRAGBERRY_DRAWABLESTRING_HPP_

#include <stdint.h>
#include "lib/screen/colors.h"
#include "../../data/imagemono8x8.hpp"

namespace dragberry
{
	namespace os
	{
		class DrawableString
		{
		private:
			const char* string = 0;
			uint8_t string_length = 0;
			int16_t string_width = 0;
			bool is_big = false;

			const uint8_t start_x;
			const uint8_t start_y;
			const int8_t width;
			const int8_t height;

			int16_t offset_x = 0;

		public:
			enum Align : uint8_t
			{
				LEFT, RIGHT
			}
			align = LEFT;
			Color color = WHITE;
			Color bg_color = BLACK;

			DrawableString(
					const uint8_t start_x,
					const uint8_t start_y,
					const int8_t width,
					const int8_t height);

			void update();

			void set_string(const char* new_string);

			void set_string(const char* new_string, const uint8_t length);

			void draw() const;

		};
	}
}
#endif
