#include "receiver.h"
#include "../Shared/channel.h"

#include <stdlib.h> /*malloc*/
#include <assert.h>
#include <stdio.h> /*test*/

#define MSG_SIZE 512

struct Receiver
{
	int m_msgID;
	void* m_aggPtr;
	CdrInserts m_insertFunc;
};




Receiver* ReceiverCreate(CdrInserts _func, void* _aggPtr, char* _key) /*key is fileName*/
{
	Receiver* rec;
	assert(_aggPtr && _func);
	rec = (Receiver*)malloc(sizeof(Receiver));
	assert(rec);
	
	rec->m_aggPtr = _aggPtr;
	rec->m_insertFunc = _func;
	rec->m_msgID = ChannelJoin(_key);
	
	return rec;
}



void* StartReceiving(void* _rec)
{
/*	Threadisize();*/
	void* msg;
	Receiver* rec = _rec;
	
	while(1) 
	{
		msg = malloc(sizeof(char) * MSG_SIZE);
		RecordReceive(rec->m_msgID, MSG_SIZE, msg);
		rec->m_insertFunc(msg, rec->m_aggPtr);
	}
	
	return NULL;
}



