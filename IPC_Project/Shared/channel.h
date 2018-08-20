#ifndef _CHANNEL_H
#define _CHANNEL_H


/** 
 * @brief First module to be run in the "reader" executable which will manage and initialize
 * all other modules.
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

#include <stddef.h>

int ChannelCreate(char* _name);



int ChannelJoin(char* _name);



void* RecordReceive(int _msqid, size_t _msgSize, void* _msg);



void RecordSend(int _msqid, void* _record, size_t _msgSize);



void ChannelDestroy(int _msqid);








#endif /*_CHANNEL_H*/
