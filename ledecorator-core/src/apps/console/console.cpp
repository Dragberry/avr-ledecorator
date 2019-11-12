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
	screen_interface.draw_number(7, 4, counter, true,  2, 1, RED, CYAN);
}
