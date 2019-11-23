#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdlib.h>
#include "../application.h"
#include "../../hardware/screendatainterface.h"
#include "../../hardware/i2c/i2c.h"
#include "../../hardware/i2c/i2cslavehandler.h"

class Console : public Application, public I2C::SlaveHandler
{
private:
	char string[20];

protected:
	void increment();

public:
	Console()
	{
		I2C::init();
		I2C::set_slave_handler(this);
	}

	~Console()
	{
		I2C::set_slave_handler(NULL);
	}

	bool is_running();

	void build_image(ScreenInterface& screen_interface) const;

	void hansle_recieve(uint8_t data_length, uint8_t* data)
	{
		for (uint8_t i = 0; i < 20; i++)
		{
			string[i] = data[i];
		}
	}

	uint8_t handle_transmit(uint8_t data_length, uint8_t* data)
	{
		return 0;
	}
};

#endif
