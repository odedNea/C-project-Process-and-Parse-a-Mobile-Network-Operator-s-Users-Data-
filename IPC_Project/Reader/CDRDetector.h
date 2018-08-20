#ifndef _CDR_DETECTOR_H
#define _CDR_DETECTOR_H


/** 
 * @brief Detects a CDR file in a cpecified directory once in a specified time, notifies and keeps detecting.
 * @author Author Oded Neaman (oded.neaman@gmail.com)
 * @ known bugs - unknown.
 */ 

typedef struct CDRDetector CDRDetector;



CDRDetector* CDRDetectorCreate(useconds_t _delay);


void CDRDetectorDestroy();


/*int CDRDetect(CDRDetector* _cdrDetector);*/





#endif /*_CDR_DETECTOR_H*/
