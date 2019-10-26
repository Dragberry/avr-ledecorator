#include "direction.h"
#include "snakesection.h"

void SnakeSection::move(const uint8_t vBorder, const uint8_t hBorder)
{
	switch (direction) {
	case Up:
		moveUp(vBorder);
		break;
	case Right:
		moveRight(hBorder);
		break;
	case Down:
		moveDown(vBorder);
		break;
	case Left:
		moveLeft(hBorder);
		break;
	}
}

void SnakeSection::moveUp(const uint8_t vBorder)
{
	y = y == 0 ? vBorder : y - 1;
}

void SnakeSection::moveRight(const uint8_t hBorder)
{
	x = x == hBorder ? 0 : x + 1;
}

void SnakeSection::moveDown(const uint8_t vBorder)
{
	y = y == vBorder ? 0 : y + 1;
}

void SnakeSection::moveLeft(const uint8_t hBorder)
{
	x = x == 0 ? hBorder : x - 1;
}

