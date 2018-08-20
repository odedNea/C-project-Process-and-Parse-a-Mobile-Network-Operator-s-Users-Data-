#include "processorInitializer.h"
#include "receiver.h"
#include "aggregator.h"
#include "queryProcessor.h"

#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>






void ProcessorStart()
{
	Receiver* rec;
	Aggregator* agg;
	pthread_t pThrdQuery;
	pthread_t pThrdRec;
	
	agg = AggCreate(30, 10);
	rec = ReceiverCreate(CdrInsert, agg, "../channelFile.txt");
	
    pthread_create(&pThrdQuery, NULL, StartQueryP, agg);
    pthread_create(&pThrdRec, NULL, StartReceiving, rec);
    
    pthread_join(pThrdQuery, NULL);
    pthread_join(pThrdRec, NULL);

	
	printf("end\n");
	
}




