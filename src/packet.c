#include "packet.h"
#include "command.h"
#include "ucomlib.h"

unsigned int g_cur_msgid = 0;

static inline unsigned int get_new_msgid()
{
	ENTER_CRITICAL();
	if(g_cur_msgid >= 0x00ffffff){
		g_cur_msgid = 0;
	} else {
		g_cur_msgid += 1;
	}
	EXIT_CRITICAL();

	return g_cur_msgid;
}

int ZxcomOnPacket(const char *pack,const int len)
{
	int ret;
	unsigned int datalen;

	char req[100];
	handler_param_t param;
	
	packet_t *packet = (packet_t *)pack;

	unsigned int dir = GET_DIR(packet->ctrlInfo);
	unsigned int msg_type = GET_MSG_TYPE(packet->ctrlInfo);
	cmd_content_t *content = (cmd_content_t *)packet->data;
	unsigned int msg_id = (unsigned int)GET_MSG_ID(packet->ctrlInfo);

	if(dir == DIR_REQUEST) {
		command_handler_t handler = g_command_manager.get_command(content->cmd_id);
		if(handler == NULL){
			return ERR_CMD_NOT_EXIST;
		}

		ret = handler(packet->data);
		if(ret != 0) {

		}
	} else if(dir == DIR_RESPONSE) {
		if(msg_type == MSG_TYPE_ASYNC) {
			ret = CommGetMsg(msg_id,req,&datalen);
			if(ret != 0) {
				return ret;
			}
			param.req = req;
			param.res = packet->data;

			command_handler_t handler = g_command_manager.get_response(content->cmd_id);
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

int ZxcomOnSendMsg(COMMAND_ID_TYPE cmdId,const char *param,const unsigned int paramLen,char *packet)
{
	int i,ret;
	
	packet_t *pk = (packet_t *)packet;
	unsigned int msgid = get_new_msgid();
	
	pk->ctrlInfo = SET_CTRL_INFO(DIR_REQUEST, MSG_TYPE_ASYNC,msgid);

	cmd_content_t *cmd = (cmd_content_t *)pk->data;
	cmd->cmd_id = cmdId;
	
	for(i = 0;i < paramLen;i ++) {
		cmd->param[i] = param[i];
	}

	ret = CommSaveMsg(msgid,packet,paramLen + sizeof(COMMAND_ID_TYPE));
	if(ret != 0) {
		return ret;
	}

	return 0;
}

int ZxcomOnSendResponse(const void *recv,const char *param,const unsigned int paramLen,char *packet)
{
	int i,ret;

	const char *__mptr = (char *)recv;
	packet_t *recv_pk = (packet_t *)(__mptr - offsetof(packet_t,data));
	cmd_content_t *recv_content = (cmd_content_t *)recv_pk->data;
	
	packet_t *pk = (packet_t *)packet;
	
	pk->ctrlInfo = SET_CTRL_INFO(DIR_RESPONSE, MSG_TYPE_ASYNC,GET_MSG_ID(recv_pk->ctrlInfo));

	cmd_content_t *content = (cmd_content_t *)pk->data;
	content->cmd_id = recv_content->cmd_id;
	
	for(i = 0;i < paramLen;i ++) {
		content->param[i] = param[i];
	}

	return 0;
}




