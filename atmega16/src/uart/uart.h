#ifndef UART_H_
#define UART_H_

#include <stdint.h>

namespace UART
{
	void init();

	void send_byte(const uint8_t byte);

	uint8_t receive_byte();

}

#endif
