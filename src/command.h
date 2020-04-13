#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "local_comm.h"

typedef struct  {
	int (*add_command)(command_handler_t handler);
}command_manager_t;

#endif

