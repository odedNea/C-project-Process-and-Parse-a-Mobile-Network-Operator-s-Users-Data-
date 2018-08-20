#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <pthread.h>
#include <dirent.h>

#include "parse.h"
#include "Container.h"

#define CONTAINER_SIZE 1000000
#define SIZE 15

struct SubscriberBill
{
	char* m_IMSI;
	
	size_t m_outgoingCallWin;
	size_t m_incomingCallWin;
	size_t m_outgoingCallWout;
	size_t m_incomingCallWout;
	
	size_t m_sentSMSWin;
	size_t m_recSMSWin;
	size_t m_sentSMSWout;
	size_t m_recSMSWout;
	
	size_t m_MBdownload;
	size_t m_MBupload;
};

struct OperatorSettlemet
{
	char* m_PartyMSISDN;
	
	size_t m_incomingCalls;
	size_t m_outgoingCalls;
	
	size_t m_incomingSMS;
	size_t m_outgoingSMS;
};


struct cdrStruct
{
	const void* m_key;
	const void* m_value;
};
/*
	char** m_key;
	Parse** m_parse;
*/

void sbscrbrSumDataValues(void* _pValue, void* _incomingData);


SB** assignSubscriberEntry(Parse** _parse, size_t _parseSize);

char** assignKeyVal(Parse** _parse, size_t _parseSize);
void printSBValues(char* pValue);
void printSBBill(SB* subscriberBill);
void printSBreturn(SB* _subscriberBill);
void printToFile(FILE* _fp, Parse* _printParse);


int main()
{
	CDRContainer* cdrContainer;
	Parse** myParse;
	Parse* returnParse;
	SB** subscriberBill;
	SB* returnSB;
	
	char** keyVal;
	char* pValue;
	char* pKey;
	int i;
	
	FILE* fp;
	fp = fopen("CDR_Demo","w");
	if(NULL == fp)
	{
		return -1;
	}
	
	returnParse = (Parse*) malloc(sizeof(Parse));
	returnSB = (SB*) malloc(sizeof(SB));
	subscriberBill = (SB**) malloc(SIZE*sizeof(SB*));
	
	myParse = createParseStruct(SIZE);
	subscriberBill = assignSubscriberEntry(myParse, SIZE);
	
	keyVal = assignKeyVal(myParse, SIZE);
	if(NULL == myParse || NULL == keyVal)
	{
		return -1;
	}
	
	
	cdrContainer = CDRContainerCreate(CONTAINER_SIZE, NULL);
	
	
	
	if(NULL == cdrContainer || NULL == returnParse || NULL == subscriberBill)
	{
		return -1;
	}
	
	for(i=0; i<SIZE; ++i)
	{
	
		printf("Values\t m_IMSI: %s KEY: %s CT: %5s   %s %d\n",myParse[i]->m_IMSI, keyVal[i], myParse[i]->m_CallType, myParse[i]->m_CallTime, (int)myParse[i]->m_Duration);
		printf("Values\t m_MSISDN: %s PartyMSISDN: %s CT: %5s   %s %d\n",myParse[i]->m_MSISDN, myParse[i]->m_PartyMSISDN, myParse[i]->m_CallType, myParse[i]->m_CallTime, (int)myParse[i]->m_Duration);
		
		printToFile(fp, myParse[i]);
		
	}
	for(i=0; i<SIZE; ++i)
	{
		printSBBill(subscriberBill[i]);
		
	}
	
	
	for(i=0; i<SIZE; ++i)
	{
/*		CDRContainerInsertData(cdrContainer, (void*) (keyVal[i]), (void*) (subscriberBill[i]));*/
		CDRContainerInsertAndUpdateData(cdrContainer, (void*) (keyVal[i]), (void*) (subscriberBill[i]), sbscrbrSumDataValues);
	}
	CDRContainerFindData(cdrContainer, (void*) (keyVal[0]), (void**) &returnSB);
	if(returnSB != NULL)
	{
		printSBreturn(returnSB);
	}
	CDRContainerFindData(cdrContainer, (void*) (keyVal[2]), (void**) &returnSB);
	if(returnSB != NULL)
	{
		printSBreturn(returnSB);
	}
	
	CDRContainerDeleteData(cdrContainer, (void*) (keyVal[2]), (void**) &pKey, (void**) &pValue);
	if( NULL != pKey || NULL != pValue)
	{
	printf("KEY KEY \t%s\n",pKey);
	printSBValues(pValue);
	}
	
	CDRContainerInsertAndUpdateData(cdrContainer, (void*) (keyVal[2]), (void*) (subscriberBill[2]), sbscrbrSumDataValues);
	printSBValues(pValue);
	
	CDRContainerInsertAndUpdateData(cdrContainer, (void*) (keyVal[2]), (void*) (subscriberBill[2]), sbscrbrSumDataValues);
	printSBValues(pValue);
	CDRContainerInsertAndUpdateData(cdrContainer, (void*) (keyVal[2]), (void*) (subscriberBill[3]), sbscrbrSumDataValues);
	printSBValues(pValue);
	
	
	printf("TEST !!!\n");
	
	fclose(fp);
	return 0;
}

