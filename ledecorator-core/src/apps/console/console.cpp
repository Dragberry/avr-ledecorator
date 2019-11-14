#include "console.h"
#include "lib/screen/colors.h"
#include "../../data/images.h"

void Console::increment()
{
	counter = counter - 0.01f;
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
	screen_interface.draw_number(8, 1, counter, true, 2, 1, RED, BLACK);
	screen_interface.draw_image(0, 0, IMG_TEMPERATURE, BLACK);
	const int16_t* data;
	screen_interface.draw_histogram(8, 8, 0, 0, 8, 8, data, 6, BLUE, RED);
	screen_interface.draw_histogram(16, 8, 2, 2, 8, 8, data, 6, GREEN, YELLOW);
	screen_interface.draw_histogram(24, 8, -2, -2, 8, 8, data, 6, MAGENTA, CYAN);
}
