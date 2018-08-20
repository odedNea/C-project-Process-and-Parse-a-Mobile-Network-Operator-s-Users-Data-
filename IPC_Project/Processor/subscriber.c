#include "subscriber.h"
#include "container.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define MAX_IMSI 16

struct Subscriber
{
	Container* m_container;
};





typedef struct Billing
{
	char m_IMSI[MAX_IMSI];
	size_t m_outCallInOp;
	size_t m_outCallOutOp;
	size_t m_inCallInOp;
	size_t m_inCallOutOp;
	size_t m_outSmsInOp;
	size_t m_outSmsOutOp;
	size_t m_inSmsInOp;
	size_t m_inSmsOutOp;
	size_t m_mbDownload;
	size_t m_mbUpload;

}Billing;







/*static void MemberConvert(char* _download, int* _subDownload)*/
/*{*/
/*	*/
/*	sscanf(_download, "%d", _subDownload);*/
/*}*/


/*void ParseConvert(Parse* _parse, SubParse* _subParse)*/
/*{*/
/*	strcpy(_subParse->m_IMSI, _parse->m_IMSI);*/
/*	*/
/*	if (_parse->m_download)*/
/*	{*/
/*		MemberConvert(_parse->m_download, &_subParse->m_mbDownload);*/
/*	}*/
/*	*/
/*	if (_parse->m_upload)*/
/*	{*/
/*		MemberConvert(_parse->m_upload, &_subParse->m_mbUpload);*/
/*	}*/

/*}*/


/*TODO UpdateFunc */



int SubUpdate(void* _inValue, void* _addValue)
{
	Billing* inValue = _inValue;
	Billing* addValue = _addValue;
	
	inValue->m_outCallInOp  += addValue->m_outCallInOp;
	inValue->m_outCallOutOp += addValue->m_outCallOutOp;
	inValue->m_inCallInOp   += addValue->m_inCallInOp;
	inValue->m_inCallOutOp  += addValue->m_inCallOutOp;
	inValue->m_outSmsInOp   += addValue->m_outSmsInOp;
	inValue->m_outSmsOutOp  += addValue->m_outSmsOutOp;
	inValue->m_inSmsInOp    += addValue->m_inSmsInOp;
	inValue->m_inSmsOutOp   += addValue->m_inSmsOutOp;
	inValue->m_mbDownload   += addValue->m_mbDownload;
	inValue->m_mbUpload     += addValue->m_mbUpload;
	
	printf("after update:  %u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n", inValue->m_outCallInOp, inValue->m_outCallOutOp,
		inValue->m_inCallInOp, inValue->m_inCallOutOp, inValue->m_outSmsInOp, inValue->m_outSmsOutOp, inValue->m_inSmsInOp,
		inValue->m_inSmsOutOp, inValue->m_mbDownload, inValue->m_mbUpload);
	
	return 1;
}

/************************************************************************/
/*							SubscriberCreate                            */
/************************************************************************/

Subscriber* SubscriberCreate(size_t _capacity)
{
	Subscriber* sub;

	sub = (Subscriber*)malloc(sizeof(Subscriber));
	if (NULL == sub)
	{
		return NULL;
	}
	
	sub->m_container = ContainerCreate(_capacity);
	if (NULL == sub->m_container)
	{
		free(sub);
		return NULL;
	}
	
	return sub;
}



static void OutCallSet(char* _callType, size_t _data, int _inFlag, Billing* _newBilling)
{
	if (! strcmp(_callType, "MOC"))
		{
			if (_inFlag)
			{
				_newBilling->m_outCallInOp = _data;
			}
			else
			{
				_newBilling->m_outCallOutOp = _data;
			}
		}
}


static void InCallSet(char* _callType, size_t _data, int _inFlag, Billing* _newBilling)
{
	if (! strcmp(_callType, "MTC"))
	{
		if (_inFlag)
		{
			_newBilling->m_inCallInOp = _data;
		}
		else
		{
			_newBilling->m_inCallOutOp = _data;
		}
	}
}
	
	
static void OutSmsSet(char* _callType, size_t _data, int _inFlag, Billing* _newBilling)
{
	if (! strcmp(_callType, "SMS_MO"))
	{
		if (_inFlag)
		{
			_newBilling->m_outSmsInOp = _data;
		}
		else
		{
			_newBilling->m_outSmsOutOp = _data;
		}
	}
}


static void InSmsSet(char* _callType, size_t _data, int _inFlag, Billing* _newBilling)
{
	if (! strcmp(_callType, "SMS_MT"))
	{
		if (_inFlag)
		{
			_newBilling->m_inSmsInOp = _data;
		}
		else
		{
			_newBilling->m_inSmsOutOp = _data;
		}
	}
}



static void GprsSet(char* _callType, size_t _data, int _inFlag, Billing* _newBilling)
{
	if (! strcmp(_callType, "GPRS"))
	{
		if (_inFlag)
		{
			_newBilling->m_mbDownload = _data;
		}
		else
		{
			_newBilling->m_mbUpload = _data;
		}
	}
}



Billing* BillingCreate(char* _imsi, char* _callType, size_t _data, int _inFlag) /*static?*/
{
	Billing* newBilling = (Billing*)calloc(sizeof(Billing), 1);
	assert(NULL != newBilling);
	
	strcpy(newBilling->m_IMSI, _imsi);
	
	OutCallSet(_callType, _data, _inFlag, newBilling); /*use breaks - only one field*/
	InCallSet(_callType, _data, _inFlag, newBilling);
	OutSmsSet(_callType, _data, _inFlag, newBilling);
	InSmsSet(_callType, _data, _inFlag, newBilling);
	GprsSet(_callType, _data, _inFlag, newBilling);
	
	return newBilling;
}



/************************************************************************/
/*							SubInsert                                   */
/************************************************************************/

void SubInsert(Subscriber* _sub, char* _imsi, char* _callType, size_t _data, int _inFlag)
{
	Billing* newBilling;
	
	assert(NULL != _sub);
	assert(newBilling = BillingCreate(_imsi, _callType, _data, _inFlag));
	
	if (ContainerFindAndUpdate(_sub->m_container, _imsi, newBilling, SubUpdate))
	{
/*		free(newBilling); */
		/* if updated, free the unneeded allocation */
	}	
}


/************************************************************************/
/*							SubForEach                                  */
/************************************************************************/


void SubForEach(const Subscriber* _sub, int _func(const void*, void*, void*), void* _context) /* use for file record */
{
	assert(NULL != _sub);

	ContainerForEach(_sub->m_container, _func, _context);

}


/************************************************************************/
/*							SubFind                                     */
/************************************************************************/

int SubFind(const Subscriber* _sub, const void* _searchKey, void** _pValue)
{
	assert(NULL != _sub);

	return ContainerFind(_sub->m_container, _searchKey, _pValue);
}





/************************************************************************/
/*							SubDestroy                                  */
/************************************************************************/

void SubDestroy(Subscriber* _sub)
{
	ContainerDestroy(_sub->m_container);
	free(_sub);
}




















							/*--SubInsert--*/
/*assert(newBilling = BillingCreate(_imsi, _callType, _data, _inFlag));*/
/*	*/
/*	*/
/*	if (ContainerFind(_sub->m_container, _imsi, &billingToUpdate))*/
/*	{*/
/*		SubUpdate(billingToUpdate, newBilling); */
/*		free(newBilling);*/
/*	}*/
/*	*/
/*	else*/
/*	{*/
/*		ContainerInsert(_sub->m_container, _imsi, newBilling);*/
/*	}*/


