#include "command.h"


extern unsigned int g_cur_msgid;
static int Init()
{
	g_cur_msgid = 0;
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

static int AddResponse(COMMAND_ID_TYPE id,command_handler_t handler)
{
	if(g_command_manager.current_response_number >= RESPONSE_TOTAL_NUMBER) {
		return ERR_CMD_NUM_OVERFLOW;
	}

	command_t *res = &g_command_manager.response[g_command_manager.current_response_number];
	res->handler = handler;
	res->id = id;

	g_command_manager.current_response_number += 1;
	
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

static command_handler_t GetResponse(COMMAND_ID_TYPE id)
{
	int i;
	for(i = 0;i < RESPONSE_TOTAL_NUMBER;i ++) {
		if(g_command_manager.response[i].id == id) {
			return g_command_manager.response[i].handler;
		}
	}
	return NULL;
}



command_manager_t g_command_manager = {
	.init = Init,
	.add_command = AddCommand,
	.get_command = GetCommand,
	.add_response = AddResponse,
	.get_response = GetResponse
};



