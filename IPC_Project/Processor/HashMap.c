#include "list.h"
#include "list_itr.h"
#include "list_functions.h"
#include "HashMap.h"

#include <stdlib.h> /* calloc */
#include <math.h> /* sqrt */ 
#include <assert.h>

#define EMPTY_HASH 0
#define TRUE 1
#define FALSE 0
#define KEY_SIZE 32
#define VALUE_SIZE 256


typedef struct
{
	const void* m_key;
	const void* m_value;
}KeyValuePair; 



typedef struct
{
	const void* m_refKey;
	EqualityFunction m_eqFunc;
}KeyFuncPair; 


struct HashMap
{
	/* original size given by the client */
    size_t     m_capacity;

    /* real array size */
    size_t     m_hashSize;

    /* maximum number of collisions */
    size_t     m_maxCollisions;

    /* total insertions count */
    size_t     m_numberOfInsertions;

    /* total collisions until now */
    size_t     m_totalCollisions;
    
    HashFunction     m_hashFunction;
    
    EqualityFunction       m_equalityFunction; 
    
    List**      m_bucket; 

};




static size_t CalcHashSize(size_t num)
{
	size_t i = 2;
	size_t flag = 0;

	while(! flag)
	{	
		for (i = 2; i <= num-1; i++)
		{
			if (num % i == 0)
			{
				flag = 0;
				break;
			}
			else
			{
				flag = 1;
			}

		}
	++num;	
	}
	
	return num;
}



static int IsKeyEqual(const void *_checkKeyValuePair, const void *_refKeyFuncPair)
{
	assert(_refKeyFuncPair);
	assert(_checkKeyValuePair);

	return (((KeyFuncPair*)_refKeyFuncPair)->m_eqFunc(((KeyValuePair*)_checkKeyValuePair)->m_key, ((KeyFuncPair*)_refKeyFuncPair)->m_refKey));
}





static ListItr FindItr(List* _list, KeyValuePair* _pair, EqualityFunction _keyEqualFunc)
{
	KeyFuncPair refKeyFuncPair;
	ListItr endItr = ListItr_End(_list);
	ListItr foundItr;
	
	refKeyFuncPair.m_eqFunc = _keyEqualFunc;
	refKeyFuncPair.m_refKey = _pair->m_key;
	foundItr = ListItr_FindFirst(ListItr_Begin(_list), endItr, IsKeyEqual, &refKeyFuncPair);
	
	if (endItr == foundItr)
	{
		return NULL;
	}

	return foundItr;
}







static Map_Result HashInsert(HashMap* _map, KeyValuePair* _pair, size_t _insertIndx)
{
	if (EMPTY_HASH == _map->m_bucket[_insertIndx])
	{
		_map->m_bucket[_insertIndx] = List_Create();
		assert (_map->m_bucket[_insertIndx]);
	}
	else
	{
		if (NULL != FindItr(_map->m_bucket[_insertIndx], _pair, _map->m_equalityFunction))
		{
			return MAP_KEY_DUPLICATE_ERROR;
		}
	}
	
	++_map->m_numberOfInsertions;
	
	assert (LIST_SUCCESS == List_PushHead(_map->m_bucket[_insertIndx], _pair));
		
	return MAP_SUCCESS;
}





static void RemoveKeyValPair(ListItr* _itrToRmv, void** _pKey, void** _pValue)
{
	KeyValuePair* removedItem;
	removedItem = ListItr_Remove(_itrToRmv);
	*_pKey = (void*)removedItem->m_key;	
	*_pValue = (void*)removedItem->m_value;

	free(removedItem); /* original pair */
}



static Map_Result HashRemove(size_t _removeIndx, HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
	ListItr foundItr;
	KeyValuePair* pair;

	if (EMPTY_HASH == _map->m_bucket[_removeIndx])
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
	pair->m_key = _searchKey;
	foundItr = FindItr(_map->m_bucket[_removeIndx], pair, _map->m_equalityFunction);
	
	if (NULL == foundItr)
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	RemoveKeyValPair(foundItr, _pKey, _pValue);
	
	free(pair);
	return MAP_SUCCESS;
}



/************************************************************************/
/*							HashMap_Create*/
/************************************************************************/

HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	size_t hashSize;
	HashMap* hash = NULL;

	if (NULL == _keysEqualFunc || NULL == _hashFunc)
	{
		return NULL;
	}
		
	hashSize = CalcHashSize(_capacity);
	
	if ((hash = (HashMap*)calloc(1, sizeof(HashMap))) == NULL)
	{
		return NULL;
	}

	if (NULL == (hash->m_bucket = (List**)calloc(hashSize, sizeof(List*))))
	{
		free(hash);
		return NULL;
	}

	hash->m_capacity = _capacity;
	hash->m_hashSize = hashSize;
	hash->m_hashFunction = _hashFunc;
	hash->m_equalityFunction = _keysEqualFunc;
	
	return hash;
}


/************************************************************************/
/*							HashMap_Destroy                             */
/************************************************************************/

void HashMap_Destroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
	size_t bucketIdx = 0;
	void* pKey, *pValue;
	List* currentList;
	size_t hashSize = (*_map)->m_hashSize;

	if (NULL == _map)
	{
		return;
	}	
	
	for (bucketIdx = 0; bucketIdx < hashSize; ++bucketIdx)
	{
		currentList = (*_map)->m_bucket[bucketIdx];
		
		while (List_Size(currentList))
		{
			RemoveKeyValPair(ListItr_Begin(currentList), &pKey, &pValue);
			if (_keyDestroy)
			{
				_keyDestroy(pKey);
			}	
			
			if (_valDestroy)
			{
				_valDestroy(pValue);
			}	
			
		}
		if (currentList)
		{
			free(currentList);
		}	
	}
	
	free((*_map)->m_bucket);
	*_map = NULL;
	free(*_map);
	_map = NULL;
}





static void SpliceOldToNew(List* _currentOldList ,HashMap* _newMap)
{
	size_t newIndx = 0;
	ListItr firstChain;
	ListItr dest;
	void* chainData;
	List* listToInsert;

	while (List_Size(_currentOldList))
	{
		firstChain = ListItr_Begin(_currentOldList);
		chainData = ListItr_Get(firstChain);
		
		newIndx = _newMap->m_hashFunction(((KeyValuePair*)chainData)->m_key) % _newMap->m_hashSize;
		listToInsert = _newMap->m_bucket[newIndx];
		if (EMPTY_HASH == listToInsert)
		{
			listToInsert = List_Create();
			assert (listToInsert);
			_newMap->m_bucket[newIndx] = listToInsert;
		}

		dest = ListItr_End(listToInsert);

		ListItr_Splice(dest, firstChain, ListItr_Next(firstChain));
	}
	free(_currentOldList);
}




/************************************************************************/
/*							HashMap_Rehash                              */
/************************************************************************/

Map_Result HashMap_Rehash(HashMap *_map, size_t newCapacity)
{
	List** oldBucket;
	List* currentOldList;
	size_t oldBucketIdx = 0;
	size_t oldHashSize = _map->m_capacity;

	if (NULL == _map)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	_map->m_capacity = newCapacity;
	_map->m_hashSize = CalcHashSize(newCapacity);
	oldBucket = _map->m_bucket;
	_map->m_bucket = (List**)calloc(_map->m_hashSize, sizeof(List*));
	if (NULL == _map->m_bucket)
	{
		return MAP_ALLOCATION_ERROR;
	}

	for (oldBucketIdx = 0; oldBucketIdx < oldHashSize; ++oldBucketIdx)
	{
		currentOldList = oldBucket[oldBucketIdx];
		SpliceOldToNew(currentOldList, _map);
	
	}

 	free(oldBucket);
	return MAP_SUCCESS;
}





/************************************************************************/
/*							HashMap_Insert                              */
/************************************************************************/

Map_Result HashMap_Insert(HashMap* _map, const void* _key, const void* _value)
{
	size_t InsertIndx = 0;
	KeyValuePair* pair;

	if (NULL == _map || NULL == _value)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (NULL == _key)
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	InsertIndx = (*_map->m_hashFunction)(_key) % _map->m_hashSize;
	
	pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
	if (NULL == pair)
	{
		return MAP_ALLOCATION_ERROR;
	}
	
	pair->m_key = _key;
	pair->m_value = _value;

	return HashInsert(_map, pair, InsertIndx);;
}


/************************************************************************/
/*							HashMap_Remove                              */
/************************************************************************/

Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
	size_t removeIndx = 0;

	if (NULL == _map)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (NULL == _searchKey)
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	removeIndx = (*_map->m_hashFunction)(_searchKey) % _map->m_hashSize;

	return HashRemove(removeIndx, _map, _searchKey, _pKey, _pValue);
}


