#ifndef DISPLAYINTERFACE_H_
#define DISPLAYINTERFACE_H_

#include <stdint.h>

class DisplayInterface
{
public:
	virtual ~DisplayInterface() {}

	virtual void start_row() = 0;

	virtual void send_byte(const uint8_t byte) = 0;

	virtual void complete_row() = 0;
};

#endif
