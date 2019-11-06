#include "application.h"

Application::~Application() {}

Application& Application::operator++ ()
{
	time++;
	increment();
	return *this;
}
