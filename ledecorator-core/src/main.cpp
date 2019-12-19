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
#include "apps/application.h"
#include "apps/console/console.h"
#include "apps/dummyapp/dummyapp.hpp"
#include "apps/games/life/lifegame.h"
#include "apps/games/snake/snakegame.h"
#include "apps/sensors/sensorsapp.h"

// 10s - f/1024 - 0x2faf0
// 1s - f/1024 - 0x4C4B
// 1/4s - f/1024 - 0x1313
// 1/16s - f/1024 - 0x4C5
// 1/32s - f/1024 - 262

using namespace dragberry::os;

Program* (*programms[])() =
{
	[]() -> Program* { return new DummyApp(); },
	[]() -> Program* { return new DummyApp(); }
};

int main()
{
	sei();
	uint8_t command = 0;
	while (true)
	{
		run(programms[command++ % 2]);
	}
}
