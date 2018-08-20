#ifndef _SUBSCRIBER_H_
#define _SUBSCRIBER_H_

#include <stddef.h>

/** 
 * @brief Controlls both containers - Operator and subscriber
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct Subscriber Subscriber;



Subscriber* SubscriberCreate(size_t _capacity);


void SubDestroy(Subscriber* _sub);


void SubInsert(Subscriber* _sub, char* _imsi, char* _callType, size_t _data, int _inFlag);


int SubFind(const Subscriber* _sub, const void* _searchKey, void** _pValue);


void SubForEach(const Subscriber* _sub, int _func(const void*, void*, void*), void* _context);

#endif /*_SUBSCRIBER_H_*/
