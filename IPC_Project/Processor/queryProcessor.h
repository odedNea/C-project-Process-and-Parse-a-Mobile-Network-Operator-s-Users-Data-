#ifndef _QUERY_PROCESSOR_H_
#define _QUERY_PROCESSOR_H_

#include "aggregator.h"
#include <stddef.h>

/** 
 * @brief Controlls both containers - Operator and subscriber
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct Query Query;


void* StartQueryP(void* _agg);



#endif /*_QUERY_PROCESSOR_H_*/
