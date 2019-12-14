#ifndef COMMANDS_H_
#define COMMANDS_H_

#define TOTAL_WORKERS 2

#define COMMAND_MASK_INDICATOR 0b01000000
#define COMMAND_MASK 0b00111111
#define mask_command(command)			(command | COMMAND_MASK_INDICATOR)
#define is_command(byte) 				(byte & COMMAND_MASK_INDICATOR)
#define check_command(byte, command)	(byte == command)
#define extract_command(byte) 			(byte & COMMAND_MASK)
#define is_worker_command(command) 		(command < TOTAL_WORKERS)

#define CR '\r'
#define LF '\n'
#define CMD_DEFAULT 	0b00000000	// @
#define CMD_FILL_SCREEN 0b00000001	// 1
#define CMD_INIT_SCREEN 0b00111111	// 63

#endif
