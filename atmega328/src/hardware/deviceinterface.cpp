#include <stdlib.h>
#include "deviceinterface.hpp"

DeviceInterface::DeviceInterface()
{
	data_interface = NULL;
	timer_interface = NULL;
}

DeviceInterface::~DeviceInterface() { }

void DeviceInterface::register_timer_interface(TimerInterface* timer_interface)
{
	this->timer_interface = timer_interface;
}

void DeviceInterface::register_data_interface(DataInterface* data_interface)
{
	this->data_interface = data_interface;
}
