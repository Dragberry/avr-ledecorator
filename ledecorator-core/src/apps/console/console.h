#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "../application.h"
#include "../../hardware/screendatainterface.h"

class Console : public Application
{
private:
	float counter;
	uint8_t digits[5];

protected:
	void increment();

	bool is_running();

	void build_image(ScreenInterface& screen_interface) const;
};

#endif
