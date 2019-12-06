#ifndef SECTION_H_
#define SECTION_H_

#include "pixel.h"

class Section
{
public:
	Pixel red;
	Pixel green;
	Pixel blue;

	void reset()
	{
		red.reset();
		green.reset();
		blue.reset();
	}
};

#endif
