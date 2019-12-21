#ifndef DRAGBERRY_OS_HPP_
#define DRAGBERRY_OS_HPP_

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

		template <typename ProgramLoader>
		static void run(ProgramLoader&& loader)
		{
			Program* process = loader();
			process->run();
			delete process;
		}
	}
}

#endif
