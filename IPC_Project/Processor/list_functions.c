#include "list.h"
#include "list_itr.h"
#include "list_functions.h"

#include <assert.h>




 
ListItr ListItr_FindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, const void* _context)
{
	assert (NULL != _predicate);
	
	while (_begin != _end)
	{
	
		if ((_predicate(ListItr_Get(_begin), _context)))
		{
			return _begin;
		}
		
		_begin = ListItr_Next(_begin);
	}
	

	return _begin;
}



size_t ListItr_CountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	size_t count;
	assert (NULL != _begin && NULL != _end);
	count = 0;

	while (_begin != _end)
	{
		if ( ! (_predicate(ListItr_Get(_begin), _context)))
		{
			return count;
		}
	
		++count;
		_begin = ListItr_Next(_begin);
	}

	return count;
}



ListItr ListItr_ForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
	assert (NULL != _begin && NULL != _end);
	
	while (_begin != _end)
	{
		if ( ! (_action(ListItr_Get(_begin), _context)))
		{
			return _begin;
		}
	
		_begin = ListItr_Next(_begin);
	}

	return _begin;

}



/** 
 * @brief Sorts a sublist in place using the bubble sort algorithm
 * Complexity O(?)
 *
 * @params _begin : Iterator to sublist start
 * @params _end : Iterator to sublist end
 * @params _less : less compare function
 * 
 */
 
 
 static void Swap(ListItr _a, ListItr _b)
 {
 	void* aVal = ListItr_Get(_a);
 	void* bVal = ListItr_Get(_b);	
 	
 	ListItr_Set(_a, bVal);
 	ListItr_Set(_b, aVal);
 }
 
 
 
 
void ListItr_Sort(ListItr _begin, ListItr _end, LessFunction _less)
{
	int flag = 0;
	ListItr ptr = _begin;
		
	while (_begin != _end)
	{
		flag = 1;
		
		while (_begin != _end)
		{
			if (_less(_begin, ListItr_Next(_begin)))
			{
				Swap(_begin, ListItr_Next(_begin));	
				flag = 0;
			}
		
			_begin = ListItr_Next(_begin);
		}
		
		if (flag)
		{
			break;
		}
		
		_begin = ListItr_Next(ptr);
		ptr = _begin;
	}

}

ListItr ListItr_Splice(ListItr _dest, ListItr _begin, ListItr _end)
{
	void* data;
	ListItr itrToRemove;

	while (_begin != _end)
	{
		_end = ListItr_Prev(_end);
		itrToRemove = _end;
		data = ListItr_Remove(itrToRemove); 
		_dest = ListItr_InsertBefore(_dest, data);
	}

	return _dest;
}




/** 
 * @brief Merges two given sub lists into destination 
 * @details Merge elements from two sub lists defined by [_firstbegin.._firstEnd)
 * and [_secondBegin.._secondEnd) in optionaly sorted order using _less function.
 * merged elements will be inserted before _destBegin
 * if a _less function is provided then merge will be sorted
 * 
 * @warning this will removes all merged items from source ranges.
 * O(?)
 *  
 */
/*ListItr ListItr_Merge(ListItr _destBegin, ListItr _firstBegin, ListItr _firstEnd,*/
/*			ListItr _secondBegin, ListItr _secondEnd, LessFunction _less)*/
/*{*/
/*	void* dataFirst, *dataSecond;*/
/*	*/
/*	if (_less)*/
/*	{*/
/*		while (_firstBegin != _firstEnd && _secondBegin != _secondEnd)*/
/*		{*/
/*			dataFirst = ListItr_Remove(_firstBegin);*/
/*			dataSecond = ListItr_Remove(_secondBegin);*/
/*	*/
/*			if (flag && _less(dataFirst, dataSecond)*/
/*			{*/
/*				_destBegin = ListItr_InsertBefore(_destBegin, dataSecond);*/
/*			}*/
/*		}*/
/*		*/
/*	}*/


/*}*/


/** 
 * @brief Removes a sublist [_begin.._end) from a list and creates a new list
 * with all removed items.
 * O(?)
 *
 */
List* ListItr_Cut(ListItr _begin, ListItr _end)
{
	void* data;
	List* list;
	ListItr itrToRemove = _begin;
	list = List_Create();
	
	if (NULL == list)
	{
		return NULL;
	}
	
	
	while (_begin != _end)
	{
		_begin = ListItr_Next(_begin);
		data = ListItr_Remove(itrToRemove);
		itrToRemove = _begin;
		assert (LIST_SUCCESS == List_PushTail(list, data));
	}
	
	return list;
}



/** 
 * @brief Removes all duplicate elements from a sorted sublist [_begin.._end) 
 * and rertun a new list with all removed items.
 * O(?)
 * post condition: [_begin.._end) contains only unique elements
 */
List* ListItr_Unique(ListItr _begin, ListItr _end, EqualsFunction _equals)
{
	List* list;
	void* nextData;
	void* firstUnique;
	ListItr itrToRemove = _begin;
	firstUnique = ListItr_Get(_begin);
	list = List_Create();

	if (NULL == firstUnique || NULL == list)
	{
		return NULL;
	}
	
	_begin = ListItr_Next(_begin);
	while (_begin != _end)
	{
	
		nextData = ListItr_Get(_begin);	
		if (nextData == firstUnique)
		{
			assert (LIST_SUCCESS == List_PushTail(list, nextData));
			itrToRemove = _begin;
			_begin = ListItr_Next(_begin);
			ListItr_Remove(itrToRemove);
		}
		else
		{
			firstUnique = nextData;
			_begin = ListItr_Next(_begin);
		}
		
	}
	
	return list;
}












