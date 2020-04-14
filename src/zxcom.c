#include "zxcom.h"
#include "command.h"
#include "packet.h"
#include "ucomlib.h"

/////////////////////////////////// UCOM CALLBACK /////////////////////////////////
int handler(const int event,const int msgId)
{
	switch (event) {
		case EV_TIMEOUT:
			DEBUG("ev_timeout\n");
			break;
		default:
			DEBUG("undefine event");
			break;	
	}
}


///////////////////////////////////// API /////////////////////////////////////////
int ZxcomInit()
{
	g_command_manager.init();
	CommuInit();
	return 0;
}

int ZxcomDeInit()
{
	return 0;
}






