#ifndef SECTION_H_
#define SECTION_H_

#include "colors.h"

class Section
{
public:
	Colors red;
	Colors green;
	Colors blue;

	void reset()
	{
		red.reset();
		green.reset();
		blue.reset();
	}
};

#endif
