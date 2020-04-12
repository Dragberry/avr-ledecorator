#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "dragberry/os.hpp"

// 10s - f/1024 - 0x2faf0
// 1s - f/1024 - 0x4C4B
// 1/4s - f/1024 - 0x1313
// 1/16s - f/1024 - 0x4C5
// 1/32s - f/1024 - 262

using namespace dragberry::os;

int main()
{
    System::init();
    System::run();
}
