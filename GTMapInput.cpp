#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include <vector>
#include <iostream>
#include <iomanip>

// File operation
#include <fstream> 

#include "DataFormat.h"
#include "GTMapObject.h"


using namespace std;

// MACROS
#define TAG_GTM_IN "GTMapIn: " // GTMap module OUTPUT

// GLOBAL VARIABLES

static ofstream myFile;


//// OUTPUT TO FILE /////
int file_open() {

  // File: out, binary, not appended, thus will reset size to zero
  myFile.open("..\\localize_out_right.mjpeg",
    ios::out | ios::binary); // | ios::app to append
  if (!myFile.is_open()) {
    std::cout << "Input operation not successful\n";
    return -1;
  }
  return 0;
}

int file_write(std::vector<unsigned char> &buf) {
  char* data = reinterpret_cast<char*>(buf.data());

  // Program assume the file is already opened.
  myFile.write(data, buf.size());
  return 0;
}

int GTMapInput_GetRequest(GTMapObject *pGTMapObj)
{
  int iRetVal;
 
  // Network related variables
  //sockaddr sockClientAddr;
  //int iLenSockClient = sizeof(sockaddr_in);
  SOCKET *phSock;
  sockaddr *phCliAddr;

  int *piLenAddr;
  int iPktLen;
  char *pPktBuf;

  // REQUEST holder
  char ucReqMsg[11] = REQ_STREAM; // +1 to add null at last

  // Packet details
  GTMapPacket    *pGTMapPkt;
  unsigned char  *pFrameL;
  unsigned char  *pFrameR;

  // Assign the object pointers
  pGTMapPkt  = pGTMapObj->pGTMapPacket;
  phSock     = &(pGTMapObj->hSockObj.hSock);
  //phSockObj  = &(pLocObj->hSockStereo);

  // Recv client details
  phCliAddr = &(pGTMapObj->hSockObj.hClientAddr);
  piLenAddr = &(pGTMapObj->hSockObj.iLenClientAddr);

  // streaming buffer address and its length
  pPktBuf = (char *)pGTMapPkt;
  iPktLen = sizeof(GTMapPacket);

  *piLenAddr = sizeof(sockaddr_in);

  // RECEIVE STEREO PACKET DATA
  printf(TAG_GTM_IN "Wait to receive request. iPktLen:%d, addr:%d, len:%d \n", iPktLen, phCliAddr, *piLenAddr);
  //iRetVal = SocketUDP_ClientRecv(phSockObj, pPktBuf, iPktLen);

  iRetVal = SocketUDP_RecvFrom(phSock, pPktBuf, iPktLen, phCliAddr, piLenAddr);

  if (iRetVal < 0 ) { goto ret_err; } // If no client, it will be -1, it is okay to retry as a server

  printf(TAG_GTM_IN "Received request of len:%d\n", iRetVal);

  if (iRetVal > 0) {

	  // Process the packet
	  
	  // SEND THE RESPONSE
	 // iRetVal = SocketUDP_SendTo(phSock, pPktBuf, iPktLen,
	//	  phClientAddr, *piLenCliAddr);

	//  if (iRetVal < 0) { goto ret_err; }
  }
  
  return 0;
ret_err:
  printf(TAG_GTM_IN "Error: ret:%d, NetErr:%d\n", iRetVal, WSAGetLastError());
  return -1;

}

int GTMapInput_Deinit(GTMapObject *pGTMapObj)
{
  // Socket interfaces
  SOCKET      *phSock;

  phSock     = &pGTMapObj->hSockObj.hSock;
  SocketUDP_Deinit(phSock);

  return 0;
}

//
// GTMapInput_Init
//
// Allocated the packet memory
// Create a Server Socket
int GTMapInput_Init(GTMapObject *pGTMapObj)
{

  int iRetVal = 0;

  // INPUT GTMapPacket.
  GTMapPacket   *pGTMapPkt;
 
  // Socket interfaces
  SOCKET      *phSock;
  SOCKADDR_IN *phServAddr;
  int         iPortNum;  
  char        cIPAddr[16];

  printf("In GTMapInput_Init\n");

  // TODO: Ring buffer
  
  // IN & OUT Buffer
  // Allocate memory stereo packet, i.e. metadata + jpeg frames bytes
  pGTMapPkt     = (GTMapPacket *) malloc(sizeof(GTMapPacket));
  
  // Check if memory allocated
  if (pGTMapPkt == NULL ) { 
	printf("Error: malloc\n"); return -1; }

  // fill the object
  pGTMapObj->pGTMapPacket   = pGTMapPkt;

  phSock     = &(pGTMapObj->hSockObj.hSock);
  phServAddr = &(pGTMapObj->hSockObj.hServAddr),
  iPortNum   = SOCK_PORT_GTMAP;
  memcpy(cIPAddr, SOCK_IP_GTMAP, strlen(SOCK_IP_GTMAP));

  iRetVal = SocketUDP_InitServer(phSock, phServAddr, iPortNum, cIPAddr);
  if (iRetVal != 0) {
    printf("Error: In SocketUDP_InitServer\n");
    return -1;
  }
  SocketUDP_PrintIpPort(phSock, "Init");

  printf("SocketUDP_InitServer... OK\n");
  return 0;
}

