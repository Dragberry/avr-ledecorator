#ifndef DUMMYAPP_HPP_
#define DUMMYAPP_HPP_

#include "lib/avr/hardware/timers.hpp"

#include "../../dragberry/os.hpp"
#include "../../hardware/screen/drawablestring.hpp"

class DummyApp :
		public dragberry::os::Program,
		public Timers::T1::Handler
{
private:
	DrawableString string_h = DrawableString(0, 0, 32, 8);
	DrawableString string_l = DrawableString(0, 8, 32, 8);

	volatile uint8_t timer = 0;

public:
	DummyApp()
	{
		string_h.set_string("DOBROE UTRO", 11);
		string_h.align = DrawableString::Align::LEFT;
		string_h.color = 0b00111111 & dragberry::io::read();
		string_l.set_string("SVINUSHKA", 9);
		string_l.align = DrawableString::Align::LEFT;
		string_l.color = 0b00111111 & dragberry::io::read();
		Timers::T1::start(0x4c4, Timers::Prescaller::F_1024, this);
	}

	~DummyApp()
	{
		Timers::T1::stop();
	}

	void on_timer1_event()
	{
		timer++;
		string_h.update();
		string_l.update();
		string_h.draw(dragberry::os::screen_interface);
		string_l.draw(dragberry::os::screen_interface);
		dragberry::os::screen_interface.update();
	}

	void run()
	{
		while(timer < 64);
	}

};

#endif
