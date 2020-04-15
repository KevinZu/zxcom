#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "zxcom.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct {
	COMMAND_ID_TYPE id;
	command_handler_t handler;
}command_t;

typedef struct  {
	command_t commands[COMMAND_TOTAL_NUMBER];
	command_t response[COMMAND_TOTAL_NUMBER];
	unsigned int current_command_number;
	unsigned int current_response_number;

	int (*init)();
	int (*add_command)(COMMAND_ID_TYPE id,command_handler_t handler);
	int (*add_response)(COMMAND_ID_TYPE id,command_handler_t handler);
	command_handler_t (*get_command)(COMMAND_ID_TYPE id);
	command_handler_t (*get_response)(COMMAND_ID_TYPE id);
}command_manager_t;

extern command_manager_t g_command_manager;

#endif

