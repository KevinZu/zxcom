#ifndef __ZXCOM_H__
#define __ZXCOM_H__

/////////////////////////////// ERR NO. //////////////////////////////////////////
#define ERR_CMD_NUM_OVERFLOW             -10
#define ERR_CMD_NOT_EXIST                -11

/////////////////////////////// command //////////////////////////////////////////
#define COMMAND_TOTAL_NUMBER 2
#define RESPONSE_TOTAL_NUMBER 2
#define COMMAND_ID_TYPE unsigned short

//////////////////////////////// handler /////////////////////////////////////////
typedef int (*command_handler_t)(void *para);
//////////////////////////////// callback ////////////////////////////////////////

/////////////////////////////////////////// API //////////////////////////////////

int ZxcomInit();
int ZxcomDeInit();

int ZxcomAddCommand(COMMAND_ID_TYPE id,command_handler_t handler);
int ZxcomAddResponse(COMMAND_ID_TYPE id,command_handler_t handler);



int ZxcomOnPacket(const char *pack,const int len);
int ZxcomOnSendMsg(COMMAND_ID_TYPE cmdId,const char *param,const unsigned int paramLen,char *packet);  //when send async message,call it
int ZxcomOnSendResponse(COMMAND_ID_TYPE cmdId,const char *param,const unsigned int paramLen,char *packet);


#endif

