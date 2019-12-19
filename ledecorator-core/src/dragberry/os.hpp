#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

#include "io.hpp"
#include "../hardware/screen/screeninterface.hpp"

namespace dragberry
{
	namespace os
	{
		static ScreenInterface screen_interface;

		class Program
		{
		public:
			virtual ~Program() {}

			virtual void run() = 0;
		};

		template <typename ProgramLoader>
		void run(ProgramLoader&& loader)
		{
			Program* process = loader();
			process->run();
			delete process;
		}
	}
}

#endif
