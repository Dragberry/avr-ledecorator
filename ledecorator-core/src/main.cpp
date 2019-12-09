#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "lib/avr/hardware/timers.hpp"
#include "lib/avr/hardware/uart.hpp"
#include "lib/screen/colors.h"
#include "lib/screen/definitions.h"
#include "apps/application.h"
#include "apps/console/console.h"
#include "apps/games/life/lifegame.h"
#include "apps/games/snake/snakegame.h"
#include "apps/sensors/sensorsapp.h"

ScreenInterface screen_interface = ScreenInterface();

class Main : public Timers::Handler
{
	Application* app;
public:
	Main()
	{
		// 10s - f/1024 - 0x2faf0
		// 1s - f/1024 - 0x4C4B
		// 1/4s - f/1024 - 0x1313
		// 1/16s - f/1024 - 0x4C5
		// 1/32s - f/1024 - 262
//		UART::init(UART::BaudRate::B_9_600);
		Timers::T1::start(0x4C4, Timers::F_1024, this);
		sei();
	}

	~Main()
	{
		Timers::T1::stop();
		cli();
	}

	void perform()
	{
		while(1)
		{
			app = new SensorsApp();
			screen_interface.start_picture();
			while(app->is_running());
			delete app;
		}
	}

	void handle()
	{
		sei();
		++(*app);
		app->build_image(screen_interface);
		if (!screen_interface.is_image_being_transmitted)
		{
			screen_interface.switch_buffer();
			screen_interface.start_picture();
		}
	}
};

int main()
{
	while (1)
	{
		Main process = Main();
		process.perform();
	}
}
