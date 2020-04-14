#include "command.h"

static int Init()
{
	g_command_manager.current_command_number = 0;
	return 0;
}

static int AddCommand(COMMAND_ID_TYPE id,command_handler_t handler)
{
	if(g_command_manager.current_command_number >= COMMAND_TOTAL_NUMBER) {
		return ERR_CMD_NUM_OVERFLOW;
	}

	command_t *cmd = &g_command_manager.commands[g_command_manager.current_command_number];
	cmd->handler = handler;
	cmd->id = id;

	g_command_manager.current_command_number += 1;
	
	return 0;
}


static command_handler_t GetCommand(COMMAND_ID_TYPE id)
{
	int i;
	for(i = 0;i < COMMAND_TOTAL_NUMBER;i ++) {
		if(g_command_manager.commands[i].id == id) {
			return g_command_manager.commands[i].handler;
		}
	}
	return NULL;
}



command_manager_t g_command_manager = {
	.init = Init,
	.add_command = AddCommand,
	.get_command = GetCommand
};



