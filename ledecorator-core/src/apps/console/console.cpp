#include <avr/pgmspace.h>
#include "console.h"
#include "lib/screen/colors.h"
#include "../../common/datatypeutils.h"

void Console::increment()
{

}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
}
