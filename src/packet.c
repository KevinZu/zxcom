#include "packet.h"
#include "command.h"
#include "ucomlib.h"



int OnPacketRecv(const char *pack,const int len)
{
	int ret;
	unsigned int datalen;

	char req[100];
	handler_param_t param;
	
	packet_t *packet = (packet_t *)pack;

	unsigned int dir = GET_DIR(packet->ctrlInfo);
	unsigned int msg_type = GET_MSG_TYPE(packet->ctrlInfo);
	COMMAND_ID_TYPE cmd_id = (COMMAND_ID_TYPE)GET_MSG_ID(packet->ctrlInfo);

	if(dir == DIR_REQUEST) {
		command_handler_t handler = g_command_manager.get_command(cmd_id);
		if(handler == NULL){
			return ERR_CMD_NOT_EXIST;
		}

		ret = handler(packet->data);
		if(ret != 0) {

		}
	} else if(dir == DIR_RESPONSE) {
		if(msg_type == MSG_TYPE_ASYNC) {
			ret = CommGetMsg((int)cmd_id,req,&datalen);
			if(ret != 0) {
				return ret;
			}
			param.req = req;
			param.res = packet->data;

			command_handler_t handler = g_command_manager.get_command(cmd_id);
			if(handler == NULL){
				return ERR_CMD_NOT_EXIST;
			}

			ret = handler(&param);
			if(ret != 0) {

			}
		}
	}

	return 0;
}



