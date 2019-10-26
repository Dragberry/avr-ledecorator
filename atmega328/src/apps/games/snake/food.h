#ifndef FOOD_H_
#define FOOD_H_

#include <stdint.h>

struct Food
{
	uint8_t x;
	uint8_t y;

	Food(uint8_t x, uint8_t y) : x(x), y(y) {};

	bool isHere(uint8_t row, uint8_t column)
	{
		return y == row && x == column;
	}
};

#endif /* FOOD_H_ */
