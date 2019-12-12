#include "hardware/avr/atmega328device.hpp"
#include "screen/screeninterface.hpp"

Atmega328Device m328 = Atmega328Device();

ScreenInterface screen = ScreenInterface(m328);

int main()
{
	m328.init();
	while(1);
}
