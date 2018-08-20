#include "commCreator.h"
#include "CDRReader.h"

#include <stdio.h>

 
 
 int main()
 {
 	CommCreator* comm;
 	comm = CommCreatorCreate("../channelFile.txt");
 
 	Notify("CDR_File", comm);
 
 	return 0;
 }
