#ifndef __PACKET_H__
#define __PACKET_H__

#include "zxcom.h"

#define CTRL_INFO_TYPE unsigned int
#define PACKET_DATA_SIZE 10
#define PACKET_SIZE (PACKET_DATA_SIZE + sizeof(CTRL_INFO_TYPE))


#ifdef _MSC_VER

	#pragma pack(push)
	#pragma pack(1)

	#define PACKED

#else

	#define PACKED  __attribute__((packed))

#endif



#define GET_DIR(x)					((x & 0xC000) >> 14)
#define GET_MSG_TYPE(x)				((x & 0x3800) >> 11)
#define GET_MSG_ID(x)				(x & 0x00ff)
#define SET_CTRL_INFO(dir,type,id)	((dir << 14) | (type << 11) | id)

#define DIR_REQUEST          0
#define DIR_RESPONSE         1

#define MSG_TYPE_SYNC        0
#define MSG_TYPE_ASYNC       2
#define MSG_TYPE_BROADCAST   1

//	ctrlInfo:
//		dir(2bit):			00---request;01---response
//		msg type(3bit):		000---sync;010---async;001---broadcast
//		reserved(3bit):
//		message id(16bit):

struct packet_content{
	CTRL_INFO_TYPE ctrlInfo;
	char data[PACKET_DATA_SIZE];
}PACKED;



struct cmd_content {
	COMMAND_ID_TYPE cmd_id;
	char param[PACKET_DATA_SIZE - sizeof(COMMAND_ID_TYPE)];
}PACKED;


#ifdef _MSC_VER

	#pragma pack(pop)

#endif

typedef struct packet_content packet_t;
typedef struct cmd_content cmd_content_t;

typedef struct {
	char *req;
	char *res;
}handler_param_t;



#endif

