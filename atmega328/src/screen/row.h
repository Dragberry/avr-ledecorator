#ifndef ROW_H_
#define ROW_H_

#include <stdint.h>

#include "definitions.h"
#include "section.h"

class Row
{
public:
	uint8_t index;
	uint8_t step;
	uint8_t low;
	uint8_t high;
	Section sections[SECTIONS];

	void reset()
	{
		step = 0;
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			sections[sectionIdx].reset();
		}
	}
};

#endif
