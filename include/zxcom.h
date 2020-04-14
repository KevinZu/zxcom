#ifndef __ZXCOM_H__
#define __ZXCOM_H__

/////////////////////////////// ERR NO. //////////////////////////////////////////
#define ERR_CMD_NUM_OVERFLOW             -10

/////////////////////////////// command //////////////////////////////////////////
#define COMMAND_TOTAL_NUMBER 5
#define COMMAND_ID_TYPE unsigned short

//////////////////////////////// 
typedef int (*command_handler_t)(void *para);
//////////////////////////////// callback ////////////////////////////////////////

/////////////////////////////////////////// API //////////////////////////////////

int ZxcomInit();
int ZxcomDeInit();

int ZxcomOnPacket(const int len,const char* data);
int ZxcomSendCommand(const char *cmd,const int len);



#endif

