#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdint.h>

class Application
{

protected:
	uint16_t time;

	virtual void increment() = 0;
	virtual void onAction() = 0;

public:
	virtual ~Application();

	Application& operator++ ();

	virtual bool isGoingOn();

	virtual void buildImage(uint8_t** image) = 0;

};

#endif
