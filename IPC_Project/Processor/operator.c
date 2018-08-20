#include "operator.h"
#include "container.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


struct Operator
{
	Container* m_container;
};





typedef struct Settlement
{
	char      m_MCCmnc[7];
	size_t     m_inCall;
	size_t    m_outCall;
	size_t      m_inSms;
	size_t     m_outSms;

}Settlement;




Operator* OperatorCreate(size_t _opCapacity)
{
	Operator* operator;

	operator = (Operator*)malloc(sizeof(Operator));
	assert(operator);
	
	operator->m_container = ContainerCreate(_opCapacity);
	if (NULL == operator->m_container)
	{
		free(operator);
		return NULL;
	}
	
	return operator;
}


static void InCallSet(char* _callType, size_t _data, Settlement* _newStlmnt) /*use breaks*/
{
	if (! strcmp(_callType, "MTC"))
	{
		_newStlmnt->m_inCall = _data;
	}
}


static void OutCallSet(char* _callType, size_t _data, Settlement* _newStlmnt) 
{
	if (! strcmp(_callType, "MOC"))
	{
		_newStlmnt->m_outCall = _data;
	}
}


static void InSmsSet(char* _callType, size_t _data, Settlement* _newStlmnt) 
{
	if (! strcmp(_callType, "SMS-MT"))
	{
		_newStlmnt->m_inSms = _data;
	}
}


static void OutSmsSet(char* _callType, size_t _data, Settlement* _newStlmnt) 
{
	if (! strcmp(_callType, "SMS-MO"))
	{
		_newStlmnt->m_outSms = _data;
	}
}



Settlement* StlmntCreate(char* _mccMNC, char* _callType, size_t _data) /*static?*/
{
	Settlement* newStlmnt = (Settlement*)calloc(sizeof(Settlement), 1);
	assert(NULL != newStlmnt);
	
	strcpy(newStlmnt->m_MCCmnc, _mccMNC);
	
	InCallSet(_callType, _data, newStlmnt);
	OutCallSet(_callType, _data, newStlmnt);
	InSmsSet(_callType, _data, newStlmnt);
	OutSmsSet(_callType, _data, newStlmnt);
	
	return newStlmnt;
}



static void OperatorUpdate(void* _stlmntToUpdate, Settlement* _newStlmnt)
{
	Settlement* stlmntUpdate = _stlmntToUpdate;
	
	stlmntUpdate->m_inCall  += _newStlmnt->m_inCall;
	stlmntUpdate->m_outCall += _newStlmnt->m_outCall;
	stlmntUpdate->m_inSms   += _newStlmnt->m_inSms;
	stlmntUpdate->m_outSms  += _newStlmnt->m_outSms;
}



void OperatorInsert(Operator* _operator, char* _mccMNC, char* _callType, size_t _data)
{
	Settlement* newStlmnt;
	void* stlmntToUpdate; 
	assert(NULL != _operator);
	
	assert(newStlmnt = StlmntCreate(_mccMNC, _callType, _data));
	
	if (ContainerFind(_operator->m_container, _mccMNC, &stlmntToUpdate))
	{
		OperatorUpdate(stlmntToUpdate, newStlmnt); /*I free the allocated pValue in MapFind - care*/
		free(newStlmnt);
	}
	
	else
	{
		ContainerInsert(_operator->m_container, _mccMNC, newStlmnt);
	}
}

/************************************************************************/
/*							OperatorFind                                     */
/************************************************************************/

int OperatorFind(const Operator* _operator, const void* _searchKey, void** _pValue)
{
	assert(NULL != _operator);

	return ContainerFind(_operator->m_container, _searchKey, _pValue);
}


void OperatorForEach(const Operator* _operator) /* use for file record */
{
	assert(NULL != _operator);

	ContainerForEach(_operator->m_container, NULL, NULL);

}




void OperatorDestroy(Operator* _operator)
{
	ContainerDestroy(_operator->m_container);
	free(_operator);
}





