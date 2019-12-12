#ifndef TIMERINTERFACE_HPP_
#define TIMERINTERFACE_HPP_

class TimerInterface
{
public:
	virtual ~TimerInterface() { }

	virtual void on_timer_event() = 0;
};

#endif
