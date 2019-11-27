#include <avr/pgmspace.h>
#include "console.h"
#include "lib/screen/colors.h"
#include "../../common/datatypeutils.h"

Console::Console()
{
	string_h.set_string("HAPPY BIRTHDAY", 14);
	string_l.set_string("GAVRILA", 7);
}

Console::~Console()
{

}

void Console::increment()
{
	string_h.increment();
	string_l.increment();
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
//	screen_interface.draw_area(8, 0, 8, 8, YELLOW);
//	screen_interface.draw_area(16, 0, 8, 8, i2c_state % 2 ? YELLOW : RED);
//	screen_interface.draw_area(24, 0, 8, 8, state % 2 ? YELLOW : CYAN);
	string_h.draw(screen_interface);
	string_l.draw(screen_interface);
}
