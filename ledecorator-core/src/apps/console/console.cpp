#include "console.h"
#include "lib/screen/colors.h"

void Console::increment()
{
	counter++;
	uint16_t number = counter;
	for (uint8_t i = 0; i < 5; i++) {
		digits[i] = number % 10;
		number /= 10;
	}
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	const Color bg = CYAN;
	const Color color = RED;
	screen_interface.clear_screen(bg);
	uint8_t cursor_x = 1;
	uint8_t cursor_y = 4;
	screen_interface.draw_image(cursor_x, cursor_y, DS[digits[4]], 5, 7, color, bg);
	cursor_x += 6;
	screen_interface.draw_image(cursor_x, cursor_y, DS[digits[3]], 5, 7, color, bg);
	cursor_x += 6;
	screen_interface.draw_image(cursor_x, cursor_y, DS[digits[2]], 5, 7, color, bg);
	cursor_x += 6;
	screen_interface.draw_image(cursor_x, cursor_y, DS[digits[1]], 5, 7, color, bg);
	cursor_x += 6;
	screen_interface.draw_image(cursor_x, cursor_y, DS[digits[0]], 5, 7, color, bg);
//	cursor_x = 1;
//	cursor_y += 8;
//	screen_interface.draw_image(cursor_x, cursor_y, D_5, 5, 7, color, bg);
//	cursor_x += 6;
//	screen_interface.draw_image(cursor_x, cursor_y, D_6, 5, 7, color, bg);
//	cursor_x += 6;
//	screen_interface.draw_image(cursor_x, cursor_y, D_7, 5, 7, color, bg);
//	cursor_x += 6;
//	screen_interface.draw_image(cursor_x, cursor_y, D_8, 5, 7, color, bg);
//	cursor_x += 6;
//	screen_interface.draw_image(cursor_x, cursor_y, D_9, 5, 7, color, bg);
}
