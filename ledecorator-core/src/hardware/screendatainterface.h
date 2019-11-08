#ifndef SCREENDATAINTERFACE_H_
#define SCREENDATAINTERFACE_H_

#include <stdint.h>

class ScreenDataInterface
{
protected:
	volatile uint8_t is_confirmed = 1;

public:
	virtual ~ScreenDataInterface() {}

	virtual void send_byte(const uint8_t byte);

	virtual uint8_t on_byte_confirmed();
};

#endif
