#ifndef PIXEL_H_
#define PIXEL_H_

#include <stdint.h>

class Pixel
{
public:
	uint8_t level0;
	uint8_t level1;
	uint8_t level2;
	uint8_t level3;

	void reset()
	{
		level0 = 0;
		level1 = 0;
		level2 = 0;
		level3 = 0;
	}
};


#endif
