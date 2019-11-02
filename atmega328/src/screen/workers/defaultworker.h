#ifndef DEFAULTWORKER_H_
#define DEFAULTWORKER_H_

#include <stdint.h>
#include "worker.h"

class DefaultWorker : public Worker
{
public:
	uint8_t do_work(Screen* screen)
	{
		while (1)
		{
			uint8_t data = screen->data_interface->get_data_byte();
			if (data & 0b01000000)
			{
				uint8_t command = data & 0b00111111;
				if (command < 2)
				{
					return command;
				}
			}
		}
	}

};



#endif
