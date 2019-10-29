#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include "../defenitions.h"

class Buffer
{
private:
	uint8_t** buffer_1;
	uint8_t** buffer_2;

public:
	uint8_t** buffer;
	uint8_t** active_buffer;

	uint8_t is_being_read;

	Buffer();

	void switch_buffer();

	void start_reading()
	{
		is_being_read = 1;
	}

	void stop_reading()
	{
		is_being_read = 0;
	}

	bool is_reading()
	{
		return is_being_read;
	}
};

#endif
