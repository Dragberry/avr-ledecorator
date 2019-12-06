#ifndef ROW_H_
#define ROW_H_

#include <stdint.h>
#include "lib/screen/definitions.h"
#include "section.h"

class Row
{
public:
	uint8_t index;
	uint8_t brightness_step;
	uint8_t rows_state;
	Section sections[SECTIONS];

	void reset()
	{
		brightness_step = 0;
		for (uint8_t sectionIdx = 0; sectionIdx < SECTIONS; sectionIdx++)
		{
			sections[sectionIdx].reset();
		}
	}
};

#endif
