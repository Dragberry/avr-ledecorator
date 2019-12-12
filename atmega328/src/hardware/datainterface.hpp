#ifndef DATAINTERFACE_HPP_
#define DATAINTERFACE_HPP_

#include <stdint.h>

class DataInterface
{
public:
	virtual ~DataInterface() { }

	virtual void handle_byte(const uint8_t byte);
};

#endif
