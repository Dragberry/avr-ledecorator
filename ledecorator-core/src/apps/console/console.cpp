#include <avr/pgmspace.h>
#include "console.h"
#include "lib/screen/colors.h"
#include "../../common/datatypeutils.h"

Console::Console()
{
	string_h.set_string("DOBROE UTRO", 11);
	string_h.align = DrawableString::Align::LEFT;
	string_h.color = MAGENTA;
	string_l.set_string("SVINUSHKA", 9);
	string_l.align = DrawableString::Align::LEFT;
	string_l.color = YELLOW;
}

Console::~Console() {}

void Console::increment()
{
	string_h.increment();
	if (time >= 32)
	{
		string_l.increment();
	}
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
	string_h.draw(screen_interface);
	string_l.draw(screen_interface);
}
