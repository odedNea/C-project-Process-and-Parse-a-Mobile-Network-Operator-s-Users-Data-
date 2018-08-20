#include "container.h"
#include "HashMap.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define MAX_IMSI 15

struct Container
{
	HashMap* m_map;
};







size_t HashFunc(const void* _key)
{
	unsigned long hash = 5381;
    char c;
    int i = 0;
    char strKey[MAX_IMSI];
    
    strcpy(strKey, (char*)_key);

    while (strKey[i])
    {
    	c = strKey[i];
        hash = ((hash << 5) + hash) + (unsigned long)c; 
        ++i;
	}
    return hash;
}



int EqualityFunc(const void* _checkedKey, const void* _contextKey)
{
	return (! (strcmp((char*)_checkedKey, (char*)_contextKey)));
}




Container* ContainerCreate(size_t _capacity)
{
	Container* container;

	container = (Container*)malloc(sizeof(Container));
	if (NULL == container)
	{
		return NULL;
	}
	
	container->m_map = HashMap_Create(_capacity, HashFunc, EqualityFunc);
	if (NULL == container->m_map)
	{
		return NULL;
	}
	
	return container;
}




int ContainerFind(const Container* _container, const void* _searchKey, void** _pValue)
{
	assert(NULL != _container);

	if (MAP_KEY_NOT_FOUND_ERROR == HashMap_Find(_container->m_map, _searchKey, _pValue))
	{
		return 0;
	}
	
	return 1;
}




void ContainerInsert(Container* _container, const void* _key, const void* _value)
{
	assert(NULL != _container);

	HashMap_Insert(_container->m_map, _key, _value); 
}



int ContainerFindAndUpdate(Container* _container, void* _searchKey, void* _value, int _func(void*, void*))
{
	assert(_container);

	if (MAP_UPDATED == HashMap_FindAndUpdate(_container->m_map, _searchKey, _value, _func))
	{
		return 1;
	}
	
	return 0;
}


void ContainerRemove(Container* _container,const void* _key, void** _pValue)
{
	HashMap_Remove(_container->m_map, _key, NULL, _pValue);

}



void ContainerDestroy(Container* _container)
{
	assert (NULL != _container);

	HashMap_Destroy(&_container->m_map, NULL, NULL);
	free(_container);
}



void ContainerForEach(const Container* _container, int _func(const void*, void*, void*), void* _context) /* use for file record */
{
	assert(NULL != _container);

	HashMap_ForEach(_container->m_map, _func, _context);

}







