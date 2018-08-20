#ifndef _COMM_CREATOR_H
#define _COMM_CREATOR_H


/** 
 * @brief First module to be run in the "reader" executable which will manage and initialize
 * all other modules.
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct CommCreator CommCreator;


/*typedef void(*FileNotify)(const char* _path, void* _context); */


CommCreator* CommCreatorCreate(char* _name);


void SendComm(CommCreator* _comm, void* _record);


void CommCreatorDestroy(CommCreator* _comm);



#endif /*_COMM_CREATOR_H*/
