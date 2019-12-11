#include "screen/atmega328/atmega328interface.h"
#include "screen/screen.h"

Atmega328Interface m328 = Atmega328Interface();

Screen screen = Screen(m328);

int main()
{
	m328.launch(&screen);
	screen.launch();
}
