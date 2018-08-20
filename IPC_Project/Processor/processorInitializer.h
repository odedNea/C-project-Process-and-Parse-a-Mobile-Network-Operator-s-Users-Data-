#ifndef _PROCESSOR_INITLIALIZER_H
#define _PROCESSOR_INITLIALIZER_H


/** 
 * @brief First module to be run in the "processor" executable which will manage and initialize
 * all other modules.
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct ProcsrInit ProcsrInit;



/* 
 * function pointer to be passed to Comm when created.
 * _parce will be sent to aggregator through _context.
 */
typedef void(*ParceReceive)(void* _parce, void* _context);

/** 
 * @brief Gets starting info from config file, and creates Comm and CDRDetector.
 * @params _configName : config file name.
 * @return pointer to the ProcsrInit.
 */
void ProcessorStart();














#endif /*_PROCESSOR_INITLIALIZER_H*/
