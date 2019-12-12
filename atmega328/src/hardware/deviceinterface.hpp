#ifndef DEVICEINTERFACE_HPP_
#define DEVICEINTERFACE_HPP_

#include <stdint.h>
#include <stdlib.h>

#include "datainterface.hpp"
#include "timerinterface.hpp"

class DeviceInterface
{
protected:
	DataInterface* data_interface = NULL;

	TimerInterface* timer_interface = NULL;

public:
	DeviceInterface() { }

	virtual ~DeviceInterface() { }

	void register_timer_interface(TimerInterface* timer_interface)
	{
		this->timer_interface = timer_interface;
	}

	void register_data_interface(DataInterface* data_interface)
	{
		this->data_interface = data_interface;
	}

	virtual void init() = 0;

	virtual void stop() = 0;

	virtual void start_row() const = 0;

	virtual void send_byte(const uint8_t byte) const = 0;

	virtual void complete_row() const = 0;

};

#endif
