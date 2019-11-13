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
}
