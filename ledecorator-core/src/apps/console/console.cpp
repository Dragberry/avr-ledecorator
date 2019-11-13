#include "console.h"
#include "lib/screen/colors.h"

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
	screen_interface.clear_screen(CYAN);
	screen_interface.clear_area(0, 0, SCREEN_WIDTH, 8, WHITE);
	screen_interface.draw_number(8, 1, counter, true, 2, 1, RED, WHITE);
	screen_interface.clear_area(0, 8, SCREEN_WIDTH, 4, CYAN);
	screen_interface.clear_area(0, 12, SCREEN_WIDTH, 4, BLUE);
}
