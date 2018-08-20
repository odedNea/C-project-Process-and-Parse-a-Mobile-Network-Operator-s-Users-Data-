/* Parse Generator */
#include "processorInitializer.h"
#include "aggregator.h"
#include "queryProcessor.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h>  

#define MAX_IMSI 16
#define MAX_OP 65
#define MAX_DURATION 7
#define ENGLISH_LETTERS 26
#define NUM_DIGITS 10

struct Cdr
{
	char m_IMSI[MAX_IMSI];
	char m_MSISDN[MAX_IMSI]; 
	char m_subscriberIMEI[MAX_IMSI]; 
	char m_opBrandName[MAX_OP]; 
	char m_opMCCmncTuple[MAX_DURATION]; 
	char m_callType[7]; 
	char m_callDate[11];
	char m_callTime[9];
	char m_duration[MAX_DURATION]; 
	char m_download[32];
	char m_upload[32];
	char m_partyMSISDN[MAX_IMSI];
	char m_partyOp[MAX_OP];

};



Cdr cdr1 = {"12546554","4127156","124573","CellcomeIL","42502","SMS_MT","20/03/1989","20:10:00","","","","123487","Cellcome"};
Cdr cdr2 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr3 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr4 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr5 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr6 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr7 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr8 = {"12546573","4127156","124573","CellcomeIL","42502","MOC","20/03/1989","20:10:00","67","","","123487","CellcomeIL"};
Cdr cdr9 = {"12546554","4127156","124573","CellcomeIL","42502","SMS_MT","20/03/1989","20:10:00","","","","123487","Cellcome"};
Cdr cdr10 = {"12546554","4127156","124573","CellcomeIL","42502","SMS_MT","20/03/1989","20:10:00","","","","123487","Cellcome"};
Cdr cdr11 = {"12546554","4127156","124573","CellcomeIL","42502","SMS_MT","20/03/1989","20:10:00","","","","123487","Cellcome"};



int main()
{
	ProcessorStart();
	
	
	return 0;
}

















/*Cdr* CdrGenerate(size_t _numOfCDRs) */
/*{*/
/*	size_t i;*/
/*	int j;*/
/*	char tmpStr[MAX_IMSI + 1];*/
/*	char randDurationChar, randImsiChar;*/
/*	int randDurationLen, randImsiLen;*/
/*	Cdr* cdrsArray = malloc(_numOfCDRs * sizeof(Cdr));*/
/*	srand ((unsigned)time(NULL));*/
/*	for (i = 0; i < _numOfCDRs; ++i) */
/*	{*/
/*   		*/
/*		randDurationLen = (1 + rand()% (MAX_DURATION));*/
/*		randImsiLen = (1 + rand()% (MAX_IMSI));*/
/*		for (j = 0; j < randDurationLen; ++j)*/
/*		{*/
/*			randDurationChar = (char)((rand() % NUM_DIGITS) + '0');*/
/*			tmpStr[j] = randDurationChar;*/
/*		}*/
/*		tmpStr[j] = '\0';*/
/*		strcpy(cdrsArray[i].m_duration, tmpStr);*/
/*		tmpStr[0] = '\0';*/
/*		*/
/*		for (j = 0; j < randImsiLen; ++j)*/
/*		{*/
/*			randImsiChar = (char)((rand() % NUM_DIGITS) + '0');*/
/*			tmpStr[j] = randImsiChar;*/
/*		}*/
/*		tmpStr[j] = '\0';*/
/*		strcpy(cdrsArray[i].m_IMSI, tmpStr);*/
/*		tmpStr[0] = '\0';*/
/*	}*/

/*	return cdrsArray;*/
/*}*/






/*int main()*/
/*{*/
/*	char a = '1';*/
/*	Aggregator* agg = AggCreate(30, 30);*/
/*	CdrInsert(&cdr1, agg);*/
/*	CdrInsert(&cdr2, agg);*/
/*	CdrInsert(&cdr3, agg);	*/
/*	CdrInsert(&cdr4, agg);*/
/*	CdrInsert(&cdr5, agg);*/
/*	CdrInsert(&cdr9, agg);*/
/*	CdrInsert(&cdr10, agg);*/
/*	CdrInsert(&cdr11, agg);*/

/*	CGF(agg, "12546573", a);*/
/*	CGF(agg, "12546554", '3');*/








/*	*/
/*	return 0;*/
/*}*/


	
/*	int i;*/
/*	void* pValue;*/
/*	Cdr* cdrsArray = CdrGenerate(30);*/
/*	for (i = 0; i < 30; ++i)*/
/*	{*/
/*		CdrInsert(&cdrsArray[i], agg);*/
/*	*/
/*		printf("duration: %7s IMSI: %15s\n", cdrsArray[i].m_Duration, cdrsArray[i].m_IMSI);*/
/*		*/
/*	}*/
	
/*	SubFind(sub, &cdrsArray[10].m_IMSI, &pValue);*/

/*	printf("imsi: %s duration: %s\n", ((Parse*)pValue)->m_IMSI, ((Parse*)pValue)->m_Duration);*/