/************************************************************************/
/*							HashMap_Find                                */
/************************************************************************/

Map_Result HashMap_Find(const HashMap* _map, const void* _searchKey, void** _pValue)
{
	List* currentList;
	ListItr foundItr;
	size_t findIndx = 0;
	KeyValuePair* pairToFind;

	if (NULL == _map)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (NULL == _searchKey)
	{
		return MAP_KEY_NULL_ERROR;
	}
	
	findIndx = (*_map->m_hashFunction)(_searchKey) % _map->m_hashSize;
	currentList = _map->m_bucket[findIndx];
	
	if (NULL == currentList)
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	
	pairToFind = (KeyValuePair*)malloc(sizeof(KeyValuePair));
	if (NULL == pairToFind)
	{
		return MAP_ALLOCATION_ERROR;
	}

	pairToFind->m_key = _searchKey;
	
	foundItr = FindItr(currentList, pairToFind, _map->m_equalityFunction);
	
	if (NULL == foundItr)
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	pairToFind = ListItr_Get(foundItr);
	*_pValue = (void*)pairToFind->m_value;
	free(pairToFind);
	
	return MAP_SUCCESS;
}




/************************************************************************/
/*							HashMap_FindAndUpdate                       */
/************************************************************************/

Map_Result HashMap_FindAndUpdate(HashMap* _map, void* _searchKey, void* _addValue, UpdateFunc _Updatefunc)
{
	List* currentList;
	ListItr foundItr;
	void* valueToUpdate;
	size_t findIndx = 0;
	KeyValuePair* pairToFind;

	if (NULL == _map)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	if (NULL == _searchKey)
	{
		return MAP_KEY_NULL_ERROR;
	}
	pairToFind = (KeyValuePair*)malloc(sizeof(KeyValuePair));
	if (NULL == pairToFind)
	{
		return MAP_ALLOCATION_ERROR;
	}

	findIndx = (*_map->m_hashFunction)(_searchKey) % _map->m_hashSize;
	currentList = _map->m_bucket[findIndx];
	
	if (NULL == currentList) /*not found --> insert --> keep pair */
	{
		pairToFind->m_key = _searchKey;
		pairToFind->m_value = _addValue;
		HashInsert(_map, pairToFind, findIndx);
		return MAP_SUCCESS;
	}
	
	/*found --> use update function on extracted context*/
	pairToFind->m_key = _searchKey;
	foundItr = FindItr(currentList, pairToFind, _map->m_equalityFunction);
	
	if (NULL == foundItr)
	{
		return MAP_KEY_NOT_FOUND_ERROR;
	}
	pairToFind = ListItr_Get(foundItr);
	
	valueToUpdate = (void*)pairToFind->m_value;
	_Updatefunc(valueToUpdate, _addValue);
/*	free(pairToFind); ????  */
	
	return MAP_UPDATED;
}








/************************************************************************/
/*							HashMap_Size                                */
/************************************************************************/

size_t HashMap_Size(const HashMap* _map)
{	
	if (NULL == _map)
	{
		return 0;
	}

	return _map->m_numberOfInsertions;
}


static void SeparateKeyValue(void** _key, void** _value, void* _keyValPair)
{
	KeyValuePair* pair = _keyValPair;
	*_key = (char*)pair->m_key;
	*_value = (char*)pair->m_value;
}





static size_t VisitEachNode(List* _list, void* _context, KeyValueActionFunction _action)
{
	ListItr begin = ListItr_Begin(_list);
	ListItr end = ListItr_End(_list);
	void* key, *value;
	size_t count = 0;


	while (begin != end)
	{
		SeparateKeyValue(&key, &value, ListItr_Get(begin));
		
		if ( ! (_action(key, value, _context)))
		{
			return count;
		}
	
		++count;
		begin = ListItr_Next(begin);
	}
	
	return count;	
}

/************************************************************************/
/*							HashMap_ForEach*/
/************************************************************************/

size_t HashMap_ForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
	List* currentList;
	size_t hashSize = _map->m_hashSize;
	size_t i;
	size_t count = 0;

	if (NULL == _map)
	{
		return MAP_UNINITIALIZED_ERROR;
	}
	
	for (i = 0; i < hashSize; ++i)
	{
		currentList = _map->m_bucket[i];
	
		if (currentList)
		{
			count += VisitEachNode(currentList, _context, _action);
		}
		
	}

	return count;
}



