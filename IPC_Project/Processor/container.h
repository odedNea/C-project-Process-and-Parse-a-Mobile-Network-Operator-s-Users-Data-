#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <stddef.h>

/** 
 * @brief Generic container 
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct Container Container;



Container* ContainerCreate(size_t _capacity);


void ContainerDestroy(Container* _container);


int ContainerFind(const Container* _container, const void* _searchKey, void** _pValue);


void ContainerInsert(Container* _container, const void* _key, const void* _value);


int ContainerFindAndUpdate(Container* _container, void* _searchKey, void* _value, int _func(void*, void*));



void ContainerForEach(const Container* _container, int _func(const void*, void*, void*), void* _context);







#endif /*_CONTAINER_H_*/
