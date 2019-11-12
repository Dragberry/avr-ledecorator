#include "datatypeutils.h"
#include <math.h>

void int_to_string(
			char* data,
			int number,
			const uint8_t pr_int)
{
	for (uint8_t i = 0; i < pr_int; i++)
	{
		data[i] = (number % 10) + '0';
		number /= 10;
	}
	uint8_t i = 0, j = pr_int - 1, temp;
	while (i < j)
	{
		temp = data[i];
		data[i] = data[j];
		data[j] = temp;
		i++;
		j--;
	}
}

void float_to_string(
				char* data,
				float number,
				const uint8_t pr_int,
				const uint8_t pr_float)
{
	if (number < 0)
	{
		number = -number;
	}
	uint16_t ipart = (uint16_t) number;
	int_to_string(data, ipart, pr_int);
	data[pr_int] = '.';
	float fpart = number - (float) ipart;
	ipart = fpart * powf(10, pr_float);
	int_to_string(data + pr_int + 1, ipart, pr_float);
}
