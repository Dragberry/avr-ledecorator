#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

#include "os/display.hpp"

namespace dragberry
{
	namespace os
	{
		class Program
		{
		public:
			virtual ~Program();

			virtual void run() = 0;
		};

		template <typename Execution>
		static void run(Execution&& execution)
		{
			execution();
		}
	}
}

#endif
