#include "queryProcessor.h"
#include "aggregator.h"
#include "../Shared/channel.h"

#include <stdio.h>
#include <string.h>

#define CGF_SIZE 80
#define MSG_SIZE 512


typedef struct Billing
{
	char m_IMSI[16];
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


typedef struct CGFpair
{
	char m_funcNum;
	char m_query[64];

}CGFpair;







int AllSubReportFunc(const void* _key, void* _value, void* _context)
{
	FILE* fp;
	Billing* billing;
	static int flag = 0;
	printf("Imsi: %s Value: %s context: %s \n", (char*)_key, (char*)_value, (char*)_context);
	
	if (0 == flag)
	{
		if (NULL == (fp = fopen((char*)_key, "a+"))) 
		{
			return -1;
		}
		printf("opened file\n");
		flag = 1;
	}
	
	billing = _value;
		fprintf(fp, "%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",(char*)_key, billing->m_outCallInOp,
		billing->m_outCallOutOp, billing->m_inCallInOp, billing->m_inCallOutOp, billing->m_outSmsInOp,
		billing->m_outSmsOutOp, billing->m_inSmsInOp, billing->m_inSmsOutOp, billing->m_mbDownload, billing->m_mbUpload);
		printf("file %s was created\n", (char*)_key);
		
	return 0;
}




static void* ImsiRetrieve(Aggregator* _agg, char* _key)
{
	void* pValue;
	
	if (AggFind(_agg, _key, &pValue, 1))
	{
		printf("found\n");
		
		return pValue;
	}
	else
	printf("not found\n");
	
	return NULL;
}




int CGF(Aggregator* _agg, char* _key, char _funcNum, int _msqID)
{
	void* record;

	if (NULL == _key || NULL == _agg)
	{
		return -1;
	}

	switch ( _funcNum )
	 {
	 	case '1':            
            record = ImsiRetrieve(_agg, _key);
            if (record)
            {
            	RecordSend(_msqID, record, MSG_SIZE);
            }
            break;
        case '3':            
            AggForEach(_agg, AllSubReportFunc, NULL);
            break;
        default:            
            return -1;
            break;
    }


	return 0;
}


void* StartQueryP(void* _agg)
{
	Aggregator* agg = _agg;
	CGFpair CGFtoReceive;
/*	void* msgToReceive;*/
	int msqID = ChannelCreate("../UIchannel.txt");

	while (1)
	{
		RecordReceive(msqID, CGF_SIZE, &CGFtoReceive);
		CGF(agg, CGFtoReceive.m_query, CGFtoReceive.m_funcNum, msqID);
	}
	
	return NULL;
}















/*		printf("%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",(char*)_key, billing->m_outCallInOp, billing->m_outCallOutOp,*/
/*		billing->m_inCallInOp, billing->m_inCallOutOp, billing->m_outSmsInOp, billing->m_outSmsOutOp, billing->m_inSmsInOp,*/
/*		billing->m_inSmsOutOp, billing->m_mbDownload, billing->m_mbUpload);*/
/*		printf("file %s was created\n", (char*)_key);*/





/*	FILE* fp;*/

/*	if (NULL == (fp = fopen(_fileName, "a+"))) */
/*	{*/
/*		return -1;*/
/*	}*/
/*	fclose(fp);*/
/*static int ImsiRetrieve(Aggregator* _agg, char* _key, char* _fileName)*/
/*{*/
/*	FILE* fp;*/
/*	void* pValue;*/
/*	Cdr* cdr;*/

/*	if (NULL == (fp = fopen(_fileName, "a+"))) */
/*	{*/
/*		return -1;*/
/*	}*/
/*	*/
/*	AggFind(_agg, _key, &pValue, 1);*/
/*	cdr = pValue;*/
/*	*/
/*	fprintf(fp, "%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n", cdr, cdr, cdr, cdr, cdr, cdr, cdr, cdr, cdr, cdr, cdr );*/
/*	*/
/*	fclose(fp);*/
/*	return 0;*/
/*}*/







