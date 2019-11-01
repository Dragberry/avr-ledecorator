#ifndef TIMERINTERFACE_H_
#define TIMERINTERFACE_H_

class TimerInterface
{
public:
	virtual ~TimerInterface() {}

	virtual void launch() = 0;

	virtual void stop() = 0;
};

#endif
