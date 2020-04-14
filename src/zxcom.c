#include "zxcom.h"
#include "command.h"

int ZxcomInit()
{
	g_command_manager.init();
	return 0;
}

int ZxcomDeInit()
{
	return 0;
}


int ZxcomOnPacket(const int len,const char* data)
{

	return 0;
}

int ZxcomSendCommand(const char *cmd,const int len)
{
	return 0;
}

