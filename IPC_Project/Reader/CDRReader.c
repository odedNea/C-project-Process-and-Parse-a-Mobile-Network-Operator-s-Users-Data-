#include "CDRReader.h"
#include "commCreator.h"

#include <stdio.h> /*fscanf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strtok*/
#include <assert.h>
#include <unistd.h> /*sleep*/

#define MAX_IMSI 16
#define MAX_OP 65
#define MAX_DURATION 7


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


static Cdr* FileRead(const char* _path, FILE* fp)
{
	Cdr* cdrToSend;
	char buff[512];
	const char s[2] = "|";
    char *token;
	
	cdrToSend = (Cdr*)calloc(sizeof(Cdr), 1);
	assert(cdrToSend);
	
	if (NULL == fgets(buff, 300, fp))
	{
		return NULL;
	}
   
   token = strtok(buff, s);
   
   while( token != NULL )
   {
      strcpy(cdrToSend->m_IMSI, token );
      token = strtok(NULL, s);
      strcpy(cdrToSend->m_MSISDN, token );
      token = strtok(NULL, s);
  	  strcpy(cdrToSend->m_subscriberIMEI, token );
  	  token = strtok(NULL, s);
	strcpy(cdrToSend->m_opBrandName, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_opMCCmncTuple, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_callType, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_callDate, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_callTime, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_duration, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_download, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_upload, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_partyMSISDN, token );
	token = strtok(NULL, s);
	strcpy(cdrToSend->m_partyOp, token );
	token = strtok(NULL, s);
      
   }
	
	
	buff[0] = '\0';

	
	printf("%s %s %s %s %s %s %s %s %s %s %s %s %s\n", cdrToSend->m_IMSI
	, cdrToSend->m_MSISDN, cdrToSend->m_subscriberIMEI, cdrToSend->m_opBrandName
	, cdrToSend->m_opMCCmncTuple, cdrToSend->m_callType, cdrToSend->m_callDate
	, cdrToSend->m_callTime, cdrToSend->m_duration, cdrToSend->m_download
	, cdrToSend->m_upload, cdrToSend->m_partyMSISDN, cdrToSend->m_partyOp);

	return cdrToSend;
}



/* reads from the path, sends through context (== channel name) */
void Notify(const char* _path, void* _comm)
{
	FILE* fp;
	Cdr* cdrToSend;
	
	if (NULL == (fp = fopen(_path, "r")))
	{
		return;
	}
	
	do
	{
		cdrToSend = FileRead(_path, fp);
		if (cdrToSend)
		{
			SendComm(_comm, cdrToSend);
			free(cdrToSend);
		}	
	
		sleep(3);
	}
	while(cdrToSend);
	
	fclose(fp);
}












					/*--FileRead--*/
/*	strcpy(cdrToSend->m_IMSI, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_MSISDN, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_subscriberIMEI, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_opBrandName, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_opMCCmncTuple, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_callType, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_callDate, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_callTime, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_duration, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_download, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_upload, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_partyMSISDN, strtok(buff, "|"));*/
/*	strcpy(cdrToSend->m_partyOp, strtok(buff, "|"));*/
	
	
	
/*	fscanf(fp, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\n", cdrToSend->m_IMSI*/
/*	, cdrToSend->m_MSISDN, cdrToSend->m_subscriberIMEI, cdrToSend->m_opBrandName*/
/*	, cdrToSend->m_opMCCmncTuple, cdrToSend->m_callType, cdrToSend->m_callDate*/
/*	, cdrToSend->m_callTime, cdrToSend->m_duration, cdrToSend->m_download*/
/*	, cdrToSend->m_upload, cdrToSend->m_partyMSISDN, cdrToSend->m_party*/





