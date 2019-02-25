#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "DataFormat.h"

#pragma comment (lib, "Ws2_32.lib")


// MACROS
//
typedef struct SockInterfaceStruct {
	SOCKET      hSock;
	SOCKADDR_IN hServAddr;

    // Recv will fill this
    sockaddr hClientAddr;
    int      iLenClientAddr;

	int    iPortNum;
	char   cIPAddr[16];
} SockObject;

//
// GTMap needs:
// REQ_TRAFFIC_SIGNS
// 1. INPUT:   Metadata of Lat-long from Localize module
// 2. PROCESS: GTMap will find the Object near to Lat-long 
// 3. OUTPUT:  Sent list of Objects info, its lat-long, image descriptor, etc.
//
// REQ_CAL_COORDINATES
// 1. INPUT:   Send calculated distance of the object, along with its object data
// 2. PROCESS: Calculate the co-ordinates 
// 3. OUTPUT:  Send the co-ordinates (TBD: directly to Localize or Genesys)
//
typedef struct GTMapObjectStruct {
	// INPUT & OUTPUT

	// GTMap packet
	GTMapPacket *pGTMapPacket;

	// PROCESSING Variables
    HANDLE hGTMapScheduler;    // Thread, scheduler

	// INTERFACING Variables
	SockObject hSockObj;

} GTMapObject;

// GTMap main execution function declaration

int GTMapExecute_Start(GTMapObject *pGTMapObject);
int GTMapExecute_Terminate(GTMapObject *pGTMapObject);

// GTMap input and output processing function declarations
int GTMapInput_Init(GTMapObject *pGTMapObj);
int GTMapInput_GetRequest(GTMapObject *pGTMapObj);

// GTMap output processing function declarations
// Network related API declarations
int SocketUDP_RecvFrom(SOCKET *phSock, char *pDataBuf, int iDataSize,
    sockaddr *pSockCliAddr, int *pSockSize);

int SocketUDP_SendTo(SOCKET *phSock, char *pDataBuf, int iDataSize, 
    sockaddr *pSockClientAddr, int iSockSize);

int SocketUDP_InitServer(SOCKET *phSock, SOCKADDR_IN *phServAddr,
    int  iPortNum,   char *pServerIP);

int SocketUDP_Deinit(SOCKET*);
int SocketUDP_ServerInit();

