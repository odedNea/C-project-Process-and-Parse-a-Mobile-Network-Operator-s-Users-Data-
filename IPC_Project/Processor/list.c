#include <stdlib.h> /* size_t, malloc */
#include <string.h> /* memset */
#include <assert.h>
#include "list.h"
#include "list_itr.h"


typedef struct Node Node;

struct Node
{
	void*       m_data;
	Node*       m_next;
	Node*       m_prev;
};

struct List
{
	Node m_head;
	Node m_tail;
};



/*
	Sub Functions Headers:
*/
static List* InitializeList(List* _list);

static void InitializePushHead(List* _list, Node* _node, void* _item);

static void InitializePushTail(List* _list, Node* _node, void* _item);

static void PopHead(List* _list);

static void PopTail(List* _list);


/*
Header Functions:
*/

List* List_Create(void)
{
	List* list = (List*)malloc(sizeof(List));

	if (list == NULL)
	{
		return NULL;
	}

	return InitializeList(list);
}



void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item))
{
	Node* nodeToFree = NULL;
	Node* nodePtr;

	if (NULL == _pList || NULL == *_pList)
	{
		return;
	}
	
	while (nodePtr->m_next != nodePtr) /* fix */
	{
		 nodePtr = (*(_pList))->m_head.m_next;
	
		if (_elementDestroy)
		{
			_elementDestroy(nodePtr->m_data);
		}
		
		nodeToFree = (nodePtr);
		PopHead(*_pList); 
		memset(nodeToFree, 0, 1);
		free(nodeToFree);
	}

	*_pList = NULL;
	free(*_pList);
	
	return;
}


List_Result List_PushHead(List* _list, void* _item)
{
	Node* node = NULL;
	if (NULL == _list)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	
	if (NULL == _item)
	{
		return LIST_NULL_ELEMENT_ERROR;
	}

	node = (Node*)malloc(sizeof(Node));
	if (NULL == node)
	{
		return LIST_ALLOCATION_ERROR;
	}
	
	InitializePushHead(_list, node, _item);
	
	return LIST_SUCCESS;
}



List_Result List_PushTail(List* _list, void* _item)
{
	Node* node = NULL;
	if (NULL == _list)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	
	if (NULL == _item)
	{
		return LIST_NULL_ELEMENT_ERROR;
	}

	node = (Node*)malloc(sizeof(Node));
	if (NULL == node)
	{
		return LIST_ALLOCATION_ERROR;
	}

	InitializePushTail(_list, node, _item);

	return LIST_SUCCESS;
}



List_Result List_PopHead(List* _list, void** _pItem)
{
	Node* nodeToFree = NULL;

	if (NULL == _list )
	{
		return LIST_UNINITIALIZED_ERROR;
	}

	if (_list->m_head.m_next == &(_list->m_tail))
	{
		return LIST_UNDERFLOW_ERROR;
	}

	if (_pItem)
	{
		*_pItem = _list->m_head.m_next->m_data;
	}
	
	nodeToFree = _list->m_head.m_next;

	PopHead(_list);
	memset(nodeToFree, 0, 1);
	free(nodeToFree);

	return LIST_SUCCESS;
}




List_Result List_PopTail(List* _list, void** _pItem)
{
	Node* nodeToFree = NULL;

	if (NULL == _list)
	{
		return LIST_UNINITIALIZED_ERROR;
	}

	if (_list->m_head.m_next == &(_list->m_tail))
	{
		return LIST_UNDERFLOW_ERROR;
	}

	if (_pItem)
	{
		*_pItem = _list->m_tail.m_prev->m_data;
	}
	
	nodeToFree = _list->m_tail.m_prev;

	PopTail(_list);
	memset(nodeToFree, 0, 1);
	free(nodeToFree);

	return LIST_SUCCESS;
}




