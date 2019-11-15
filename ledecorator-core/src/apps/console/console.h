#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "../application.h"
#include "../../hardware/screendatainterface.h"

class Console : public Application
{
private:
	int8_t offset_h = 0;
	int8_t offset_l = 0;
	float counter = 10.0;
	uint8_t digits[5];

protected:
	void increment();

	bool is_running();

	void build_image(ScreenInterface& screen_interface) const;
};

#endif
