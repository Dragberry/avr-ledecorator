#ifndef DUMMYAPP_HPP_
#define DUMMYAPP_HPP_

#include "lib/avr/hardware/timers.hpp"
#include "../../dragberry/os.hpp"
#include "../../dragberry/os/display.hpp"
#include "../../dragberry/os/drawablestring.hpp"
#include "../../dragberry/os/io.hpp"

using namespace dragberry::os;

class DummyApp : public Timers::T1::Handler
{
private:
	DrawableString string_h = DrawableString(0, 0, 32, 8);
	DrawableString string_l = DrawableString(0, 8, 32, 8);

	volatile uint8_t timer = 0;

	uint8_t field[16][32];

public:
	DummyApp()
	{
		string_h.set_string("DOBROE UTRO", 11);
		string_h.align = DrawableString::Align::LEFT;
		string_h.color = 0b00111111 & dragberry::io::read();
		string_l.set_string("SVINUSHKA", 9);
		string_l.align = DrawableString::Align::LEFT;
		string_l.color = 0b00111111 & dragberry::io::read();
		dragberry::os::display::connect();
		// 0.1 second
		Timers::T1::start(0x4C4, Timers::Prescaller::F_1024, this);
	}

	~DummyApp()
	{
		Timers::T1::stop();
		dragberry::os::display::disconnect();
	}

	void on_timer1_event()
	{
		timer++;
		string_h.update();
		string_l.update();
		string_h.draw();
		string_l.draw();
		dragberry::os::display::update_requsted();
	}

	void run()
	{
		while(timer < 64);
	}

	static void runner()
	{
		DummyApp app;
		app.run();
	}

};

#endif
