#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include <vector>
#include <iostream>
#include <iomanip>

#include "DataFormat.h"
#include "GTMapObject.h"


using namespace std;

// MACROS
#define TAG_GTM_OUT "GTMapOut: " // GTMap module OUTPUT

int GTMapOutput_SendCalc(GTMapObject *pGTMapObj)
{
	int iRetVal;

	SOCKET   *phSock;
	sockaddr *phCliAddr;
	int      *piLenAddr;

	int iPktLen;
	char *pPktBuf;

	// Packet details
	GTMapPacket    *pGTMapPkt;

	// Assign the object pointers
	pGTMapPkt = pGTMapObj->pGTMapPacket;
	phSock = &(pGTMapObj->hSockObj.hSock);

	// Recv client details
	phCliAddr = &(pGTMapObj->hSockObj.hClientAddr);
	piLenAddr = &(pGTMapObj->hSockObj.iLenClientAddr);

	// streaming buffer address and its length
	pPktBuf = (char *)pGTMapPkt;
	iPktLen = sizeof(GTMapPacket);

	*piLenAddr = sizeof(sockaddr_in);

	// RECEIVE STEREO PACKET DATA
	printf(TAG_GTM_OUT "Sending. iPktLen:%d, addr:%d, len:%d \n", iPktLen, phCliAddr, *piLenAddr);

	iRetVal = SocketUDP_SendTo(phSock, pPktBuf, iPktLen, phCliAddr, *piLenAddr);

	if (iRetVal < 0) { goto ret_err; } // If no client, it will be -1, it is okay to retry as a server

	printf(TAG_GTM_OUT "sent request of len:%d\n", iRetVal);

	return 0;
ret_err:
	printf(TAG_GTM_OUT "Error: ret:%d, NetErr:%d\n", iRetVal, WSAGetLastError());
	return -1;

}

int GTMapOutput_SendMarks(GTMapObject *pGTMapObj)
{
  int iRetVal;
 
  SOCKET   *phSock;
  sockaddr *phCliAddr;
  int      *piLenAddr;

  int iPktLen;
  char *pPktBuf;

  // Packet details
  GTMapPacket    *pGTMapPkt;

  // Assign the object pointers
  pGTMapPkt  = pGTMapObj->pGTMapPacket;
  phSock     = &(pGTMapObj->hSockObj.hSock);

  // Recv client details
  phCliAddr = &(pGTMapObj->hSockObj.hClientAddr);
  piLenAddr = &(pGTMapObj->hSockObj.iLenClientAddr);

  // streaming buffer address and its length
  pPktBuf = (char *)pGTMapPkt;
  iPktLen = sizeof(GTMapPacket);

  *piLenAddr = sizeof(sockaddr_in);

  // RECEIVE STEREO PACKET DATA
  printf(TAG_GTM_OUT "Sending. iPktLen:%d, addr:%d, len:%d \n", iPktLen, phCliAddr, *piLenAddr);

  iRetVal = SocketUDP_SendTo(phSock, pPktBuf, iPktLen, phCliAddr, *piLenAddr);

  if (iRetVal < 0 ) { goto ret_err; } // If no client, it will be -1, it is okay to retry as a server

  printf(TAG_GTM_OUT "sent request of len:%d\n", iRetVal);

  return 0;
ret_err:
  printf(TAG_GTM_OUT "Error: ret:%d, NetErr:%d\n", iRetVal, WSAGetLastError());
  return -1;

}



