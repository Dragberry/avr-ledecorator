#include "snakegame.h"
#include <stdlib.h>
#include "../../../screen/color.h"

SnakeGame::SnakeGame(
		uint8_t height,
		uint8_t width,
		uint8_t color_field,
		uint8_t color_snake,
		uint8_t color_food
		)
{
	this->isImageBuilt = false;
	this->height = height;
	this->width = width;
	this->color_field = color_field;
	this->color_snake = color_snake;
	this->color_food = color_food;
	this->snake = new Snake(height, width, 4, Direction::Right);
	this->food = NULL;
	this->state = true;
}

SnakeGame::~SnakeGame()
{
	delete food;
	delete snake;
}

bool SnakeGame::is_running()
{
	return state && snake->getLength() < height;
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
		image[food->y][food->x] = color_food;
		SnakeSection *head = snake->getHead();
		image[head->y][head->x] = color_snake;
		SnakeSection *tail = snake->getTail();
		image[tail->y][tail->x] = color_field;
	}
	else
	{
		for (uint8_t row = 0; row < height; row++)
		{
			for (uint8_t cell = 0; cell < width; cell++)
			{
				if (snake->isHere(row, cell))
				{
					image[row][cell] = color_snake;
				}
				else if (food->isHere(row, cell))
				{
					image[row][cell] = color_food;
				}
				else
				{
					image[row][cell] = color_field;
				}
			}
		}
		isImageBuilt = true;
	}
}
