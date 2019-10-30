#ifndef COLORS_H_
#define COLORS_H_

#include <stdint.h>

#define BLACK 0b00000000
#define RED 0b00000011
#define GREEN 0b00001100
#define BLUE 0b00110000
#define YELLOW RED | GREEN
#define CYAN GREEN | BLUE
#define MAGENTA RED | BLUE
#define WHITE RED | GREEN | BLUE

class Colors
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


#endif /* COLORS_H_ */
