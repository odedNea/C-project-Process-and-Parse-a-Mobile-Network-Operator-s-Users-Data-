#ifndef _COMM_H_
#define _COMM_H_


/** 
 * @brief Communication between 2 executable modules. 
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 
 
 
 
typedef struct Comm Comm;



/** 
 * @brief Creates a communication channel between 2 executable modules.
 * @params _fileName : a file name read from config file as a key for the channel.
 * @return pointer to Comm or NULL for error.
 */
Comm* CommCreate(char* _fileName);

/** 
 * @brief Destroyes the communication channel.
 * @params _comm : Pointer to Comm.
 * @return void.
 */
void CommDestroy(Comm* _comm);


void CommPauseAndResume(Comm* _comm);





 #endif /*_COMM_H_*/
