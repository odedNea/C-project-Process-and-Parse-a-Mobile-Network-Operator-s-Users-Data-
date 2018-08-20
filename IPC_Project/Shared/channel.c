#include "channel.h"

#include <sys/msg.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h> /*sleep*/
#include <stdio.h> /*tests*/
#include <string.h>

#define MAX_MSG 300

typedef struct MsgBuf
{
	long m_type;
	char m_text[MAX_MSG];
}MsgBuf;


int ChannelCreate(char* _name)
{
	int msqid;
	key_t key;
	key = ftok(_name, 'b');
	printf("name = %s\n", _name);
	msqid = msgget(key, 0666 | IPC_CREAT);
	assert(-1 != msqid);

	return msqid;
}





int ChannelJoin(char* _name)
{
	int msqid;
	key_t key;
	assert(_name);
	key = ftok(_name, 'b');
	msqid = msgget(key, 0666 | IPC_CREAT);
	assert(-1 != msqid);
	
	return msqid;
}




void* RecordReceive(int _msqid, size_t _msgSize, void* _msg)
{
	MsgBuf* msgToRec;
	
	msgToRec = (MsgBuf*)malloc(sizeof(char)* _msgSize + 10);
	assert(msgToRec);
	msgToRec->m_type = 2;
	assert(-1 != (msgrcv(_msqid, msgToRec, _msgSize, 2, 0)));
	memcpy(_msg, msgToRec->m_text, _msgSize);
	free(msgToRec);
	
	return _msg; /*need to free*/
}



void RecordSend(int _msqid, void* _record, size_t _msgSize)
{
	MsgBuf* msgToSend;
	assert(_record);
	msgToSend = (MsgBuf*)malloc(sizeof(char)* _msgSize + 10);
	assert(msgToSend);
	
	msgToSend->m_type = 2;
	memcpy(msgToSend->m_text, _record, _msgSize);
	msgsnd(_msqid, msgToSend, _msgSize, 0);
}


void ChannelDestroy(int _msqid)
{
	msgctl(_msqid, IPC_RMID, NULL);
}


/*while (*msgToRec->m_text != '\0')*/
/*	{*/
/*		printf("%c", *msgToRec->m_text);*/
/*		(*(msgToRec->m_text))++;*/
/*	}*/