/*
	MOC
	outgoing voice call
	MTC
	incoming voice call
	SMS-MO
	outgoing message
	SMS-MT
	incoming message
	GPRS
	internet
	*/
SB** assignSubscriberEntry(Parse** _parse, size_t _parseSize)
{
		int i;
		SB** subscriberBill;
		subscriberBill = (SB**) calloc(SIZE,sizeof(SB*));
		
		for(i=0; i<_parseSize; ++i)
		{
			subscriberBill[i] = (SB*) calloc(1,sizeof(SB));
			subscriberBill[i]->m_IMSI = _parse[i]->m_IMSI;
			subscriberBill[i]->m_MBdownload = _parse[i]->m_Download;
			subscriberBill[i]->m_MBupload = _parse[i]->m_Upload;
			if(strcmp(_parse[i]->m_CallType, "MOC") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") == 0)
			{
				subscriberBill[i]->m_outgoingCallWin = _parse[i]->m_Duration;
			}
			if(strcmp(_parse[i]->m_CallType, "MTC") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") == 0)
			{
				subscriberBill[i]->m_incomingCallWin = _parse[i]->m_Duration;
			}
			if(strcmp(_parse[i]->m_CallType, "MOC") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") != 0)
			{
				subscriberBill[i]->m_outgoingCallWout = _parse[i]->m_Duration;
			}
			if(strcmp(_parse[i]->m_CallType, "MTC") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") != 0)
			{
				subscriberBill[i]->m_incomingCallWout = _parse[i]->m_Duration;
			}
			
			if(strcmp(_parse[i]->m_CallType, "SMS_MT") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") == 0)
			{
				subscriberBill[i]->m_recSMSWin = 1;
			}
			if(strcmp(_parse[i]->m_CallType, "SMS_MO") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") == 0)
			{
				subscriberBill[i]->m_sentSMSWin = 1;
			}
			if(strcmp(_parse[i]->m_CallType, "SMS_MT") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") != 0)
			{
				subscriberBill[i]->m_recSMSWout = 1;
			}
			if(strcmp(_parse[i]->m_CallType, "SMS_MO") == 0 && strcmp(_parse[i]->m_PartyMSISDN, "Golan Tell") != 0)
			{
				subscriberBill[i]->m_sentSMSWout = 1;
			}	
		}
		return subscriberBill;
}

void printSBValues(char* pValue)
{
	printf("SubscriberBill: %s %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n",((SB*)pValue)->m_IMSI, (int)((SB*)pValue)->m_outgoingCallWin, (int)((SB*)pValue)->m_incomingCallWin, (int)((SB*)pValue)->m_outgoingCallWout, (int)((SB*)pValue)->m_incomingCallWout, (int)((SB*)pValue)->m_sentSMSWin, (int)((SB*)pValue)->m_recSMSWin, (int)((SB*)pValue)->m_sentSMSWout, (int)((SB*)pValue)->m_recSMSWout, (int)((SB*)pValue)->m_MBdownload, (int)((SB*)pValue)->m_MBupload);
}

void printSBBill(SB* _subscriberBill){
	printf("SubscriberBill: %s %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n",_subscriberBill->m_IMSI, (int)_subscriberBill->m_outgoingCallWin, (int)_subscriberBill->m_incomingCallWin, (int)_subscriberBill->m_outgoingCallWout, (int)_subscriberBill->m_incomingCallWout, (int)_subscriberBill->m_sentSMSWin, (int)_subscriberBill->m_recSMSWin, (int)_subscriberBill->m_sentSMSWout, (int)_subscriberBill->m_recSMSWout, (int)_subscriberBill->m_MBdownload, (int)_subscriberBill->m_MBupload);
}

void printSBreturn(SB* _subscriberBill){
	printf("Subscbr Return: %s %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n",_subscriberBill->m_IMSI, (int)_subscriberBill->m_outgoingCallWin, (int)_subscriberBill->m_incomingCallWin, (int)_subscriberBill->m_outgoingCallWout, (int)_subscriberBill->m_incomingCallWout, (int)_subscriberBill->m_sentSMSWin, (int)_subscriberBill->m_recSMSWin, (int)_subscriberBill->m_sentSMSWout, (int)_subscriberBill->m_recSMSWout, (int)_subscriberBill->m_MBdownload, (int)_subscriberBill->m_MBupload);
}

void printToFile(FILE* _fp, Parse* _printParse)
{
	fprintf(_fp, "%s|%s|%s|%s|%s|%s|%s|%s|%d|%d|%d|%s|%s\n" ,_printParse->m_IMSI, _printParse->m_MSISDN, _printParse->m_subscriberIMEI, _printParse->m_OpBrandName, _printParse->m_OpMCCmncTuple, _printParse->m_CallType, _printParse->m_CallDate, _printParse->m_CallTime, (int)_printParse->m_Duration, (int)_printParse->m_Download, (int)_printParse->m_Upload, _printParse->m_PartyMSISDN, _printParse->m_PartyOp);
}

void sbscrbrSumDataValues(void* _pValue, void* _incomingData)
{
	((SB*)_pValue)->m_outgoingCallWin = ((SB*)_pValue)->m_outgoingCallWin + ((SB*)_incomingData)->m_outgoingCallWin;
	((SB*)_pValue)->m_incomingCallWin = ((SB*)_pValue)->m_incomingCallWin + ((SB*)_incomingData)->m_incomingCallWin;
	((SB*)_pValue)->m_outgoingCallWout = ((SB*)_pValue)->m_outgoingCallWout + ((SB*)_incomingData)->m_outgoingCallWout;
	((SB*)_pValue)->m_incomingCallWout = ((SB*)_pValue)->m_incomingCallWout + ((SB*)_incomingData)->m_incomingCallWout;
	
	((SB*)_pValue)->m_sentSMSWin = ((SB*)_pValue)->m_sentSMSWin + ((SB*)_incomingData)->m_sentSMSWin;
	((SB*)_pValue)->m_recSMSWin = ((SB*)_pValue)->m_recSMSWin + ((SB*)_incomingData)->m_recSMSWin;
	((SB*)_pValue)->m_sentSMSWout = ((SB*)_pValue)->m_sentSMSWout + ((SB*)_incomingData)->m_sentSMSWout;
	((SB*)_pValue)->m_recSMSWout = ((SB*)_pValue)->m_recSMSWout + ((SB*)_incomingData)->m_recSMSWout;
	
	((SB*)_pValue)->m_MBdownload = ((SB*)_pValue)->m_MBdownload + ((SB*)_incomingData)->m_MBdownload;
	((SB*)_pValue)->m_MBupload = ((SB*)_pValue)->m_MBupload + ((SB*)_incomingData)->m_MBupload;	
}

void opertorSumDataValues(void* _pValue, void* _incomingData)
{
	((OSett*)_pValue)->m_incomingCalls = ((OSett*)_pValue)->m_incomingCalls + ((OSett*)_incomingData)->m_incomingCalls;
	((OSett*)_pValue)->m_outgoingCalls = ((OSett*)_pValue)->m_outgoingCalls + ((OSett*)_incomingData)->m_outgoingCalls;
	((OSett*)_pValue)->m_incomingSMS = ((OSett*)_pValue)->m_incomingSMS + ((OSett*)_incomingData)->m_incomingSMS;
	((OSett*)_pValue)->m_outgoingSMS = ((OSett*)_pValue)->m_outgoingSMS + ((OSett*)_incomingData)->m_outgoingSMS;
}

