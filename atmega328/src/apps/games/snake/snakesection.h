#ifndef SNAKESECTION_H_
#define SNAKESECTION_H_

#include <stdint.h>

#include "direction.h"

struct SnakeSection
{

public:
	uint8_t x = 0;
	uint8_t y = 0;
	Direction direction;

	void move(const uint8_t vBorder, const uint8_t hBorder);

private:
	void moveUp(const uint8_t vBorder);
	void moveRight(const uint8_t hBorder);
	void moveDown(const uint8_t vBorder);
	void moveLeft(const uint8_t hBorder);
};

#endif
