#ifndef SNAKE_H_
#define SNAKE_H_

#include <stdint.h>

#include "direction.h"
#include "food.h"
#include "snakesection.h"

class Snake
{

private:
	uint8_t fieldHeight;
	uint8_t fieldWidth;
	uint16_t maxLength;
	uint16_t length;
	Direction direction;
	SnakeSection* body;
	SnakeSection tail;

public:
	Snake(uint8_t fieldHeight, uint8_t fieldWidth, uint8_t initLength, Direction direction);
	~Snake();

	bool isHere(uint8_t row, uint8_t column);
	void setDirection(Direction direction);
	Direction getDirection();
	void move();
	void turnLeft();
	void turnRight();
	bool eat(const Food &food);
	uint16_t getLength();
	SnakeSection* getHead();
	SnakeSection* getTail();
	bool isAlive();
};

#endif
