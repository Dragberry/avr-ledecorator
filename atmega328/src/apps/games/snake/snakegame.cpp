#include "snakegame.h"
#include <stdlib.h>
#include "../../../screen/color.h"

SnakeGame::SnakeGame(uint8_t height, uint8_t width)
{
	this->isImageBuilt = false;
	this->height = height;
	this->width = width;
	this->snake = new Snake(height, width, 4, Direction::Right);
	this->food = NULL;
	this->state = true;
}

SnakeGame::~SnakeGame()
{
	delete food;
	delete snake;
}

bool SnakeGame::is_going_on()
{
	return state && snake->getLength() < 10;
}

void SnakeGame::placeFood()
{
	uint8_t newX = rand() % width;
	uint8_t newY = rand() % height;
	if (food == NULL)
	{
		food = new Food(newX, newY);
	}
	else
	{
		food->x = newX;
		food->y = newY;
	}
}

void SnakeGame::increment()
{
	if (time % (rand() % 5 == 0)  == 0)
	{
		if (rand() % 2 == 0)
		{
			snake->turnRight();
		}
		else
		{
			snake->turnLeft();
		}
	}
	snake->move();
	if (!snake->isAlive())
	{
		state = false;
		return;
	}
	if (food == NULL || snake->eat(*food))
	{
		placeFood();
	}
}

void SnakeGame::build_image(uint8_t** image)
{
	if (isImageBuilt)
	{
		image[food->y][food->x] = CYAN;
		SnakeSection *head = snake->getHead();
		image[head->y][head->x] = CYAN;
		SnakeSection *tail = snake->getTail();
		image[tail->y][tail->x] = RED;
	}
	else
	{
		for (uint8_t row = 0; row < height; row++)
		{
			for (uint8_t cell = 0; cell < width; cell++)
			{
				image[row][cell] =
						snake->isHere(row, cell) || food->isHere(row, cell) ? CYAN : RED;
			}
		}
		isImageBuilt = true;
	}
}
