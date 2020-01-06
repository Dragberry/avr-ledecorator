#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include "lib/avr/software/operators.hpp"
#include "display.hpp"
#include "drawablestring.hpp"
#include "fps.hpp"
#include "../../common/datatypeutils.h"

using namespace dragberry::os;

volatile uint32_t time = 0;
volatile uint32_t frames = 0;

DrawableString lbl_section = DrawableString(0, 0, 32, 8);
DrawableString fps_section = DrawableString(0, 8, 32, 8);
char fps_string[7];

void fps::init()
{
	// F / 1024
	sbi(TCCR2B, CS22);
	sbi(TCCR2B, CS21);
	sbi(TCCR2B, CS20);

	// CTC
	cbi(TCCR2B, WGM22);
	sbi(TCCR2A, WGM21);
	cbi(TCCR2A, WGM20);

	lbl_section.bg_color = BLACK;
	lbl_section.color = RED;
	lbl_section.set_string("FPS", 3);
	fps_section.bg_color = BLACK;
	fps_section.color = WHITE;
	fps_section.set_string(fps_string, 5);
}

void fps::start()
{
	time = 0;
	frames = 0;
	sbi(TIMSK2, OCIE2A);
	outb(OCR2A, 196);
}

void fps::stop()
{
	cbi(TIMSK2, OCIE2A);
	outb(OCR2A, 0);
}


void fps::count()
{
	frames++;
}

void fps::show()
{
	display::clear_screen(BLACK);

	int32_t full_fps = frames * 100 * 100 / time;
	itoa(full_fps / 100, fps_string, 10);
	uint8_t offset = 0;
	while (fps_string[offset] != '\0')
	{
	    offset++;
	}
	fps_string[offset++] = '.';
	itoa(full_fps % 100, fps_string + offset, 10);

	fps_section.set_string(fps_string, 5);
	fps_section.draw();
	lbl_section.draw();
	display::update_assured();
	time = 0;
	while (time < 500);
}

ISR(TIMER2_COMPA_vect)
{
	time++;
}
