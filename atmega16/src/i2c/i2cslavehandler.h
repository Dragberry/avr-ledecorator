#ifndef I2CSLAVEHANDLER_H_
#define I2CSLAVEHANDLER_H_

#include <stdint.h>

namespace I2C
{

class SlaveHandler
{
public:
	virtual ~SlaveHandler() {}

	virtual void handle_recieve(uint8_t data_length, uint8_t* data) = 0;

	virtual uint8_t handle_transmit(uint8_t data_length, uint8_t* data) = 0;
};

}
#endif
