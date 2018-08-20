#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include <string.h> /* strcpy */
#include <assert.h>
#include "list.h"
#include "list_itr.h"
#include "list_functions.h"

#define TRUE 1
#define FALSE 0
#define NAME_SIZE 32


#define PRINT_RES(func) PrintStat(func(), #func)

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


typedef struct
{
	char m_name[NAME_SIZE];
	unsigned m_age;
	unsigned m_id;

}Person;




void PrintStat(int result, char *str)
{
	
	if (result)
	{
		printf("PASS: \t%s\n", str);
	} 
	else
	{
		printf("FAIL: \t%s\n", str);
	}
}


static void FillPerson(Person* _person, unsigned _noe)
{
	char names[][NAME_SIZE] = {"oded", "lior", "dvir", "yossi", "rita", "matilda", "ruti", "shimi", "yuval", "danielL"};
	unsigned i;

	for (i = 0; i < _noe; ++i)
	{
		_person[i].m_age = i + 10;
		_person[i].m_id = i + 303012291;
		strcpy(_person[i].m_name, names[i%10]);
	}

}


int Predicate(void * _element, void* _context)
{
	Person* elem = _element, *cont = _context;

	return elem->m_id == cont->m_id;
}


int ListPrintAction(void* _element, void* _context)
{
	Person* elem = _element, *cont = _context;

	printf("%s, %u, %u\n", elem->m_name, cont->m_age, elem->m_id);
	
	return 1;
}




 int isLess(void* _a, void* _b)
{
	void* aData, *bData;
	assert(NULL != _a && NULL != _b);

	aData = ListItr_Get(_a);
	bData = ListItr_Get(_b);
	
	return ((Person*)aData)->m_id < ((Person*)bData)->m_id;
}










int find_first_edges_ok()
{
	int i = 0;
	ListItr itrStart;
	ListItr itrEnd;
	ListItr result = NULL;

	List* list = NULL;
	Person* person;
	list = List_Create();

	person = (Person*)malloc(10 * sizeof(Person));
	
	FillPerson(person, 10);
	for (i = 0; i < 10; ++i)
	{
		if (List_PushHead(list, &person[i]) != LIST_SUCCESS)
		{
			return FALSE;
		}
	}
	
	itrStart = ListItr_Begin(list);
	itrEnd = ListItr_End(list);
	
	result = ListItr_FindFirst(itrStart, itrEnd, Predicate, &person[5]);
	
	printf("%s\n", ((Person*)result)->m_name);
	

	return TRUE;
}



int count_if_ok()
{
	int i = 0;
	ListItr itrStart;
	ListItr itrEnd;
	size_t count = 0;

	List* list = NULL;
	Person* person;
	list = List_Create();

	person = (Person*)malloc(10 * sizeof(Person));
	
	FillPerson(person, 10);
	for (i = 0; i < 10; ++i)
	{
		if (List_PushHead(list, &person[0]) != LIST_SUCCESS)
		{
			return FALSE;
		}
	}
	
	itrStart = ListItr_Begin(list);
	itrEnd = ListItr_End(list);
	
	count = ListItr_CountIf(itrStart, itrEnd, Predicate, &person[0]);
	
	printf("%lu\n", count);
	

	return TRUE;
}










int main()
{	





	PRINT_RES(find_first_edges_ok);
	PRINT_RES(count_if_ok);
/*	PRINT_RES(push_head_one_ok);*/
/*	PRINT_RES(push_tail_few_ok);*/
/*	PRINT_RES(push_head_not_initialized);*/
/*	PRINT_RES(push_tail_not_initialized);*/
/*	PRINT_RES(remove_one_from_head_ok);*/
/*	PRINT_RES(count_items_ok);*/
/*	PRINT_RES(list_itr_begin_end_ok);*/
/*	PRINT_RES(list_itr_equals_ok);*/
/*	PRINT_RES(list_itr_not_equals_ok);*/
/*	PRINT_RES(list_itr_next_ok);*/
/*	PRINT_RES(itr_get_one_ok);*/
/*	PRINT_RES(itr_set_one_ok);*/
/*	PRINT_RES(itr_insert_before_ok);*/
/*	PRINT_RES(itr_remove_two_ok);*/



/*	PRINT_RES(is_empty_ok);*/


	return 0;
}



