#include "../Shared/channel.h"

#include <stdio.h>
#include <stdlib.h> /*malloc*/
#include <assert.h>

#define CGF_SIZE 80

typedef struct CGFpair
{
	char m_funcNum;
	char m_query[64];
}CGFpair;




static void ImsiRetreive(int _msqid, CGFpair* _pair)
{
	
	
	_pair->m_funcNum = '1';
	printf("Please enter Imsi for query:\n");
	scanf("%s", _pair->m_query);
	
	RecordSend(_msqid, _pair, CGF_SIZE);
}




static void AllSubReport(int _msqid, CGFpair* _pair)
{
	
	
	_pair->m_funcNum = '3';
	
	RecordSend(_msqid, _pair, CGF_SIZE);
}




void ShowMenu()
{
	printf("\n\n\n***CDR MENU***\n\n\n");
		printf("Please enter your choice:\n\n");
		printf("1. IMSI Retreive Query\n"
			   "2. Operator Retreive Query\n"
			   "3. All Subscribers Report\n"
			   "4. All Operators Report\n"
			   "5. Aggregated Information\n");
	

}



void MenuInit(int _msqid)
{
	int choice;
	CGFpair* pair;
	
	while (1)
	{
		pair = (CGFpair*)malloc(sizeof(CGFpair));
		assert(pair);

		ShowMenu();

		scanf("%d", &choice);
	
		switch(choice)
		{
		    case 1:
		        ImsiRetreive(_msqid, pair);
		        break;
		    case 2:
		        RecordSend(_msqid, "2", CGF_SIZE);
		        break;
		    case 3:
		        AllSubReport(_msqid, pair);
		        break;
		    case 4:
		        RecordSend(_msqid, "4", CGF_SIZE);
		        break;
		    case 5:
		        RecordSend(_msqid, "5", CGF_SIZE);
		        break;
		    default:
		    	printf("invalid choice\n");
		    	break;
		    	
		}
		/*RecAndPrint()*/
		free(pair);
	}
}



int main()
{
	int msqID;

	msqID = ChannelCreate("../UIchannel.txt");
	MenuInit(msqID);
	
	return 0;
}