size_t List_Size(const List* _list)
{
	size_t itemCount = 0;
	Node* nodePtr;

	if (NULL == _list)
	{
		return 0;
	}
	nodePtr = (Node*)&(_list->m_head);
	
	while (nodePtr->m_next != &(_list->m_tail))
	{
		++itemCount;
		nodePtr = nodePtr->m_next;
	}

	return itemCount;
}





/*
Sub Functions Body:
*/
static List* InitializeList(List* _list)
{
	_list->m_head.m_next = &(_list->m_tail);
	_list->m_tail.m_prev = &(_list->m_head);
	_list->m_head.m_prev = &(_list->m_head);
	_list->m_tail.m_next = &(_list->m_tail);
	
	return _list;
}

static void InitializePushHead(List* _list, Node* _node, void* _item)
{
	_node->m_data = _item;

	_node->m_next = _list->m_head.m_next;
	_node->m_prev = &(_list->m_head);
	_list->m_head.m_next->m_prev = _node;
	_list->m_head.m_next = _node;

	return;
}

	/* TODO connect logic of these 2 functions InitializePushHead, InitializePushTail*/
	
static void InitializePushTail(List* _list, Node* _node, void* _item)
{
	_node->m_data = _item;

	_node->m_next = &(_list->m_tail);
	_node->m_prev = _list->m_tail.m_prev;
	_list->m_tail.m_prev->m_next = _node;
	_list->m_tail.m_prev = _node;

	return;
}

static void PopHead(List* _list)
{
	_list->m_head.m_next->m_next->m_prev = &(_list->m_head);
	_list->m_head.m_next = _list->m_head.m_next->m_next;

	return;
}

static void PopTail(List* _list)
{
	_list->m_tail.m_prev->m_prev->m_next = &(_list->m_tail);
	_list->m_tail.m_prev = _list->m_tail.m_prev->m_prev;

	return;
}




ListItr ListItr_Begin(const List* _list)
{
	assert (NULL != _list);

	return _list->m_head.m_next;	
}



ListItr ListItr_End(const List* _list)
{
	assert (NULL != _list);

	return (const ListItr)(&(_list->m_tail)); /* I deleted the "_list->m_tail.m_prev" in order to compare last element of list...*/
}



int ListItr_Equals(const ListItr _a, const ListItr _b)
{
	return _a == _b;
}


ListItr ListItr_Next(ListItr _itr)
{
	assert (NULL != _itr);

	return ((Node*)_itr)->m_next;
}



ListItr ListItr_Prev(ListItr _itr)
{
	assert (NULL != _itr);

	return ((Node*)_itr)->m_prev;
}



void* ListItr_Get(ListItr _itr)
{
	assert (NULL != _itr);
	
	return ((Node*)_itr)->m_data;
}


void* ListItr_Set(ListItr _itr, void* _element)
{
	void* data;
	assert (NULL != _itr || NULL != _element);

	data = ((Node*)_itr)->m_data;
	((Node*)_itr)->m_data = _element;
	
	return data;
}


ListItr ListItr_InsertBefore(ListItr _itr, void* _element)
{
	Node* node;

	if (NULL == _itr || NULL == _element)
	{
		return NULL;
	}
	
	node = (Node*)malloc(sizeof(Node));
	if (NULL == node)
	{
		return NULL;
	}
	
	node->m_data = _element;

	node->m_next = (Node*)_itr;
	node->m_prev = ((Node*)_itr)->m_prev;
	((Node*)_itr)->m_prev->m_next = node;
	((Node*)_itr)->m_prev = node;

	return (ListItr)node;
}



void* ListItr_Remove(ListItr _itr)
{
	void* data;
	assert(NULL != _itr);

	data = ((Node*)_itr)->m_data;
	
	((Node*)_itr)->m_next->m_prev = ((Node*)_itr)->m_prev;
	((Node*)_itr)->m_prev->m_next = ((Node*)_itr)->m_next;
	memset(_itr, 0, 1);
	free(_itr);
	
	return data;
}






