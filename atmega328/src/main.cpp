#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#include "apps/application.h"
#include "apps/games/snake/snakegame.h"
#include "apps/games/life/lifegame.h"
#include "screen/atmega328/atmega328interface.h"
#include "screen/screen.h"
#include "screen/workers/byteterminalworker.h"
#include "screen/workers/defaultworker.h"
#include "screen/workers/pumpkinscreenworker.h"
#include "screen/workers/worker.h"

#define FCPU 20000000UL
#define USART_BAUDRATE  9600UL
#define UBRR ((FCPU / (USART_BAUDRATE * 16UL)) - 1)

const Atmega328Interface m328 = Atmega328Interface(UBRR);
Screen screen = Screen(&m328, &m328, &m328);
Worker* workers[TOTAL_WORKERS] =
{
	new DefaultWorker(),
	new ByteTerminalWorker()
};

int main()
{
	Worker* worker = workers[0];
	screen.launch();
	while(1)
	{
		worker = workers[worker->do_work(screen)];
	}
}

ISR(TIMER0_COMPA_vect)
{
	screen.draw_row();
}
