#include "io.hpp"

uint8_t b = 0;

uint8_t dragberry::io::read()
{
	return b++ % 2 ? 0b00111100 : 0b00000011;
}

