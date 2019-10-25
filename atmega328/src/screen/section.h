#ifndef SECTION_H_
#define SECTION_H_

#include <avr/io.h>
#include "color.h"

class Section
{
public:
	Color red;
	Color green;
	Color blue;

	void reset()
	{
		red.reset();
		green.reset();
		blue.reset();
	}
};

#endif
