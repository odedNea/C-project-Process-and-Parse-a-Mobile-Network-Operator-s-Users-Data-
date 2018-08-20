#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#include <stddef.h>

/** 
 * @brief Controlls both containers - Operator and subscriber
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct Operator Operator;



Operator* OperatorCreate(size_t _opCapacity);


void OperatorDestroy(Operator* _operator);


void OperatorInsert(Operator* _operator, char* _mccMNC, char* _callType, size_t _data);


int OperatorFind(const Operator* _operator, const void* _searchKey, void** _pValue);


void OperatorForEach(const Operator* _operator);


#endif /*_OPERATOR_H_*/
