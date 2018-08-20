#include "aggregator.h"
#include "subscriber.h"
#include "operator.h"

#include <stdlib.h> /*malloc*/
#include <string.h> /*strcmp*/
#include <stdio.h> /*sscanf*/
#include <assert.h>

#define MAX_IMSI 16
#define MAX_OP 65
#define MAX_DURATION 7

struct Aggregator
{
	Subscriber* m_subscriber;
	Operator*   m_operator;

};


struct Cdr
{
	char m_IMSI[MAX_IMSI]; /* max 15 digits - the key*/
	char m_MSISDN[MAX_IMSI]; /*max 15 digs*/
	char m_subscriberIMEI[MAX_IMSI]; /*max 15 digs*/
	char m_opBrandName[MAX_OP]; /*cellcome*/
	char m_opMCCmncTuple[MAX_DURATION]; /*5-6 digs*/
	char m_callType[7]; /*MOC/MTC/SMS_MT...*/
	char m_callDate[11]; /* 20/03/1989 */ 
	char m_callTime[9]; /*22:10:00*/
	char m_duration[MAX_DURATION]; /*30sec*/
	char m_download[32];
	char m_upload[32];
	char m_partyMSISDN[MAX_IMSI];
	char m_partyOp[MAX_OP];

};





Aggregator* AggCreate(size_t _subCapacity, size_t _opCapacity)
{
	Aggregator* agg = (Aggregator*)malloc(sizeof(Aggregator));
	assert(NULL != agg);
	
	agg->m_subscriber = SubscriberCreate(_subCapacity);
	agg->m_operator = OperatorCreate(_opCapacity);
	assert(NULL != agg->m_subscriber && NULL != agg->m_operator); 
	
	return agg;
}



void AggDestroy(Aggregator* _agg);




/*static void BillingFill(Cdr* _cdr, Subscriber* _sub)*/
/*{*/
/*	Func(_cdr->m_IMSI, _cdr->m_CallType, _cdr->flag, _cdr->data)*/
/*}*/



static size_t DataSet(Cdr* _cdr)
{
	size_t data;
	
	if (( ! strcmp(_cdr->m_callType, "SMS_MO")) || ( ! strcmp(_cdr->m_callType, "SMS_MT")))
	{
		data = 1;
	}

	if (*_cdr->m_duration != 0 && *_cdr->m_duration != '\0' && *_cdr->m_duration != '0')
	{
		sscanf(_cdr->m_duration, "%u", &data);
	}
	
	if (*_cdr->m_download != 0 && *_cdr->m_download != '\0' && *_cdr->m_download != '0')
	{
		sscanf(_cdr->m_download, "%u", &data);
	}
	
	if (*_cdr->m_upload != 0 && *_cdr->m_upload != '\0' && *_cdr->m_upload != '0')
	{
		sscanf(_cdr->m_upload, "%u", &data);
	}
	
	return data;
}



static int InFlagSet(Cdr* _cdr)
{
	int inFlag = 0;
	
	if ( ! strcmp(_cdr->m_opBrandName, _cdr->m_partyOp))
	{
		inFlag = 1;
	}

	if (*_cdr->m_download)
	{
		inFlag = 1;
	}

	return inFlag;
}




void CdrInsert(void* _record, void* _context)
{
	int inFlag = 0;
	size_t data = 0;
	Aggregator* agg = _context;
	Cdr* cdr = _record;
	
	data = DataSet(cdr);
	inFlag = InFlagSet(cdr);
	
	SubInsert(agg->m_subscriber, cdr->m_IMSI, cdr->m_callType, data, inFlag);
	
	OperatorInsert(agg->m_operator, cdr->m_opMCCmncTuple, cdr->m_callType, data);
	
/*	free(_record);  */
}


int AggFind(const Aggregator* _agg, const void* _searchKey, void** _pValue, int _flag)
{
	assert(NULL != _agg);

	if (_flag)
	{
		return SubFind(_agg->m_subscriber, _searchKey, _pValue);
	}
	
	return OperatorFind(_agg->m_operator, _searchKey, _pValue);
}



void AggForEach(const Aggregator* _agg, int _func(const void*, void*, void*), void* _context) /* use for file record */
{
	assert(NULL != _agg);

	SubForEach(_agg->m_subscriber, _func, _context);

}


