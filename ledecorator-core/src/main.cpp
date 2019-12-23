#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "dragberry/os.hpp"
#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"
#include "lib/screen/colors.h"
#include "lib/screen/definitions.h"
#include "apps/dummyapp/dummyapp.hpp"
#include "apps/games/life/lifegame.hpp"
#include "apps/games/snake/snakegame.h"
#include "apps/sensors/sensorsapp.hpp"

// 10s - f/1024 - 0x2faf0
// 1s - f/1024 - 0x4C4B
// 1/4s - f/1024 - 0x1313
// 1/16s - f/1024 - 0x4C5
// 1/32s - f/1024 - 262

using namespace dragberry::os;

void (*programms[])() =
{
		SensorsApp::runner,
		LifeGame::runner
};


//Program* (*programms1[])() =
//{
//	[]() -> Program* { return new LifeGame(); },
//	[]() -> Program* { return new SensorsApp(); }
//};

int main()
{
	sei();
	uint8_t command = 0;
	while (true)
	{
		dragberry::os::run(programms[command++]);
		 if (command >= 2)
		 {
			 command = 0;
		 }
	}
}
