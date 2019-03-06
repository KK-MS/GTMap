#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

// Threads: to run streaming server
#include <process.h>

#include "DataFormat.h"
#include "GTMapObject.h"

// Macros
#define TAG_GTM "GTM: "


//
// GTMap_scheduler
//
// This is the GTMap server loop
// 
// Server waits for the request,
// Based on the received request the processing is done. 
// The result is sent back the requester.
//
void GTMapExecute_Scheduler(void *param)
{
	int iRetVal = 0;

	GTMapObject *pGTMapObject = (GTMapObject *)param;

	printf(TAG_GTM "In GTMap_scheduler\n");

	while (1) {

		// INPUT: Metadata + JPEG Frames (Right & Left)
		iRetVal = GTMapInput_GetRequest(pGTMapObject);
		if (iRetVal) { goto err_ret; }

        printf("Request:%d\n",pGTMapObject->pGTMapPacket->iRequestType);

        // Process request iRequestType
        switch(pGTMapObject->pGTMapPacket->iRequestType) {
		case REQ_GTMAP_MARKS:
            printf("In REQ_GTMAP_MARKS\n");
			iRetVal = GTMapProcess_GetMarks(pGTMapObject);
		    iRetVal = GTMapOutput_SendMarks(pGTMapObject);
			break;
		case REQ_GTMAP_CALC:
			iRetVal = GTMapOutput_SendCalc(pGTMapObject);
			break;
        }

		//iRetVal = GTMapProcess_JpegToRaw(pGTMapObject);
		//if (iRetVal) { goto err_ret; }

		Sleep(10);
	}

err_ret:
	printf("%s: thread closing: %d\n", __func__, iRetVal);
}


int GTMapExecute_Terminate(GTMapObject *pGTMapObject)
{

	// WAIT for Server loop to end
	printf(TAG_GTM "In GTMap: WaitForSingleObject h_GTMap_scheduler\n");
	WaitForSingleObject(pGTMapObject->hGTMapScheduler, INFINITE);

	return 0;
}


// 
// StereoExecute_Start
// 
// Starts the execution processing by starting the scheduler.
//
int GTMapExecute_Start(GTMapObject *pGTMapObject)
{
  pGTMapObject->hGTMapScheduler = 
      (HANDLE)_beginthread(GTMapExecute_Scheduler, 0, (void *)pGTMapObject);

	return 0;
}


int main()
{

	GTMapObject *pGTMapObject;

	pGTMapObject = (GTMapObject *)malloc(sizeof(GTMapObject));
	if (!(pGTMapObject)) { printf("Error: malloc\n"); return -1; }

	// Initialize the input interfaces
	GTMapInput_Init(pGTMapObject);

	// Initialize the output interface.
	// Output is a stream of IMU data + GTMap camera frames
	//GTMapOutput_Init(pGTMapObject);

	GTMapExecute_Start(pGTMapObject);

	getchar();

    GTMapExecute_Terminate(pGTMapObject);

	//GTMap_input_deinit();

    // Thread ended, release resources
    free(pGTMapObject);

	return 0;
}
