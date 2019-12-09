#ifndef TIMERS_HPP_
#define TIMERS_HPP_

#include <avr/io.h>
#include "../software/operators.hpp"

namespace Timers
{
	enum Prescaller
	{
		F_0,	// 001
		F_8,	// 010
		F_64,	// 011
		F_256,	// 100
		F_1024	// 101
	};

	class Handler
	{
	public:
		virtual ~Handler();

		virtual void handle() = 0;
	};

	namespace T0
	{
		static Handler* comp_a_handler = NULL;

		void start(uint8_t counter, Prescaller prescaller, Handler* handler);

		void stop();
	}

	namespace T1
	{
		static Handler* comp_a_handler = NULL;

		void start(uint16_t counter, Prescaller prescaller, Handler* handler);

		void stop();
	}
}

#endif
