#ifndef CHARACTERS_H_
#define CHARACTERS_H_

#include <stdint.h>

struct Character
{
	const uint8_t width;
	const uint8_t height;
	const uint8_t data[height][width];
};

const uint8_t A[7][4] = {
		{0,1,1,0},
		{1,0,0,1},
		{1,0,0,1},
		{1,1,1,1},
		{1,0,0,1},
		{1,0,0,1},
		{1,0,0,1},
};

const uint8_t B[7][4] = {
		{1,1,1,0},
		{1,0,0,1},
		{1,0,0,1},
		{1,1,1,0},
		{1,0,0,1},
		{1,0,0,1},
		{1,1,1,0},
};

const uint8_t C[7][4] = {
		{0,1,1,1},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{1,0,0,0},
		{0,1,1,1},
};

const uint8_t D[7][4] = {
		{1,1,1,0},
		{1,0,0,1},
		{1,0,0,1},
		{1,0,0,1},
		{1,0,0,1},
		{1,0,0,1},
		{1,1,1,0},
};


#endif
