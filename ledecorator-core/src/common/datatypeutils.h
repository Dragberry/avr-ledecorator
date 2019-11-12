#ifndef DATATYPEUTILS_H_
#define DATATYPEUTILS_H_

#include <stdint.h>

void int_to_string(
			char* data,
			int number,
			const uint8_t pr_int);

void float_to_string(
		char* data,
		float number,
		const uint8_t pr_int,
		const uint8_t pr_float);

#endif
