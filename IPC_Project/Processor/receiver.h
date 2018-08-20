#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <stddef.h>

/** 
 * @brief Controlls both containers - Operator and subscriber
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct Receiver Receiver;



typedef void(*CdrInserts)(void* _record, void* _context); 



Receiver* ReceiverCreate(CdrInserts _func, void* _aggPtr, char* _key);


void* StartReceiving(void* _rec);


#endif /*_RECEIVER_H_*/
