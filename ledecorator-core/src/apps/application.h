#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdint.h>
#include "lib/screen/definitions.h"
#include "../hardware/screen/screeninterface.h"

class Application
{

protected:
	uint16_t time;

	virtual void increment() = 0;

public:
	virtual ~Application();

	Application& operator++ ();

	virtual bool is_running();

	virtual void build_image(ScreenInterface& screen_interface) const;

};

#endif
