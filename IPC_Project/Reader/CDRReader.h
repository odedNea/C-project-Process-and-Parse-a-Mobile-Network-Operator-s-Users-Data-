#ifndef _CDR_READER_H
#define _CDR_READER_H


/** 
 * @brief Reads each line of a CDR file parces it and sends to the reader manager. 
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 
 
 
 
typedef struct CDRReader CDRReader;

typedef struct Cdr Cdr;
/* 
 * function pointer to be passed to CDRDetector when created.
 * the function will create a new process CDRReader to read the file specified in the _path,
 * the _context will be the pointer to the Comm module created.
 */
typedef void(*FileNotify)(const char* _path, void* _context); 



void Notify(const char* _path, void* _context);

 /** 
 * @brief Creates a CDRReader.
 * @params _cdrName : a CDR file name to read and parse.
 * @return pointer to CDRReader or NULL for error.
 */
CDRReader* CDRReaderCreate(char* _cdrName);


void CDRReaderPauseAndResume(CDRReader* _cdrReader);
 
 
 
void CDRReaderDestroy(CDRReader* _cdrReader);
 
 
 
 
 #endif /*_CDR_READER_H*/
