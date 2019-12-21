#include "io.hpp"

uint8_t byte = 1;

uint8_t dragberry::io::read()
{
	return byte++;
}

