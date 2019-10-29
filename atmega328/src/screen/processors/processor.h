#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <stdint.h>
#include "../defenitions.h"
#include "buffer.h"

#define COMMAND_MASK 0b11000000

class Processor
{
public:
	virtual ~Processor();

    virtual uint8_t process(Buffer* buffer) = 0;

    uint8_t read_byte();

    uint8_t read_command();
};

#endif
