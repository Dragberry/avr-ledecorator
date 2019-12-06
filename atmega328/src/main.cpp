#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "screen/atmega328/atmega328interface.h"
#include "screen/screen.h"

const Atmega328Interface m328 = Atmega328Interface();
Screen screen = Screen(m328, m328, m328);

int main()
{
	screen.launch();
}

ISR(TIMER0_COMPA_vect)
{
	screen.draw_row();
}
