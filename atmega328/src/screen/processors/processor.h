#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <stdint.h>
#include "../definitions.h"
#include "screen.h"

#define COMMAND_MASK 0b01000000

#define CR '\l'
#define LF '\n'
#define CMD_CLEAR_SCREEN '@' // 0b0100 0000
#define CMD_FILL_SCREEN 'A' // 0b0100 0001

class Processor
{
public:
	virtual ~Processor();

    virtual uint8_t process(Screen* screen) = 0;

    uint8_t read_byte();

    uint8_t is_command(uint8_t data);

    uint8_t read_command();
};

#endif
