#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.hpp"

Timers::T1::Handler::~Handler() { }

Timers::T0::Handler::~Handler() { }

#if defined (__AVR_ATmega16__)
void Timers::T0::start(uint8_t counter, Prescaller prescaller, Handler* handler)
{
	comp_a_handler = handler;

	sbi(TCCR0, WGM01);
	cbi(TCCR0, WGM00);

	switch(prescaller)
	{
	case F_8:
		cbi(TCCR0, CS02);
		sbi(TCCR0, CS01);
		cbi(TCCR0, CS00);
		break;
	case F_64:
		cbi(TCCR0, CS02);
		sbi(TCCR0, CS01);
		sbi(TCCR0, CS00);
		break;
	case F_256:
		sbi(TCCR0, CS02);
		cbi(TCCR0, CS01);
		cbi(TCCR0, CS00);
		break;
	case F_1024:
		sbi(TCCR0, CS02);
		cbi(TCCR0, CS01);
		sbi(TCCR0, CS00);
		break;
	default:
		// F_0
		cbi(TCCR0, CS02);
		cbi(TCCR0, CS01);
		sbi(TCCR0, CS00);
	}
	// enable interruption
	sbi(TIMSK, OCIE0);

	outb(OCR0, counter);
}

void Timers::T0::stop()
{
	cbi(TIMSK, OCIE0);

	cbi(TCCR0, CS02);
	cbi(TCCR0, CS01);
	cbi(TCCR0, CS00);

	cbi(TCCR0, WGM01);
	cbi(TCCR0, WGM00);

	comp_a_handler = nullptr;
}

ISR(TIMER0_COMP_vect)
{
	Timers::T0::comp_a_handler->on_timer0_event();
}

void Timers::T1::start(uint16_t counter, Prescaller prescaller, Handler* handler)
{
	comp_a_handler = handler;

	cbi(TCCR1B, WGM13);
	sbi(TCCR1B, WGM12);
	cbi(TCCR1A, WGM11);
	cbi(TCCR1A, WGM10);

	switch(prescaller)
	{
	case F_8:
		cbi(TCCR1B, CS12);
		sbi(TCCR1B, CS11);
		cbi(TCCR1B, CS10);
		break;
	case F_64:
		cbi(TCCR1B, CS12);
		sbi(TCCR1B, CS11);
		sbi(TCCR1B, CS10);
		break;
	case F_256:
		sbi(TCCR1B, CS12);
		cbi(TCCR1B, CS11);
		cbi(TCCR1B, CS10);
		break;
	case F_1024:
		sbi(TCCR1B, CS12);
		cbi(TCCR1B, CS11);
		sbi(TCCR1B, CS10);
		break;
	default:
		// F_0
		cbi(TCCR1B, CS12);
		cbi(TCCR1B, CS11);
		sbi(TCCR1B, CS10);
	}
	// enable interruption
	sbi(TIMSK, OCIE1A);

	outb(OCR1A, counter);
}

void Timers::T1::stop()
{
	cbi(TIMSK, OCIE1A);

	cbi(TCCR1B, CS12);
	cbi(TCCR1B, CS11);
	cbi(TCCR1B, CS10);

	cbi(TCCR1B, WGM13);
	cbi(TCCR1B, WGM12);
	cbi(TCCR1A, WGM11);
	cbi(TCCR1A, WGM10);

	comp_a_handler = nullptr;
}


ISR(TIMER1_COMPA_vect)
{
	Timers::T1::comp_a_handler->on_timer1_event();
}

#elif defined (__AVR_ATmega328P__)
void Timers::T0::start(uint8_t counter, Prescaller prescaller, Handler* handler)
{
	comp_a_handler = handler;


	cbi(TCCR0B, WGM02);
	sbi(TCCR0A, WGM01);
	cbi(TCCR0A, WGM00);

	switch(prescaller)
	{
	case F_8:
		cbi(TCCR0B, CS02);
		sbi(TCCR0B, CS01);
		cbi(TCCR0B, CS00);
		break;
	case F_64:
		cbi(TCCR0B, CS02);
		sbi(TCCR0B, CS01);
		sbi(TCCR0B, CS00);
		break;
	case F_256:
		sbi(TCCR0B, CS02);
		cbi(TCCR0B, CS01);
		cbi(TCCR0B, CS00);
		break;
	case F_1024:
		sbi(TCCR0B, CS02);
		cbi(TCCR0B, CS01);
		sbi(TCCR0B, CS00);
		break;
	default:
		// F_0
		cbi(TCCR0B, CS02);
		cbi(TCCR0B, CS01);
		sbi(TCCR0B, CS00);
	}
	// enable interruption
	sbi(TIMSK0, OCIE0A);

	outb(OCR0A, counter);
}

void Timers::T0::stop()
{
	cbi(TIMSK0, OCIE0A);

	cbi(TCCR0B, CS02);
	cbi(TCCR0B, CS01);
	cbi(TCCR0B, CS00);

	cbi(TCCR0B, WGM02);
	cbi(TCCR0A, WGM01);
	cbi(TCCR0A, WGM00);

	comp_a_handler = nullptr;
}

ISR(TIMER0_COMPA_vect)
{
	Timers::T0::comp_a_handler->on_timer0_event();
}

void Timers::T1::start(uint16_t counter, Prescaller prescaller, Handler* handler)
{
	comp_a_handler = handler;

	cbi(TCCR1B, WGM13);
	sbi(TCCR1B, WGM12);
	cbi(TCCR1A, WGM11);
	cbi(TCCR1A, WGM10);

	switch(prescaller)
	{
	case F_8:
		cbi(TCCR1B, CS12);
		sbi(TCCR1B, CS11);
		cbi(TCCR1B, CS10);
		break;
	case F_64:
		cbi(TCCR1B, CS12);
		sbi(TCCR1B, CS11);
		sbi(TCCR1B, CS10);
		break;
	case F_256:
		sbi(TCCR1B, CS12);
		cbi(TCCR1B, CS11);
		cbi(TCCR1B, CS10);
		break;
	case F_1024:
		sbi(TCCR1B, CS12);
		cbi(TCCR1B, CS11);
		sbi(TCCR1B, CS10);
		break;
	default:
		// F_0
		cbi(TCCR1B, CS12);
		cbi(TCCR1B, CS11);
		sbi(TCCR1B, CS10);
	}
	// enable interruption
	sbi(TIMSK1, OCIE1A);

	outb(OCR1A, counter);
}

void Timers::T1::stop()
{
	cbi(TIMSK1, OCIE1A);

	cbi(TCCR1B, CS12);
	cbi(TCCR1B, CS11);
	cbi(TCCR1B, CS10);

	cbi(TCCR1B, WGM13);
	cbi(TCCR1B, WGM12);
	cbi(TCCR1A, WGM11);
	cbi(TCCR1A, WGM10);

	comp_a_handler = nullptr;
}


ISR(TIMER1_COMPA_vect)
{
	Timers::T1::comp_a_handler->on_timer1_event();
}

#endif
