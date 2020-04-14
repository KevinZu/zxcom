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

	#define PACKED __attribute((packed))__

#endif


//	ctrlInfo:
//		dir(2bit):			00---request;01---response
//		msg type(3bit):		000---sync;010---async;001---broadcast
//		reserved(3bit):
//		message id(16bit):

struct packet_content{
	CTRL_INFO_TYPE ctrlInfo;
	char data[];
} PACKED;


typedef struct packet_content packet_t;


#ifdef _MSC_VER

	#pragma pack(pop)

#endif



#endif

