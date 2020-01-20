#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "dragberry/os.hpp"
#include "apps/clock/clockapp.hpp"
#include "apps/games/life/lifegame.hpp"
#include "apps/games/snake/snakegame.h"
#include "apps/weather/weatherapp.hpp"

// 10s - f/1024 - 0x2faf0
// 1s - f/1024 - 0x4C4B
// 1/4s - f/1024 - 0x1313
// 1/16s - f/1024 - 0x4C5
// 1/32s - f/1024 - 262

using namespace dragberry::os;

const uint8_t PROGRAMMS = 1;

void (*programms[])() =
{
//        ClockApp::runner,
//		LifeGame::runner,
		WeatherApp::runner,
};

int main()
{
	sei();
	uint8_t command = 0;
	while (true)
	{
		dragberry::os::run(programms[command++]);
		 if (command >= PROGRAMMS)
		 {
			 command = 0;
		 }
	}
}
