#ifndef _READER_INITIALIZER_H
#define _READER_INITIALIZER_H


/** 
 * @brief First module to be run in the "reader" executable which will manage and initialize
 * all other modules.
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct ReaderInit ReaderInit;


/* 
 * function pointer to be passed to CDRDetector when created.
 * the function will create a new process CDRReader to read the file specified in the _path,
 * the _context will be the pointer to the Comm module created.
 */
typedef void(*FileNotify)(const char* _path, void* _context); 

/* 
 * function pointer to be passed to CDRReader when created.
 * _parce will be sent through _context.
 */
typedef void(*ParceSend)(void* _parce, void* _context); 


/** 
 * @brief Gets starting info from config file, and creates Comm and CDRDetector.
 * @params _configName : config file name.
 * @return pointer to the ReaderCreate.
 */
*ReaderInit ReaderInitCreate(char* _configName);


void ReaderPauseAndResume(*ReaderInit _rInit);
/* will receive flag via shared memory */


void ReaderInitDestroy(*ReaderInit _rInit);



/*void CDRDetectorInit(char* _directoryPath);*/




/*void CommInit();*/






#endif /*_READER_INITIALIZER_H*/
