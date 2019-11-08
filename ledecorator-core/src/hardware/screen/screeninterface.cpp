#include "screeninterface.h"

ScreenInterface::ScreenInterface(ScreenDataInterface& screen_data_interface) : screen_data_interface(screen_data_interface) {}


inline void ScreenInterface::start_picture()
{
	screen_data_interface.send_byte(COMMAND_MASK & CMD_DEFAULT);
	is_image_being_transmitted = 1;
}

inline void ScreenInterface::complete_picture()
{
	is_image_being_transmitted = 0;
}

inline void ScreenInterface::swith_buffer()
{
	uint8_t(*temp)[SCREEN_WIDTH] = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}

inline void ScreenInterface::send_byte()
{
	screen_data_interface.send_byte(active_buffer[y][x]);
	if (++x == SCREEN_WIDTH)
	{
		x = 0;
		if (++y == SCREEN_HEIGHT)
		{
			y = 0;
			complete_picture();
		}
	}
}
