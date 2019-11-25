#include <avr/pgmspace.h>
#include "console.h"
#include "lib/screen/colors.h"
#include "../../common/datatypeutils.h"

void Console::increment()
{
	state++;
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
	screen_interface.draw_area(8, 0, 8, 8, YELLOW);
	screen_interface.draw_area(16, 0, 8, 8, i2c_state % 2 ? YELLOW : RED);
	screen_interface.draw_area(24, 0, 8, 8, state % 2 ? YELLOW : CYAN);
	screen_interface.draw_string(
			string, 4, ScreenInterface::Align::RIGHT, 0, 8, 0, 0, 32, 8, RED, BLACK);
}
