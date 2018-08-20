#ifndef _AGGREGATOR_H_
#define _AGGREGATOR_H_

#include <stddef.h>

/** 
 * @brief Controlls both containers - Operator and subscriber
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct Aggregator Aggregator;


typedef struct Cdr Cdr;


/* 
 * function pointer to be passed to Receiver when created.
 * to send _record and _context is pointer to the aggregator struct.
 */
/*typedef void(*CdrInserts)(void* _record, void* _context); */


/** 
 * @brief Gets starting info from config file, and creates Comm and CDRDetector.
 * @params _configName : config file name.
 * @return pointer to the ProcsrManager.
 */
Aggregator* AggCreate(size_t _subCapacity, size_t _opCapacity);



void AggDestroy(Aggregator* _agg);

/* flag: 1 for subscriber 0 for operator*/
int AggFind(const Aggregator* _agg, const void* _searchKey, void** _pValue, int _flag);


void CdrInsert(void* _record, void* _context);



void AggForEach(const Aggregator* _agg, int _func(const void*, void*, void*), void* _context);














#endif /*_AGGREGATOR_H_*/
