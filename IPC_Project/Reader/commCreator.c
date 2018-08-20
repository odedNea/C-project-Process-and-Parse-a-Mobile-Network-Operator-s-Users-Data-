#include "commCreator.h"
#include "../Shared/channel.h"

#include <stdlib.h>
#include <assert.h>

#define MSG_SIZE 512

struct CommCreator
{
	int m_msqID;
};




CommCreator* CommCreatorCreate(char* _name)
{
	CommCreator* comm;
	assert(_name);

	comm = (CommCreator*)malloc(sizeof(CommCreator));
	assert(comm);
	
	comm->m_msqID = ChannelCreate(_name);
	
	return comm;
}



void SendComm(CommCreator* _comm, void* _record)
{
	assert(_comm && _record);

	RecordSend(_comm->m_msqID, _record, MSG_SIZE);
}



void CommCreatorDestroy(CommCreator* _comm)
{
	ChannelDestroy(_comm->m_msqID);
	
	free(_comm);
}
