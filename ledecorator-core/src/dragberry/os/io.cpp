#include "io.hpp"
#include "lib/screen/colors.h"

uint8_t byte_index = 0;

uint8_t colors[] = {
        YELLOW,
        BLUE,
        RED,
        CYAN,
		WHITE,
		GREEN,
        MAGENTA,
};

uint8_t dragberry::io::read()
{
	if (byte_index == 7)
	{
		byte_index = 0;
	}
	return colors[byte_index++];
}

