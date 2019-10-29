#include "buffer.h"

Buffer::Buffer()
{
	is_being_read = 0;
	buffer_1 = new uint8_t*[SCREEN_HEIGHT];
	buffer_2 = new uint8_t*[SCREEN_HEIGHT];
	for (uint8_t row = 0; row < SCREEN_HEIGHT; row++)
	{
		buffer_1[row] = new uint8_t[SCREEN_WIDTH];
		buffer_2[row] = new uint8_t[SCREEN_WIDTH];
	}
	active_buffer = buffer_1;
	buffer = buffer_2;
}

void Buffer::switch_buffer()
{
	while (is_reading());
	uint8_t** temp = active_buffer;
	active_buffer = buffer;
	buffer = temp;
}
