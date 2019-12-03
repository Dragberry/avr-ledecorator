#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "lib/screen/colors.h"
#include "lib/screen/definitions.h"
#include "apps/application.h"
#include "apps/console/console.h"
#include "apps/games/life/lifegame.h"
#include "apps/games/snake/snakegame.h"
#include "apps/sensors/sensorsapp.h"
#include "hardware/screen/screeninterface.hpp"
#include "hardware/uart/uart.hpp"

ScreenInterface screen_interface = ScreenInterface();

Application* app;
//Application* app = new SnakeGame(SCREEN_HEIGHT, SCREEN_WIDTH, CYAN, YELLOW, RED);

void setup()
{
	DDRC = 0xf;

	// CTC
	TCCR1A |= (0<<WGM10);
	TCCR1A |= (0<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (0<<WGM13);
	// 000 - f
	// 100 - f/256
	// 101 - f/1024
	TCCR1B |= (1<<CS12);
	TCCR1B |= (0<<CS11);
	TCCR1B |= (1<<CS10);
	TIMSK1 |= (1<<OCIE1A);
	// 10s - f/1024 - 0x2faf0
	// 1s - f/1024 - 0x4C4B
	// 1/4s - f/1024 - 0x1313
	// 1/16s - f/1024 - 0x4C5
	// 1/32s - f/1024 - 262
	OCR1A = 0x4c4b;


	// data send timer
	TCCR0A |= (0<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (0<<WGM02);
	// 001 - f
	// 010 - f/8
	// 011 - f/64
	// 100 - f/256
	// 101 - f/1024
	TCCR0B |= (1<<CS02);
	TCCR0B |= (0<<CS01);
	TCCR0B |= (0<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	// 0.2s
	OCR0A = 0x0;

	sei();
}

int main()
{
	setup();
	while(1)
	{
		app = new Console();
		screen_interface.start_picture();
		while(app->is_running());
		delete app;
	}
}

ISR(TIMER0_COMPA_vect)
{

	if (screen_interface.is_image_being_transmitted)
	{
		screen_interface.send_next_byte();
	}
}

ISR(TIMER1_COMPA_vect)
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
