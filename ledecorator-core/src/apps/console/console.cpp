#include "console.h"
#include "lib/screen/colors.h"

void Console::increment()
{
	counter += 0.01;
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(CYAN);
	screen_interface.draw_number(12, 4, counter, 2, 1, RED, CYAN);
}
