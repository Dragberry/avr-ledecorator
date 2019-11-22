#include <avr/pgmspace.h>
#include "console.h"
#include "lib/screen/colors.h"
#include "../../common/datatypeutils.h"
#include "../../data/characters.h"
#include "../../data/images.h"

void Console::increment()
{
//	counter = counter - 0.01f;
//	float_to_string(counter_string, counter, 2, 1, true);
//	offset_h++;
//	if (offset_h == 24)
//	{
//		offset_h = -24;
//	}
//	offset_l--;
//	if (offset_l == -24)
//	{
//		offset_l = 24;
//	}
}

bool Console::is_running()
{
	return true;
}

void Console::build_image(ScreenInterface& screen_interface) const
{
	screen_interface.clear_screen(BLACK);
	screen_interface.draw_image(0, 0, 0, 0, get_character_image('.'), RED, YELLOW);
	screen_interface.draw_image(8, 0, 0, 0, get_character_image(','), RED, YELLOW);
	screen_interface.draw_image(16, 0, 0, 0, get_character_image('/'), RED, YELLOW);
	screen_interface.draw_image(24, 0, 0, 0, get_character_image('+'), RED, YELLOW);
//	screen_interface.draw_string(counter_string, 5, 8, 0, offset_h, 0, 24, 7, RED, BLACK);
//	screen_interface.draw_string("-12.2", 5, 8, 8, offset_l, 0, 24, 7, RED, BLACK);
//	screen_interface.draw_image(0, 0, IMG_PRESSURE, BLACK);
}
