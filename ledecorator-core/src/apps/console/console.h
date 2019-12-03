#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdlib.h>
#include "../application.h"
#include "../../hardware/screen/drawablestring.h"
#include "../../hardware/i2c/i2c.h"
#include "../../hardware/i2c/i2cslavehandler.h"

class Console : public Application
{
private:
	DrawableString string_h = DrawableString(0, 0, 32, 8);
	DrawableString string_l = DrawableString(0, 8, 32, 8);
	uint8_t i2c_state = 0;
	uint8_t state = 0;

protected:
	void increment();

public:
	Console();

	~Console();

	bool is_running();

	void build_image(ScreenInterface& screen_interface) const;

	void handle_recieve(uint8_t data_length, uint8_t* data)
	{
//		i2c_state++;
//		for (uint8_t i = 0; i < 4; i++)
//		{
//			string[i] = data[i];
//		}
	}

	uint8_t handle_transmit(uint8_t data_length, uint8_t* data)
	{
		return 0;
	}
};

#endif
