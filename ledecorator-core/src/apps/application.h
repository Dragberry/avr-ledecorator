#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdint.h>
#include "../../../rgb-32x16-screen/lib/screen/definitions.h"

class Application
{

protected:
	uint16_t time;

	virtual void increment() = 0;

public:
	virtual ~Application();

	Application& operator++ ();

	virtual bool is_running();

	virtual void build_image(uint8_t(*image)[SCREEN_WIDTH]);

};

#endif
