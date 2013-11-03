/*
          Copyright (C) 1993, 1994, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993, 1994 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993, 1994 DICOM Central Test Node project for, and
          under contract with, the Radiological Society of North America.

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          jointly owned by RSNA and Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/
/* Copyright marker.  Copyright will be inserted above.  Do not remove */
/*
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
**			DUL_InitializeNetwork
**			DUL_DropNetwork
**			DUL_RequestAssociation
**			DUL_ReceiveAssociationRQ
**			DUL_AcknowledgeAssociateRQ
**			DUL_RejectAssociateRQ
**			DUL_ReleaseAssociation
**			DUL_AbortAssociation
**			DUL_DropAssociation
**			DUL_WritePDVs
**			DUL_ReadPDVs
** Author, Date:	Stephen M. Moore, 14-Apr-93
** Intent:		This module contains the public entry points for the
**			DICOM Upper Layer (DUL) protocol package.
** Last Update:		$Author: smm $, $Date: 2002-07-26 15:17:53 $
** Source File:		$RCSfile: dulprotocol.c,v $
** Revision:		$Revision: 1.58 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.58 $ $RCSfile: dulprotocol.c,v $";

#include "ctn_os.h"

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_uids.h"
#include "ctnthread.h"
#include "dulprotocol.h"
#include "dulstructures.h"
#include "dulprivate.h"
#include "dulfsm.h"
#ifdef BLOG
#include "blg.h"
#endif

#if 0
static int networkInitialized = 0;
#endif
static CTNBOOLEAN debug = 0;
#ifdef BLOG
static CTNBOOLEAN blog = 0;
#endif

static CONDITION
createNetworkKey(char *type, char *mode, int timeout, unsigned long opt,
		 PRIVATE_NETWORKKEY ** k);
static CONDITION
createAssociationKey(PRIVATE_NETWORKKEY ** net, char *node,
		     unsigned long maxPDU,
		     PRIVATE_ASSOCIATIONKEY ** assoc);
static CONDITION initializeNetworkTCP(PRIVATE_NETWORKKEY ** k, void *p);
static CONDITION
receiveTransportConnection(PRIVATE_NETWORKKEY ** network,
			   DUL_BLOCKOPTIONS block,
			   DUL_ASSOCIATESERVICEPARAMETERS * params,
			   CTN_SOCKET* s);
static CONDITION
receiveTransportConnectionTCP(PRIVATE_NETWORKKEY ** network,
			      DUL_BLOCKOPTIONS block,
			      DUL_ASSOCIATESERVICEPARAMETERS * params,
			      CTN_SOCKET* s);


static void destroyAssociationKey(PRIVATE_ASSOCIATIONKEY ** key);
static CONDITION writeDataPDU(PRIVATE_ASSOCIATIONKEY ** key, DUL_DATAPDU * pdu);
static CONDITION dropPDU(PRIVATE_ASSOCIATIONKEY ** association, unsigned long l);
static CONDITION
get_association_parameter(void *paramAddress,
			  DUL_DATA_TYPE paramType, size_t paramLength,
	DUL_DATA_TYPE outputType, void *outputAddress, size_t outputLength);
static void setTCPBufferLength(int sock);
static CONDITION checkNetwork(PRIVATE_NETWORKKEY ** networkKey, char *caller);
static CONDITION
checkAssociation(PRIVATE_ASSOCIATIONKEY ** association,
		 char *caller);
static void dump_presentation_ctx(LST_HEAD ** l);
static void dump_uid(char *UID, char *indent);
static void clearRequestorsParams(DUL_ASSOCIATESERVICEPARAMETERS * params);
static void
    clearPresentationContext(LST_HEAD ** l);

#define	MIN_PDU_LENGTH	4*1024
#define	MAX_PDU_LENGTH	128*1024

/* requestAssociationTCP
**
** Purpose:
**	Request a TCP Association
**
** Parameter Dictionary:
**
**	network		Handle to the network environment
**	params		Service parameters describing the Association
**	association	Handle to the Association
**
** Return Values:
**
**	DUL_ILLEGALSERVICEPARAMETER
**	DUL_NORMAL
**	DUL_TCPINITERROR
**	DUL_UNKNOWNHOST
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/


static CONDITION
requestAssociationTCP(PRIVATE_NETWORKKEY ** network,
		      DUL_ASSOCIATESERVICEPARAMETERS * params,
		      CTN_SOCKET* s)
{
  char node[128];
  int port;
  struct sockaddr_in server;
  struct hostent *hp;
#ifndef _MSC_VER
  struct hostent *gethostbyname();
#endif
  CTN_SOCKET sck;

    struct linger sockarg;
#ifdef CTN_USE_THREADS
    struct hostent hStorage;
    char hBuffer[512];
    int hError = 0;
#endif

  if (sscanf(params->calledPresentationAddress, "%[^:]:%d", node, &port) != 2)
    return COND_PushCondition(DUL_ILLEGALSERVICEPARAMETER,
				  DUL_Message(DUL_ILLEGALSERVICEPARAMETER),
				  params->calledPresentationAddress);

  sck = socket(AF_INET, SOCK_STREAM, 0);
  *s = sck;
  if (*s < 0) {
    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
    }
  server.sin_family = AF_INET;

#ifdef CTN_USE_THREADS
#ifdef SOLARIS
  hp = gethostbyname_r(node, &hStorage, hBuffer, sizeof(hBuffer), &hError);
#else
  hp = gethostbyname(node);
#endif

#else
  hp = gethostbyname(node);
#endif
  if (hp == NULL) {
    return COND_PushCondition(DUL_UNKNOWNHOST,
				  DUL_Message(DUL_UNKNOWNHOST), node);
    }
  (void) memcpy(&server.sin_addr, hp->h_addr, (unsigned long) hp->h_length);
  server.sin_port = (u_short) htons(port);

  /*fprintf(stderr, "Socket is %d:\n", *s);*/
  if (connect(*s, (struct sockaddr *) & server, sizeof(server)) < 0) {
#ifdef _WIN32
    (void) closesocket(*s);
#else
    (void) close(*s);
#endif
    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
  }

  sockarg.l_onoff = 0;
  if (setsockopt(*s, SOL_SOCKET, SO_LINGER, (char *) &sockarg, (int) sizeof(sockarg)) < 0) {
    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
  }
  setTCPBufferLength(*s);

  return DUL_NORMAL;
}


/* writeTCP
**
** Purpose:
**  This is a proxy function for writing bytes to the
**  network of a straight TCP socket. This proxy function
**  is used by the DUL toolkit in the normal mode (no encryption).
**  If the user wants to use a different proxy, that is specified
**  when the network connection is initialized.
**
** Parameter Dictionary:
**	proxyContext		The context we need for writing data
**				to the network socket. Is the address
**				of a file descriptor.
**	buf			Pointer to the buffer to write.
**	nbyte			Number of bytes to write
**
** Return Values:
**
**	-1	failure
**	> 0	number of bytes written
*/

static int
writeTCP(void* proxyContext, const void* buf, int nbyte)
{
  int bytesWritten;
  int totalBytes = 0;
  CTN_SOCKET *fdp;
  CTN_SOCKET fd;
  unsigned char* p = 0;

  fdp = (CTN_SOCKET*)proxyContext;
  fd = *fdp;
  p = (unsigned char*) buf;
  /*fprintf(stderr, "Socket is %d\n", fd); fflush(stderr);*/

  while (nbyte > 0) {
#ifdef _WIN32
    bytesWritten = send(fd, p, nbyte, 0);
#else
    bytesWritten = write(fd, p, nbyte);
#endif
    if (bytesWritten == -1 && errno == EINTR)
      continue;

    if (bytesWritten < 0) {
      perror("Writing to network");
      return -1;
    }

    p += bytesWritten;
    nbyte -= bytesWritten;
    totalBytes += bytesWritten;
  }
  return totalBytes;
}

/* readTCP
**
** Purpose:
**  This is a proxy function for reading bytes from the
**  network using a straight TCP socket. This proxy function
**  is used by the DUL toolkit in the normal mode (no encryption).
**  If the user wants to use a different proxy, that is specified
**  when the network connection is initialized.
**
** Parameter Dictionary:
**	proxyContext		The context we need for writing data
**				to the network socket. Is the address
**				of a file descriptor.
**	buf			Pointer to the buffer to write.
**	nbyte			Number of bytes to read
**
** Return Values:
**
**	-1	failure
**	> 0	number of bytes read
*/

static int 
readTCP(void* proxyContext, void* buf, int nbyte)
{
  int bytesRead;
  int totalBytes = 0;
  CTN_SOCKET *fd;
  int done = 0;

  fd = (CTN_SOCKET*)proxyContext;
  /*fprintf(stderr, "Socket is %d\n", *fd); fflush(stderr);*/

  while (!done) {
#ifdef _WIN32
    bytesRead = recv(*fd, buf, nbyte, 0);
#else
    bytesRead = read(*fd, buf, nbyte);
#endif
    if (bytesRead == -1 && errno == EINTR)
      continue;

    if (bytesRead < 0)
      return -1;

    done = 1;
  }

  return bytesRead;
}

static int
networkDataAvailable(void* proxyContext, int timeout)
{
  struct timeval t;
  fd_set fdset;
  int nfound;

  CTN_SOCKET s;
  s = *((CTN_SOCKET*)proxyContext);

  FD_ZERO(&fdset);
  FD_SET(s, &fdset);
  t.tv_sec = timeout;
  t.tv_usec = 0;
  nfound = select(s + 1, &fdset, NULL, NULL, &t);
  if (nfound <= 0)
    return 0;
  else {
    if (FD_ISSET(s, &fdset))
      return 1;
    else			/* This one should not really happen */
      return 0;
  }
}

static int
closeTCP(void* proxyContext)
{
  CTN_SOCKET* p;

  p = (CTN_SOCKET*)proxyContext;

#ifdef _WIN32
  (void) closesocket(*p);
#else
  (void) close(*p);
#endif
  return 0;
}

/* DUL_InitializeNetwork
**
** Purpose:
**  Identify and initialize a network to request or accept Associations.
**  The caller identifies a network type (DUL_NETWORK_TCP, DUL_NETWORK_OSI)
**  and whether the application wishes to be an Acceptor (AE_ACCEPTOR) or a
**  Requestor (AE_REQUESTOR) or both (AE_BOTH).  Upon successful
**  initialization of the network, the function returns a key to be used
**  for further network access.
**
** Parameter Dictionary:
**	networkType		One of a set of predefined constants which
**				describe the type of network environment
**				requested.
**	mode			NULL terminated string identifying the appli-
**				cation as a requestor or acceptor.
**	networkParameter	A parameter which is specific to the network
**				type, which may be needed to initialize the n/w
**	timeout			Length of time in seconds for TIMER timeout.
**				If 0, the function will use a default value.
**	opt			Bitmask which describes options to be used
**				when initializing the network.
**	networkKey		The handle created by this function and returned
**				to the caller to access this network environment
**
** Return Values:
**
**	DUL_KEYCREATEFAILURE
**	DUL_NETWORKINITIALIZED
**	DUL_NORMAL
**	DUL_UNRECOGNIZEDAE
**	DUL_UNSUPPORTEDNETWORK
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_InitializeNetwork(char *networkType, char *mode,
		      void *networkParameter, int timeout, unsigned long opt,
		      DUL_NETWORKKEY ** networkKey)
{
    PRIVATE_NETWORKKEY
	* key;			/* Our local copy of the key which is created */
    CONDITION
	cond;
#ifdef _MSC_VER
    WSADATA wsaData;
#endif

#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_InitializeNetwork, Type: %s, Mode: %s\n",
		       networkType, mode);
#endif

#ifdef _MSC_VER
    WSAStartup(MAKEWORD(1, 1), &wsaData);
    {
	int nOption = SO_SYNCHRONOUS_NONALERT;
	setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE,
		   (char *) &nOption, sizeof(int));
    }
#endif

#ifndef _MSC_VER
    (void) signal(SIGPIPE, SIG_IGN);
#endif
    (void) DUL_InitializeFSM();

    *networkKey = NULL;
#if 0
    if (networkInitialized++) {
	return COND_PushCondition(DUL_NETWORKINITIALIZED,
				  DUL_Message(DUL_NETWORKINITIALIZED));
    }
#endif

    cond = createNetworkKey(networkType, mode, timeout, opt, &key);
    if (!CTN_SUCCESS(cond)) {
#if 0
	networkInitialized = 0;
#endif
	return cond;
    }
    if (strcmp(networkType, DUL_NETWORK_TCP) == 0) {
	cond = initializeNetworkTCP(&key, networkParameter);
    }

    if (cond == DUL_NORMAL) {
	*networkKey = (DUL_NETWORKKEY *) key;
    } else {
	/* Added to fix memory leak.  From JCS */
	CTN_FREE(key);
    }
    return cond;
}

/* DUL_DropNetwork
**
** Purpose:
**	This function drops the network connection established by
**	by DUL_InitializeNetwork.  The caller passes the pointer to the
**	network key.  The network connection is closed and the caller's
**	key is zeroed to prevent misuse.
**
** Parameter Dictionary:
**	callerNetworkKey	Caller's handle to the network environment
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Close the socket and free the memeory occupied by the network key
*/

CONDITION
DUL_DropNetwork(DUL_NETWORKKEY ** callerNetworkKey)
{
    PRIVATE_NETWORKKEY
	** networkKey;
    CONDITION
	cond;

#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_DropNetwork\n");
#endif

    networkKey = (PRIVATE_NETWORKKEY **) callerNetworkKey;

    cond = checkNetwork(networkKey, "DUL_DropNetwork");
    if (cond != DUL_NORMAL)
	return cond;

    if ((*networkKey)->applicationFunction & PRV_APPLICATION_ACCEPTOR) {
	if (strcmp((*networkKey)->networkType, DUL_NETWORK_TCP) == 0) {
#ifdef _WIN32
	    (void) closesocket((*networkKey)->networkSpecific.TCP.listenSocket);
#else
	    (void) close((*networkKey)->networkSpecific.TCP.listenSocket);
#endif
	}
    }
#ifdef _MSC_VER
    (void) WSACleanup();
#endif

#if 0
    networkInitialized = 0;
#endif
    CTN_FREE(*networkKey);
    *networkKey = NULL;
    return DUL_NORMAL;
}

/* DUL_RequestAssociation
**
** Purpose:
**	This function requests an Association with another node.  The caller
**	identifies the network to be used, the name of the other node, a
**	network dependent parameter and a structure which defines the
**	Association parameters needed to establish an Association (the
**	syntax items).  This function establishes the Association and
**	returns the modified list of Association parameters and a key to
**	be used to access this Association.  If the caller does not choose
**	to recognize the parameters returned by the Acceptor, the caller
**	should abort the Association.
**
** Parameter Dictionary:
**	network		Caller's handle whcih describes the network environment
**	params		Pointer to list of parameters which describe the type
**			of Association requested by the caller.
**	association	Handle created by this function which describes the
**			Association; returned to the caller.
**
** Return Values:
**
**	DUL_ASSOCIATIONREJECTED
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NOCONNECTION
**	DUL_NORMAL
**	DUL_NULLKEY
**	DUL_REQUESTASSOCIATIONFAILED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_RequestAssociation(DUL_NETWORKKEY ** callerNetworkKey,
		       DUL_ASSOCIATESERVICEPARAMETERS * params,
		       DUL_ASSOCIATIONKEY ** callerAssociation)
{
  DUL_PROXY proxy;
  CTN_SOCKET* s = 0;
  PRIVATE_NETWORKKEY ** network;
  CONDITION cond;

  network = (PRIVATE_NETWORKKEY **) callerNetworkKey;
  cond = checkNetwork(network, "DUL_RequestAssociation");
  if (cond != DUL_NORMAL)
    return cond;
  if (((*network)->applicationFunction & PRV_APPLICATION_REQUESTOR) == 0) {
    return COND_PushCondition(DUL_ILLEGALREQUEST,
				  DUL_Message(DUL_ILLEGALREQUEST));
  }

  s = (CTN_SOCKET*) CTN_MALLOC(sizeof(*s));
  if (s == NULL) {
    return COND_PushCondition(DUL_MALLOCERROR, "DUL_RequestAssociation",
		sizeof(*s));
  }

  cond = requestAssociationTCP(network, params, s);
  if (cond != DUL_NORMAL) {
    return cond;
  }

  memset(&proxy, 0, sizeof(proxy));
  proxy.writeFunction = writeTCP;
  proxy.readFunction = readTCP;
  proxy.pollFunction = networkDataAvailable;
  proxy.closeFunction = closeTCP;
  proxy.proxyContext = s;

  cond = DUL_RequestAssociationWithProxy(callerNetworkKey,
		       params, &proxy, callerAssociation);
  return cond;
}


#if 0
{
    cond = createAssociationKey(network, "", params->maxPDU, association);
    if (cond != DUL_NORMAL)
	return cond;

    cond = requestAssociationTCP(network, params, association);
    (*association)->proxy.writeFunction = writeTCP;
    (*association)->proxy.readFunction = readTCP;
    (*association)->proxy.pollFunction = networkDataAvailable;
    (*association)->proxy.closeFunction = closeTCP;
    (*association)->proxy.proxyContext = &((*association)->networkSpecific.TCP.socket);
    (*association)->protocolState = STATE4;


    /* send a request primitive */
#if 0
    cond = PRV_StateMachine(network, association,
	     A_ASSOCIATE_REQ_LOCAL_USER, (*network)->protocolState, params);
#endif

    if (cond != DUL_NORMAL) {
	/*
	 * In case of an error, close the connection and go to the initial
	 * state
	 */
	cond = PRV_StateMachine(network, association,
		    TRANS_CONN_CLOSED, (*association)->protocolState, NULL);
	destroyAssociationKey(association);
	return COND_PushCondition(DUL_REQUESTASSOCIATIONFAILED,
				  DUL_Message(DUL_REQUESTASSOCIATIONFAILED));
    }
    cond = PRV_StateMachine(network, association,
      TRANS_CONN_CONFIRM_LOCAL_USER, (*association)->protocolState, params);
    if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    }
    /* Find the next event */
    cond = PRV_NextPDUType(association, DUL_BLOCK, PRV_DEFAULTTIMEOUT, &pduType);
    if (cond == DUL_NETWORKCLOSED)
	event = TRANS_CONN_CLOSED;
    else if (cond == DUL_READTIMEOUT)
	event = ARTIM_TIMER_EXPIRED;
    else if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    } else {
	switch (pduType) {
	case DUL_TYPEASSOCIATERQ:
	    event = A_ASSOCIATE_RQ_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATEAC:
	    event = A_ASSOCIATE_AC_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATERJ:
	    event = A_ASSOCIATE_RJ_PDU_RCV;
	    break;
	case DUL_TYPEDATA:
	    event = P_DATA_TF_PDU_RCV;
	    break;
	case DUL_TYPERELEASERQ:
	    event = A_RELEASE_RQ_PDU_RCV;
	    break;
	case DUL_TYPERELEASERP:
	    event = A_RELEASE_RP_PDU_RCV;
	    break;
	case DUL_TYPEABORT:
	    event = A_ABORT_PDU_RCV;
	    break;
	default:
	    event = INVALID_PDU;
	    break;
	}
    }

    cond = PRV_StateMachine(network, association, event,
			    (*association)->protocolState, params);
    if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    }
    return cond;
}
#endif

CONDITION
DUL_RequestAssociationWithProxy(DUL_NETWORKKEY ** callerNetworkKey,
		       DUL_ASSOCIATESERVICEPARAMETERS * params,
		       DUL_PROXY* proxy,
		       DUL_ASSOCIATIONKEY ** callerAssociation)
{
  PRIVATE_ASSOCIATIONKEY ** association;
  PRIVATE_NETWORKKEY ** network;
  CONDITION cond;
  unsigned char pduType;
  int event;

  network = (PRIVATE_NETWORKKEY **) callerNetworkKey;
  association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
  cond = checkNetwork(network, "DUL_RequestAssociationWithProxy");
  if (cond != DUL_NORMAL)
    return cond;

  if (((*network)->applicationFunction & PRV_APPLICATION_REQUESTOR) == 0) {
    return COND_PushCondition(DUL_ILLEGALREQUEST,
				  DUL_Message(DUL_ILLEGALREQUEST));
  }
  if (params->maxPDU < MIN_PDU_LENGTH || params->maxPDU > MAX_PDU_LENGTH)
    return COND_PushCondition(DUL_ILLEGALPARAMETER,
				DUL_Message(DUL_ILLEGALPARAMETER), "maxPDU",
				  "DUL_RequestAssociation");
  cond = createAssociationKey(network, "", params->maxPDU, association);
  if (cond != DUL_NORMAL)
    return cond;

  (*association)->proxy.writeFunction = proxy->writeFunction;
  (*association)->proxy.readFunction = proxy->readFunction;
  (*association)->proxy.pollFunction = proxy->pollFunction;
  (*association)->proxy.closeFunction = proxy->closeFunction;
  (*association)->proxy.proxyContext = proxy->proxyContext;
  (*association)->protocolState = STATE4;
  (*association)->networkState = NETWORK_CONNECTED;


    /* send a request primitive */

    if (cond != DUL_NORMAL) {
	/*
	 * In case of an error, close the connection and go to the initial
	 * state
	 */
	cond = PRV_StateMachine(network, association,
		    TRANS_CONN_CLOSED, (*association)->protocolState, NULL);
	destroyAssociationKey(association);
	return COND_PushCondition(DUL_REQUESTASSOCIATIONFAILED,
				  DUL_Message(DUL_REQUESTASSOCIATIONFAILED));
    }
    cond = PRV_StateMachine(network, association,
      TRANS_CONN_CONFIRM_LOCAL_USER, (*association)->protocolState, params);
    if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    }
    /* Find the next event */
    cond = PRV_NextPDUType(association, DUL_BLOCK, PRV_DEFAULTTIMEOUT, &pduType);
    if (cond == DUL_NETWORKCLOSED)
	event = TRANS_CONN_CLOSED;
    else if (cond == DUL_READTIMEOUT)
	event = ARTIM_TIMER_EXPIRED;
    else if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    } else {
	switch (pduType) {
	case DUL_TYPEASSOCIATERQ:
	    event = A_ASSOCIATE_RQ_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATEAC:
	    event = A_ASSOCIATE_AC_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATERJ:
	    event = A_ASSOCIATE_RJ_PDU_RCV;
	    break;
	case DUL_TYPEDATA:
	    event = P_DATA_TF_PDU_RCV;
	    break;
	case DUL_TYPERELEASERQ:
	    event = A_RELEASE_RQ_PDU_RCV;
	    break;
	case DUL_TYPERELEASERP:
	    event = A_RELEASE_RP_PDU_RCV;
	    break;
	case DUL_TYPEABORT:
	    event = A_ABORT_PDU_RCV;
	    break;
	default:
	    event = INVALID_PDU;
	    break;
	}
    }

    cond = PRV_StateMachine(network, association, event,
			    (*association)->protocolState, params);
    if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    }
    return cond;
}


/* DUL_ReqAssocExternalSocket
**
** Purpose:
**	This function requests an Association with another node.  The caller
**	identifies the network to be used, the name of the other node, a
**	network dependent parameter and a structure which defines the
**	Association parameters needed to establish an Association (the
**	syntax items).  This function establishes the Association and
**	returns the modified list of Association parameters and a key to
**	be used to access this Association.  If the caller does not choose
**	to recognize the parameters returned by the Acceptor, the caller
**	should abort the Association.
**	This function is related to DUL_RequestAssociation. In this function,
**	the caller provides an already open socket to the server.
**
** Parameter Dictionary:
**	network		Caller's handle whcih describes the network environment
**	params		Pointer to list of parameters which describe the type
**			of Association requested by the caller.
**	association	Handle created by this function which describes the
**			Association; returned to the caller.
**	sock		Existing socket to server.
**
** Return Values:
**
**	DUL_ASSOCIATIONREJECTED
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NOCONNECTION
**	DUL_NORMAL
**	DUL_NULLKEY
**	DUL_REQUESTASSOCIATIONFAILED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_ReqAssocExternalSocket(DUL_NETWORKKEY ** callerNetworkKey,
		       DUL_ASSOCIATESERVICEPARAMETERS * params,
		       DUL_ASSOCIATIONKEY ** callerAssociation,
			CTN_SOCKET sock)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    PRIVATE_NETWORKKEY
	** network;
    CONDITION
	cond;
    unsigned char
        pduType;
    int
        event;

    network = (PRIVATE_NETWORKKEY **) callerNetworkKey;
    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkNetwork(network, "DUL_RequestAssociation");
    if (cond != DUL_NORMAL)
	return cond;

#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_Request Association \n");
#endif

    if (((*network)->applicationFunction & PRV_APPLICATION_REQUESTOR) == 0) {
	return COND_PushCondition(DUL_ILLEGALREQUEST,
				  DUL_Message(DUL_ILLEGALREQUEST));
    }
    if (params->maxPDU < MIN_PDU_LENGTH || params->maxPDU > MAX_PDU_LENGTH)
	return COND_PushCondition(DUL_ILLEGALPARAMETER,
				DUL_Message(DUL_ILLEGALPARAMETER), "maxPDU",
				  "DUL_RequestAssociation");
    cond = createAssociationKey(network, "", params->maxPDU, association);
    if (cond != DUL_NORMAL)
	return cond;

#ifdef SMM_MAR31
    (*association)->networkSpecific.TCP.socket = sock;
#endif
    (*association)->isExternalSocket = 1;

    /* send a request primitive */
    cond = PRV_StateMachine(network, association,
	     A_ASSOCIATE_REQ_LOCAL_USER, (*network)->protocolState, params);

    if (cond != DUL_NORMAL) {
	/*
	 * In case of an error, close the connection and go to the initial
	 * state
	 */
	cond = PRV_StateMachine(network, association,
		    TRANS_CONN_CLOSED, (*association)->protocolState, NULL);
	destroyAssociationKey(association);
	return COND_PushCondition(DUL_REQUESTASSOCIATIONFAILED,
				  DUL_Message(DUL_REQUESTASSOCIATIONFAILED));
    }
    cond = PRV_StateMachine(network, association,
      TRANS_CONN_CONFIRM_LOCAL_USER, (*association)->protocolState, params);
    if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    }
    /* Find the next event */
    cond = PRV_NextPDUType(association, DUL_BLOCK, PRV_DEFAULTTIMEOUT, &pduType);
    if (cond == DUL_NETWORKCLOSED)
	event = TRANS_CONN_CLOSED;
    else if (cond == DUL_READTIMEOUT)
	event = ARTIM_TIMER_EXPIRED;
    else if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    } else {
	switch (pduType) {
	case DUL_TYPEASSOCIATERQ:
	    event = A_ASSOCIATE_RQ_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATEAC:
	    event = A_ASSOCIATE_AC_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATERJ:
	    event = A_ASSOCIATE_RJ_PDU_RCV;
	    break;
	case DUL_TYPEDATA:
	    event = P_DATA_TF_PDU_RCV;
	    break;
	case DUL_TYPERELEASERQ:
	    event = A_RELEASE_RQ_PDU_RCV;
	    break;
	case DUL_TYPERELEASERP:
	    event = A_RELEASE_RP_PDU_RCV;
	    break;
	case DUL_TYPEABORT:
	    event = A_ABORT_PDU_RCV;
	    break;
	default:
	    event = INVALID_PDU;
	    break;
	}
    }

    cond = PRV_StateMachine(network, association, event,
			    (*association)->protocolState, params);
    if (cond != DUL_NORMAL) {
	destroyAssociationKey(association);
	return cond;
    }
    return cond;
}

/* DUL_ReceiveAssociationRQ
**
** Purpose:
**	This function allows a caller to ask to receive an Association Request.
**	If the function does receive such a request, it returns a list of
**	Association Items which describe the proposed Association and an
**	AssociationKey used to access this Association.  The user should call
**	DUL_AcknowledgeAssociateRQ if the Association is to be accepted.
**
** Parameter Dictionary:
**	network		Caller's handle to the network environment.
**	block		Flag indicating blocking/non blocking mode
**	params		Pointer to a structure holding parameters which
**			describe this Association.
**	association	Caller handle for thsi association that is created
**			by this function.
**
** Return Values:
**
**	DUL_ABORTEDREQUEST
**	DUL_ILLEGALACCEPT
**	DUL_ILLEGALKEY
**	DUL_ILLEGALPARAMETER
**	DUL_ILLEGALREQUEST
**	DUL_NETWORKCLOSED
**	DUL_NORMAL
**	DUL_NULLKEY
**	DUL_UNKNOWNREMOTENODE
**	DUL_UNSUPPORTEDNETWORK
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_ReceiveAssociationRQ(DUL_NETWORKKEY ** callerNetworkKey,
			 DUL_BLOCKOPTIONS block,
			 DUL_ASSOCIATESERVICEPARAMETERS * params,
			 DUL_ASSOCIATIONKEY ** callerAssociation)
{
    PRIVATE_NETWORKKEY
	** network;
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION cond;
    int networkClosed = 0;
    DUL_PROXY proxy;
    CTN_SOCKET* s = 0;

    network = (PRIVATE_NETWORKKEY **) callerNetworkKey;
    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkNetwork(network, "DUL_ReceiveAssociationRQ");
    if (cond != DUL_NORMAL)
	return cond;

    if (((*network)->applicationFunction & PRV_APPLICATION_ACCEPTOR) == 0) {
	return COND_PushCondition(DUL_ILLEGALACCEPT,
				  DUL_Message(DUL_ILLEGALACCEPT));
    }
#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_Receive Association RQ\n");
#endif

    if (params->maxPDU < MIN_PDU_LENGTH || params->maxPDU > MAX_PDU_LENGTH)
	return COND_PushCondition(DUL_ILLEGALPARAMETER,
				DUL_Message(DUL_ILLEGALPARAMETER), "maxPDU",
				  "DUL_RequestAssociation");
  s = (CTN_SOCKET*) CTN_MALLOC(sizeof(*s));
  if (s == NULL) {
    return COND_PushCondition(DUL_MALLOCERROR, "DUL_ReceiveAssociationRequest",
		sizeof(*s));
  }
  clearRequestorsParams(params);
  cond = receiveTransportConnection(network, block, params, s);
  if (cond != DUL_NORMAL) {
    destroyAssociationKey(association);
    return cond;
  }
  memset(&proxy, 0, sizeof(proxy));
  proxy.writeFunction = writeTCP;
  proxy.readFunction = readTCP;
  proxy.pollFunction = networkDataAvailable;
  proxy.closeFunction = closeTCP;
  proxy.proxyContext = s;
  cond = DUL_ReceiveAssociationRQWithProxy(callerNetworkKey, block,
			 params, &proxy, callerAssociation);

  return cond;

#ifdef SMM_MAR31
  (*association)->proxy.writeFunction = proxy.writeFunction;
  (*association)->proxy.readFunction = proxy.readFunction;
  (*association)->proxy.pollFunction = proxy.pollFunction;
  (*association)->proxy.closeFunction = proxy.closeFunction;
  (*association)->proxy.proxyContext = proxy.proxyContext;
  (*association)->networkState = NETWORK_CONNECTED;

#if 0
    if (cond == DUL_NOASSOCIATIONREQUEST) {
	/* From JCS, free the association key, otherwise a DropAssociation
	**  will try to close an unopen socket.
	*/
	destroyAssociationKey(association);
    } else if (cond != DUL_NORMAL) {
	return cond;
    }
#endif

    cond = PRV_StateMachine(network, association,
		  TRANS_CONN_INDICATION, (*network)->protocolState, params);
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_NextPDUType(association, DUL_NOBLOCK, PRV_DEFAULTTIMEOUT, &pduType);
    if (cond == DUL_NETWORKCLOSED) {
	event = TRANS_CONN_CLOSED;
	networkClosed = 1;
    } else if (cond == DUL_READTIMEOUT)
	event = ARTIM_TIMER_EXPIRED;
    else if (cond != DUL_NORMAL)
	return cond;
    else {
	switch (pduType) {
	case DUL_TYPEASSOCIATERQ:
	    event = A_ASSOCIATE_RQ_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATEAC:
	    event = A_ASSOCIATE_AC_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATERJ:
	    event = A_ASSOCIATE_RJ_PDU_RCV;
	    break;
	case DUL_TYPEDATA:
	    event = P_DATA_TF_PDU_RCV;
	    break;
	case DUL_TYPERELEASERQ:
	    event = A_RELEASE_RQ_PDU_RCV;
	    break;
	case DUL_TYPERELEASERP:
	    event = A_RELEASE_RP_PDU_RCV;
	    break;
	case DUL_TYPEABORT:
	    event = A_ABORT_PDU_RCV;
	    break;
	default:
	    event = INVALID_PDU;
	    break;
	}
    }
    cond = PRV_StateMachine(network, association, event,
			    (*association)->protocolState, params);
    if (cond == DUL_UNSUPPORTEDPEERPROTOCOL) {
	abortItems.result = DUL_REJECT_PERMANENT;
	abortItems.source = DUL_ULSP_ACSE_REJECT;
	abortItems.reason = DUL_ULSP_ACSE_UNSUP_PROTOCOL;
/*	(void) DUL_RejectAssociationRQ(callerAssociation, &abortItems); */
	(void) PRV_StateMachine(NULL, association,
				A_ASSOCIATE_RESPONSE_REJECT,
				(*association)->protocolState, &abortItems);
    }
    if (networkClosed)
      return DUL_NETWORKCLOSED;

    return cond;
#endif
}

CONDITION
DUL_ReceiveAssociationRQWithProxy(DUL_NETWORKKEY ** callerNetworkKey,
			 DUL_BLOCKOPTIONS block,
			 DUL_ASSOCIATESERVICEPARAMETERS * params,
		         DUL_PROXY* proxy,
			 DUL_ASSOCIATIONKEY ** callerAssociation)
{
  PRIVATE_NETWORKKEY ** network;
  PRIVATE_ASSOCIATIONKEY ** association;
  CONDITION cond;
  unsigned char pduType;
  int event;
  DUL_ABORTITEMS abortItems;
  int networkClosed = 0;

  network = (PRIVATE_NETWORKKEY **) callerNetworkKey;
  if (*network == 0) {
    DUL_InitializeFSM();
    cond = createNetworkKey(DUL_NETWORK_TCP, DUL_AEBOTH,
	DUL_TIMEOUT, DUL_ORDERBIGENDIAN, network);
    if (!CTN_SUCCESS(cond)) {
	return cond;
    }
  }

  association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
  cond = checkNetwork(network, "DUL_ReceiveAssociationRQWithProxy");
  if (cond != DUL_NORMAL)
    return cond;

  if (params->maxPDU < MIN_PDU_LENGTH || params->maxPDU > MAX_PDU_LENGTH)
    return COND_PushCondition(DUL_ILLEGALPARAMETER,
				DUL_Message(DUL_ILLEGALPARAMETER), "maxPDU",
				  "DUL_RequestAssociationWithProxy");

  cond = createAssociationKey(network, params->callingPresentationAddress,
				params->maxPDU, association);
  if (cond != DUL_NORMAL)
    return cond;

  (*association)->proxy.writeFunction = proxy->writeFunction;
  (*association)->proxy.readFunction = proxy->readFunction;
  (*association)->proxy.pollFunction = proxy->pollFunction;
  (*association)->proxy.closeFunction = proxy->closeFunction;
  (*association)->proxy.proxyContext = proxy->proxyContext;
  (*association)->networkState = NETWORK_CONNECTED;

  (*network)->networkState = NETWORK_DISCONNECTED;	/* The state machine thinks this */
  (*network)->protocolState = STATE1;

  cond = PRV_StateMachine(network, association,
		  TRANS_CONN_INDICATION, (*network)->protocolState, params);
  if (cond != DUL_NORMAL)
    return cond;

  cond = PRV_NextPDUType(association, DUL_NOBLOCK, PRV_DEFAULTTIMEOUT, &pduType);

  if (cond == DUL_NETWORKCLOSED) {
    event = TRANS_CONN_CLOSED;
    networkClosed = 1;
  } else if (cond == DUL_READTIMEOUT)
    event = ARTIM_TIMER_EXPIRED;
  else if (cond != DUL_NORMAL)
    return cond;
  else {
    switch (pduType) {
    case DUL_TYPEASSOCIATERQ:
      event = A_ASSOCIATE_RQ_PDU_RCV;
      break;
    case DUL_TYPEASSOCIATEAC:
      event = A_ASSOCIATE_AC_PDU_RCV;
      break;
    case DUL_TYPEASSOCIATERJ:
      event = A_ASSOCIATE_RJ_PDU_RCV;
      break;
    case DUL_TYPEDATA:
      event = P_DATA_TF_PDU_RCV;
      break;
    case DUL_TYPERELEASERQ:
      event = A_RELEASE_RQ_PDU_RCV;
      break;
    case DUL_TYPERELEASERP:
      event = A_RELEASE_RP_PDU_RCV;
      break;
    case DUL_TYPEABORT:
      event = A_ABORT_PDU_RCV;
      break;
    default:
      event = INVALID_PDU;
      break;
    }
  }
  cond = PRV_StateMachine(network, association, event,
			    (*association)->protocolState, params);
  if (cond == DUL_UNSUPPORTEDPEERPROTOCOL) {
    abortItems.result = DUL_REJECT_PERMANENT;
    abortItems.source = DUL_ULSP_ACSE_REJECT;
    abortItems.reason = DUL_ULSP_ACSE_UNSUP_PROTOCOL;
/*    (void) DUL_RejectAssociationRQ(callerAssociation, &abortItems); */
    (void) PRV_StateMachine(NULL, association,
				A_ASSOCIATE_RESPONSE_REJECT,
				(*association)->protocolState, &abortItems);
  }
  if (networkClosed)
    return DUL_NETWORKCLOSED;

  return cond;
}




/* DUL_AcknowledgeAssociationRQ
**
** Purpose:
**	This function is used by an Acceptor to acknowledge an Association
**	Request.  The caller passes the partially completed Association Key
**	and the Association Items (syntax items) which describe the Association.
**	This function constructs an Acknowledge PDU and sends it to the
**	Requestor.  At this point, an Association is established.
**
** Parameter Dictionary:
**	association	Caller's handle to the Association that is to be
**			acknowledged and hence established.
**	params		Parameters that describe the type of service handled
**			by this Association.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_AcknowledgeAssociationRQ(DUL_ASSOCIATIONKEY ** callerAssociation,
			     DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_AcknowledgeAssociationRQ");
    if (cond != DUL_NORMAL)
	return cond;

#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_Acknowledge Association RQ\n");
#endif

#ifdef BLOG
    if (blog) {
	char title[40];
	U32 type = DUL_TYPEASSOCIATERQ;
	sprintf(title, "%s.%s", params->callingAPTitle, params->calledAPTitle);
	BLG_Create(NULL, title, &(*association)->logHandle);
	BLG_Write(&(*association)->logHandle, type,
		  (*association)->fragmentBuffer,
		  (*association)->nextPDULength);
    }
#endif

    cond = PRV_StateMachine(NULL, association,
	A_ASSOCIATE_RESPONSE_ACCEPT, (*association)->protocolState, params);

    return cond;

}


/* DUL_RejectAssociationRQ
**
** Purpose:
**	This function is used to reject an Association Request and destroy
**	the corresponding Association Key.  The caller indicates if the
**	reject is permanent or transient and the reason for the reject.
**	This function constructs a Reject PDU and sends it to the connected
**	peer.  The Association Key is then destroyed and the caller's reference
**	to the key destroyed to prevent misuse.
**
** Parameter Dictionary:
**	association	Caller's handle to the association that is to be
**			rejected.
**	params		Pointer to a structure that gives the reasons for
**			rejecting this Association.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREJECTREASON
**	DUL_ILLEGALREJECTRESULT
**	DUL_ILLEGALREQUEST
**	DUL_NETWORKCLOSED
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_RejectAssociationRQ(DUL_ASSOCIATIONKEY ** callerAssociation,
			DUL_ABORTITEMS * params)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;
    DUL_ABORTITEMS
	localParams;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_RejectAssociationRQ");
    if (cond != DUL_NORMAL)
	return cond;

#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_Reject Association RQ\n");
#endif

    localParams = *params;
    localParams.source = 0x01;
    {
	unsigned char diagtable[] = {0x01, 0x02, 0x03, 0x07};
	int index;
	CTNBOOLEAN found = FALSE;
	for (index = 0; index < (int) DIM_OF(diagtable) && !found; index++)
	    found = (localParams.reason == diagtable[index]);

	if (!found)
	    return COND_PushCondition(DUL_ILLEGALREJECTREASON,
		  DUL_Message(DUL_ILLEGALREJECTREASON), localParams.reason);
    }
    {
	unsigned char resulttable[] = {0x01, 0x02};
	int index;
	CTNBOOLEAN found = FALSE;
	for (index = 0; index < (int) DIM_OF(resulttable) && !found; index++)
	    found = (localParams.result == resulttable[index]);

	if (!found)
	    return COND_PushCondition(DUL_ILLEGALREJECTRESULT,
		  DUL_Message(DUL_ILLEGALREJECTRESULT), localParams.result);
    }
    cond = PRV_StateMachine(NULL, association,
			    A_ASSOCIATE_RESPONSE_REJECT, (*association)->protocolState, &localParams);
    if (!CTN_SUCCESS(cond))
	return cond;
    cond = PRV_StateMachine(NULL, association,
		  ARTIM_TIMER_EXPIRED, (*association)->protocolState, NULL);

    return cond;
}


/* DUL_DropAssociation
**
** Purpose:
**	This function drops an Association without notifying the peer
**	application and destroys the Association Key.  The caller's
**	reference to the Association Key is also destroyed to prevent misuse.
**	This routine is called if a peer disconnects from you or if you
**	want to drop an Association without notifying the peer (can be
**	used in the unix environment if you use the fork call and have
**	two open channels to the peer).
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle to the Association that is
**				to be dropped.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_DropAssociation(DUL_ASSOCIATIONKEY ** callerAssociation)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_DropAssociation");
    if (cond != DUL_NORMAL)
	return cond;

#ifdef DEBUG
    if (debug)
	(void) fprintf(DEBUG_DEVICE, "DUL_DropAssociation\n");
#endif

    if (strcmp((*association)->networkType, DUL_NETWORK_TCP) == 0) {
#ifdef SMM_MAR31
	(void) close((*association)->networkSpecific.TCP.socket);
#endif

	(*association)->proxy.closeFunction(
		(*association)->proxy.proxyContext);
#if 0
#ifdef _WIN32
		(void) closesocket((*association)->networkSpecific.TCP.socket);
#else
		(void) close((*association)->networkSpecific.TCP.socket);
#endif
#endif
	destroyAssociationKey(association);
    }
    return DUL_NORMAL;

}


/* DUL_ReleaseAssociation
**
** Purpose:
**	Request the orderly release of an Association.
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle to the Association to be
**				released.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NETWORKCLOSED
**	DUL_NORMAL
**	DUL_NULLKEY
**	DUL_PEERREQUESTEDRELEASE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_ReleaseAssociation(DUL_ASSOCIATIONKEY ** callerAssociation)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    unsigned char
        pduType;
    int
        event;
    CONDITION
	cond;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_ReleaseAssociation");
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_StateMachine(NULL, association,
			A_RELEASE_REQ, (*association)->protocolState, NULL);
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_NextPDUType(association, DUL_NOBLOCK, PRV_DEFAULTTIMEOUT, &pduType);
    if (cond == DUL_NETWORKCLOSED) {
	event = TRANS_CONN_CLOSED;
	if (debug)
	  fprintf(stderr, "DUL_ReleaseAssociation:: event = Network closed \n");

    } else if (cond == DUL_READTIMEOUT) {
	event = ARTIM_TIMER_EXPIRED;
	if (debug)
	  fprintf(stderr, "DUL_ReleaseAssociation:: event = Timer expired \n");
    } else if (cond != DUL_NORMAL) {
	if (debug)
	  fprintf(stderr, "DUL_ReleaseAssociation:: non-normal condition %08x \n", cond);
	return cond;
    } else {
	switch (pduType) {
	case DUL_TYPEASSOCIATERQ:
	    event = A_ASSOCIATE_RQ_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_ASSOCIATE_RQ_PDU_RCV \n");
	    break;
	case DUL_TYPEASSOCIATEAC:
	    event = A_ASSOCIATE_AC_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_ASSOCIATE_AC_PDU_RCV \n");
	    break;
	case DUL_TYPEASSOCIATERJ:
	    event = A_ASSOCIATE_RJ_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_ASSOCIATE_RJ_PDU_RCV \n");
	    break;
	case DUL_TYPEDATA:
	    event = P_DATA_TF_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = P_DATA_TF_PDU_RCV \n");
	    break;
	case DUL_TYPERELEASERQ:
	    event = A_RELEASE_RQ_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_RELEASE_RQ_PDU_RCV\n");
	    break;
	case DUL_TYPERELEASERP:
	    event = A_RELEASE_RP_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_RELEASE_RP_PDU_RCV\n");
	    break;
	case DUL_TYPEABORT:
	    event = A_ABORT_PDU_RCV;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_ABORT_PDU_RCV\n");
	    break;
	default:
	    event = INVALID_PDU;
	    if (debug)
	      fprintf(stderr, "DUL_ReleaseAssociation:: event = A_ASSOCIATE_RQ_PDU_RCV \n");
	    break;
	}
    }
    cond = PRV_StateMachine(NULL, association, event,
			    (*association)->protocolState, NULL);

    if (debug) {
      fprintf(stderr, "DUL_ReleaseAssociation:: final condition = %08x\n", cond);
    }

    if (cond == DUL_READTIMEOUT)
	return COND_PushCondition(DUL_READTIMEOUT, DUL_Message(DUL_READTIMEOUT));
    else
	return cond;
}

/* DUL_AcknowledgeRelease
**
** Purpose:
**	Send Acknowledgement to the Release request
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle to the Association whose
**				release is acknowledged.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_AcknowledgeRelease(DUL_ASSOCIATIONKEY ** callerAssociation)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_AcknowledgeRelease");
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_StateMachine(NULL, association,
		       A_RELEASE_RESP, (*association)->protocolState, NULL);
    if (!CTN_SUCCESS(cond))
	return cond;
#ifdef LINUX
    usleep(100000);
#endif
    cond = PRV_StateMachine(NULL, association,
		  ARTIM_TIMER_EXPIRED, (*association)->protocolState, NULL);
    return cond;
}



/* DUL_AbortAssociation
**
** Purpose:
**	Abort an association by sending an A-ABORT PDU and waiting for the
**	network to close.
**
** Parameter Dictionary:
**	callerAssociation	The handle for the association to be
**				aborted.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NETWORKCLOSED
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_AbortAssociation(DUL_ASSOCIATIONKEY ** callerAssociation)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;
    DUL_ABORTITEMS
	abortItems = {
	0, DUL_SCU_INITIATED_ABORT, 0
    };
    int
        event;
    unsigned char
        pduType;
    CTNBOOLEAN
	done;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_AbortAssociation");
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_StateMachine(NULL, association, A_ABORT_REQ,
			    (*association)->protocolState, &abortItems);

    if (cond != DUL_NORMAL)
	return cond;

    done = FALSE;
    while (!done) {
	cond = PRV_NextPDUType(association, DUL_NOBLOCK, PRV_DEFAULTTIMEOUT, &pduType);
	if (cond == DUL_NETWORKCLOSED)
	    event = TRANS_CONN_CLOSED;
	else if (cond == DUL_READTIMEOUT)
	    event = ARTIM_TIMER_EXPIRED;
	else {
	    switch (pduType) {
	    case DUL_TYPEASSOCIATERQ:
		event = A_ASSOCIATE_RQ_PDU_RCV;
		break;
	    case DUL_TYPEASSOCIATEAC:
		event = A_ASSOCIATE_AC_PDU_RCV;
		break;
	    case DUL_TYPEASSOCIATERJ:
		event = A_ASSOCIATE_RJ_PDU_RCV;
		break;
	    case DUL_TYPEDATA:
		event = P_DATA_TF_PDU_RCV;
		break;
	    case DUL_TYPERELEASERQ:
		event = A_RELEASE_RQ_PDU_RCV;
		break;
	    case DUL_TYPERELEASERP:
		event = A_RELEASE_RP_PDU_RCV;
		break;
	    case DUL_TYPEABORT:
		event = A_ABORT_PDU_RCV;
		break;
	    default:
		event = INVALID_PDU;
		break;
	    }
	}

	cond = PRV_StateMachine(NULL, association, event,
				(*association)->protocolState, NULL);

	if (cond == DUL_NORMAL)
	    done = TRUE;
    }
    return DUL_NORMAL;
}



/* DUL_WritePDVs
**
** Purpose:
**	Write a list of PDVs on an active Association.
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle to the Association
**	pdvList			Pointer to a structure which describes
**				the list of PDVs to be written on the active
**				Association.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NETWORKCLOSED
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_WritePDVs(DUL_ASSOCIATIONKEY ** callerAssociation,
	      DUL_PDVLIST * pdvList)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_WritePDVs");
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_StateMachine(NULL, association, P_DATA_REQ,
			    (*association)->protocolState, pdvList);

    return cond;
}


/* DUL_ReadPDVs
**
** Purpose:
**	Read the next available PDU.
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle for the Association.
**      pdvList                 Pointer to a structure which describes
**                              the list of PDVs to be read from the active
**                              Association.
**	block			Option used for blocking/non-blocking read.
**	timeout			Timeout interval.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NETWORKCLOSED
**	DUL_NORMAL
**	DUL_NULLKEY
**	DUL_PEERABORTEDASSOCIATION
**	DUL_PEERDROPPEDASSOCIATION
**	DUL_PEERREQUESTEDRELEASE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_ReadPDVs(DUL_ASSOCIATIONKEY ** callerAssociation,
	     DUL_PDVLIST * pdvList, DUL_BLOCKOPTIONS block, int timeout)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;
    unsigned char
        pduType;
    int
        event;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_ReadPDVs");
    if (cond != DUL_NORMAL)
	return cond;

    cond = PRV_NextPDUType(association, block, timeout, &pduType);
    if (cond == DUL_NETWORKCLOSED)
	event = TRANS_CONN_CLOSED;
    else if ((cond == DUL_READTIMEOUT) && (block == DUL_NOBLOCK))
	return cond;
    else if (cond == DUL_READTIMEOUT)
	event = ARTIM_TIMER_EXPIRED;
    else if (cond != DUL_NORMAL)
	return cond;
    else {
	switch (pduType) {
	case DUL_TYPEASSOCIATERQ:
	    event = A_ASSOCIATE_RQ_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATEAC:
	    event = A_ASSOCIATE_AC_PDU_RCV;
	    break;
	case DUL_TYPEASSOCIATERJ:
	    event = A_ASSOCIATE_RJ_PDU_RCV;
	    break;
	case DUL_TYPEDATA:
	    event = P_DATA_TF_PDU_RCV;
	    break;
	case DUL_TYPERELEASERQ:
	    event = A_RELEASE_RQ_PDU_RCV;
	    break;
	case DUL_TYPERELEASERP:
	    event = A_RELEASE_RP_PDU_RCV;
	    break;
	case DUL_TYPEABORT:
	    event = A_ABORT_PDU_RCV;
	    break;
	default:
	    event = INVALID_PDU;
	    break;
	}
    }
    cond = PRV_StateMachine(NULL, association, event,
			    (*association)->protocolState, pdvList);

    return cond;
}

/* DUL_Debug
**
** Purpose:
**	Set debug flag in this module and in the other modules.
**
** Parameter Dictionary:
**	flag	The boolean variable to set the debug facility.
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
DUL_Debug(CTNBOOLEAN flag)
{
    debug = flag;
    fsmDebug(flag);
    constructDebug(flag);
    parseDebug(flag);
#ifdef SNOOP
    snoopDebug(flag);
#endif
}

void
DUL_Blog(CTNBOOLEAN flag)
{
#ifdef BLOG
    blog = flag;
    fsmBlog(flag);
#endif
}

/* DUL_AssociationParameter
**
** Purpose:
**	This function examines the Association parameters and returns
**	a single parameter requested by the caller.
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle to the Association.
**	param			The only supported value is DUL_K_MAX_PDV_XMIT.
**	type			The TYPE of the parameter requested.
**	address			Requested parameter to be obtained from the
**				Association key.
**	length			The size of the parameter requested.
**
** Return Values:
**
**	DUL_ASSOCIATIONPARAMETERFAILED
**	DUL_ILLEGALKEY
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_AssociationParameter(DUL_ASSOCIATIONKEY ** callerAssociation,
			 DUL_ASSOCIATION_PARAMETER param, DUL_DATA_TYPE type,
			 void *address, size_t length)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_AssociationParameter");
    if (cond != DUL_NORMAL)
	return cond;

    switch (param) {
    case DUL_K_MAX_PDV_XMIT:
/*	if (strcmp((*association)->applicationType, AE_ACCEPTOR) == 0) { */
	if ((*association)->applicationFunction == PRV_APPLICATION_ACCEPTOR) {
	    cond = get_association_parameter(
					 &((*association)->maxPDVRequestor),
		     DUL_K_INTEGER, sizeof((*association)->maxPDVRequestor),
					     type, address, length);
	} else {
	    cond = get_association_parameter(
					  &((*association)->maxPDVAcceptor),
		      DUL_K_INTEGER, sizeof((*association)->maxPDVAcceptor),
					     type, address, length);
	}
	if (cond != DUL_NORMAL)
	    cond = COND_PushCondition(DUL_ASSOCIATIONPARAMETERFAILED,
				DUL_Message(DUL_ASSOCIATIONPARAMETERFAILED),
				      "DUL_K_MAX_PDU_XMIT");
	break;
    }
    return cond;
}


/* DUL_NextPDV
**
** Purpose:
**	Return the next PDV to the caller.
**
** Parameter Dictionary:
**	callerAssociation	Caller's handle to the Association
**	pdv			The next PDU to be returned
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NOPDVS
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_NextPDV(DUL_ASSOCIATIONKEY ** callerAssociation, DUL_PDV * pdv)
{
    PRIVATE_ASSOCIATIONKEY
	** association;
    CONDITION
	cond;
    unsigned char
       *p;
    unsigned long
        pdvLength;

    association = (PRIVATE_ASSOCIATIONKEY **) callerAssociation;
    cond = checkAssociation(association, "DUL_NextPDV");
    if (cond != DUL_NORMAL)
	return cond;

    if ((*association)->pdvIndex == -1)
	return COND_PushCondition(DUL_NOPDVS, DUL_Message(DUL_NOPDVS));

    *pdv = (*association)->currentPDV;

    (*association)->pdvIndex++;
    if ((*association)->pdvIndex >= (*association)->pdvCount)
	(*association)->pdvIndex = -1;
    else {
	unsigned char u;
	p = (*association)->pdvPointer;
	p += (*association)->currentPDV.fragmentLength + 2 + 4;

	EXTRACT_LONG_BIG(p, pdvLength);
	(*association)->currentPDV.fragmentLength = pdvLength - 2;
	(*association)->currentPDV.presentationContextID = p[4];

	u = p[5];
	if (u & 2)
	    (*association)->currentPDV.lastPDV = TRUE;
	else
	    (*association)->currentPDV.lastPDV = FALSE;

	if (u & 1)
	    (*association)->currentPDV.pdvType = DUL_COMMANDPDV;
	else
	    (*association)->currentPDV.pdvType = DUL_DATASETPDV;

	(*association)->currentPDV.data = p + 6;
	(*association)->pdvPointer += 4 + pdvLength;
    }
    return DUL_NORMAL;
}


/* DUL_ClearServiceParameters
**
** Purpose:
**	DUL_ClearServiceParameters is used to clear the lists that were
**	constructed in a DUL_ASSOCIATEDSERVICEPARAMETERS structure.  The
**	lists in that structure were built to contain the requested
**	and accepted Presentation Contexts.  Once the lists were built,
**	an Association request could be made.  After the Association
**	has been closed, the Presentation Contexts need to be discarded
**	so that the same structure can be reused for a new Association.
**	This function removes all of the nodes from the requested
**	and accepted Presentation Context lists and then destroys the
**	lists themselves.
**
** Parameter Dictionary:
**	params	Pointer to a DUL_ASSOCIATESERVICEPARAMETERS structure
**		to be cleared.
**
** Return Values:
**	DUL_NORMAL
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DUL_ClearServiceParameters(DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    clearPresentationContext(&params->requestedPresentationContext);
    clearPresentationContext(&params->acceptedPresentationContext);
    return DUL_NORMAL;
}

/* DUL_DefaultServiceParameters
**
** Purpose:
**	DUL_DefaultServiceParameters is used to set a number of default
**	parameters in a DUL_ASSOCIATESERVICEPARAMETERS structure.  These
**	parameters are the default MIR parameters (which should be
**	adequate for most Unix installations).
**
** Parameter Dictionary:
**	params	Pointer to a DUL_ASSOCIATESERVICEPARAMETERS structure
**		to be set.
**
** Return Values:
**	none
** Notes:
**
** Algorithm:
*/

void
DUL_DefaultServiceParameters(DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    static DUL_ASSOCIATESERVICEPARAMETERS p = {
	DICOM_STDAPPLICATIONCONTEXT,	/* Application Ctx Name */
	"Calling AP Title",	/* Calling AP Title */
	"Called AP Title",	/* Called AP Title */
	"",			/* Responding AP Title */
	16384,			/* Max PDU */
	0,			/* result */
	0,			/* result source */
	0,			/* diagnostic */
	"localhost",		/* Calling presentation addr */
	"localhost:104",	/* Called presentation addr */
	NULL,			/* Requested presentation ctx list */
	NULL,			/* Accepted presentation ctx list */
	0,			/* Maximum operations invoked */
	0,			/* Maximum operations performed */
	MIR_IMPLEMENTATIONCLASSUID,	/* Calling implementation class UID */
	MIR_IMPLEMENTATIONVERSIONNAME,	/* Calling implementation vers name */
	"",			/* Called implementation class UID */
	"",			/* Called implementation vers name */
	0			/* peer max pdu */
    };

    *params = p;
}

/*      ==============================================================
 *  Private functions not meant for users of the package are included below.
 */


/* receiveTransportConnection
**
** Purpose:
**	Receive a transport connection and fill in various fields of the
**	service parameters and Association handle.
** Parameter Dictionary:
**	network		Pointer to a structure maintaining information about
**			the network environment.
**	block		Option indicating blocking/non-blocking mode
**	params		Pointer to structure describing the services for the
**			Association.
**	association	Handle to the association
**
** Return Values:
**
**	DUL_NOASSOCIATIONREQUEST
**	DUL_NORMAL
**	DUL_TCPINITERROR
**	DUL_UNKNOWNREMOTENODE
**	DUL_UNSUPPORTEDNETWORK
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
receiveTransportConnection(PRIVATE_NETWORKKEY ** network,
			   DUL_BLOCKOPTIONS block,
			   DUL_ASSOCIATESERVICEPARAMETERS * params,
			   CTN_SOCKET* s)
{
    if (strcmp((*network)->networkType, DUL_NETWORK_TCP) == 0)
	return receiveTransportConnectionTCP(network, block, params,
					     s);
    else
	return COND_PushCondition(DUL_UNSUPPORTEDNETWORK,
				  DUL_Message(DUL_UNSUPPORTEDNETWORK),
				  (*network)->networkType);
}


/* receiveTransportConnectionTCP
**
** Purpose:
**	Receive a TCP transport connection and fill in the fields of the
**	service parameters and the Association handle.
**
** Parameter Dictionary:
**	network		Pointer to a structure maintaining information about
**			the network environment.
**	block		Option indicating blocking/non-blocking mode
**	params		Pointer to structure describing the services for the
**			Association.
**	association	Handle to the association
**
** Return Values:
**
**	DUL_NOASSOCIATIONREQUEST
**	DUL_NORMAL
**	DUL_TCPINITERROR
**	DUL_UNKNOWNREMOTENODE
**	DUL_UNSUPPORTEDNETWORK
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
receiveTransportConnectionTCP(PRIVATE_NETWORKKEY ** network,
			      DUL_BLOCKOPTIONS block,
			      DUL_ASSOCIATESERVICEPARAMETERS * params,
			      CTN_SOCKET* s)
{
    fd_set
    fdset;
    struct timeval
        timeout;
    int
        len,
        nfound,
        connected;
    struct sockaddr from;
    struct hostent *remote;
    struct linger sockarg;
    int reuse = 1;
#ifdef CTN_USE_THREADS
    struct hostent hStorage;
    char hBuf[512];
    int hError = 0;
#endif

    if (block == DUL_NOBLOCK) {
	connected = 0;
	FD_ZERO(&fdset);
	FD_SET((*network)->networkSpecific.TCP.listenSocket, &fdset);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	nfound = select((*network)->networkSpecific.TCP.listenSocket + 1,
			&fdset, NULL, NULL, &timeout);
	if (nfound != 0) {
	    if (FD_ISSET((*network)->networkSpecific.TCP.listenSocket, &fdset))
		connected++;
	}
	if (!connected)
	    return COND_PushCondition(DUL_NOASSOCIATIONREQUEST,
				      DUL_Message(DUL_NOASSOCIATIONREQUEST));
    } else {
	connected = 0;
	do {
	    FD_ZERO(&fdset);
	    FD_SET((*network)->networkSpecific.TCP.listenSocket, &fdset);

	    timeout.tv_sec = 5;
	    timeout.tv_usec = 0;
	    nfound = select((*network)->networkSpecific.TCP.listenSocket + 1,
			    &fdset, NULL, NULL, &timeout);
	    if (nfound != 0) {
		if (FD_ISSET((*network)->networkSpecific.TCP.listenSocket,
			     &fdset))
		    connected++;
	    }
	} while (!connected);
    }

    len = sizeof(from);
    *s = -1;
    do {
	if (debug)
	    fprintf(stdout, "\n\n\n*************BEFORE ACCEPT*************\n");
#ifdef CTN_USE_THREADS
	THR_ObtainMutex(FAC_DUL);
#endif
	*s = accept((*network)->networkSpecific.TCP.listenSocket, &from, &len);
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_DUL);
#endif
	if (debug)
	    fprintf(stdout, "*************AFTER ACCEPT(sock: %d/errno: %d)*************\n\n\n", *s, errno);
    } while (*s == -1 && errno == EINTR);

    if (*s < 0) {
	(void) COND_PushCondition(DUL_TCPINITERROR,
				  "Accept Error In %s, socket %d",
				  "receiveTransportConnectionTCP", *s);
	return COND_PushCondition(DUL_TCPINITERROR, strerror(errno));
    }
    sockarg.l_onoff = 0;
    if (setsockopt(*s, SOL_SOCKET, SO_LINGER, (char *) &sockarg,
		   sizeof(sockarg)) < 0) {
	(void) COND_PushCondition(DUL_TCPINITERROR, "In %s, socket %d",
				  "receiveTransportConnectionTCP", *s);
	return COND_PushCondition(DUL_TCPINITERROR, strerror(errno));
    }
    reuse = 1;
    if (setsockopt(*s,
	    SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) < 0) {
	return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
    }
    setTCPBufferLength(*s);
#ifdef CTN_USE_THREADS
#ifdef SOLARIS
    remote = gethostbyaddr_r(&from.sa_data[2], 4, 2, &hStorage,
			     hBuf, sizeof(hBuf), &hError);
#else
    remote = gethostbyaddr(&from.sa_data[2], 4, 2);
#endif
#else
    remote = gethostbyaddr(&from.sa_data[2], 4, 2);
#endif
    if (remote == NULL) {
	(void) sprintf(params->callingPresentationAddress, "%-d.%-d.%-d.%-d",
		       ((int) from.sa_data[2]) & 0xff,
		       ((int) from.sa_data[3]) & 0xff,
		       ((int) from.sa_data[4]) & 0xff,
		       ((int) from.sa_data[5]) & 0xff);
    } else {
	char node[128];
	size_t size;

	if ((*network)->options & DUL_FULLDOMAINNAME)
	    strcpy(node, remote->h_name);
	else {
	    if (sscanf(remote->h_name, "%[^.]", node) != 1)
		node[0] = '\0';
	}

	size = sizeof(params->callingPresentationAddress);
	(void) strncpy(params->callingPresentationAddress, node, size);
	params->callingPresentationAddress[size - 1] = '\0';
    }

    /*(*association)->networkSpecific.TCP.socket = sock;*/

    return DUL_NORMAL;
}


/* createNetworkKey
**
** Purpose:
**	Create a network key for the network environment.
**
** Parameter Dictionary:
**	networkType	Type of transport level network (e.g. TCP)
**	mode		Role played by the application entity
**	timeout		The timeout value to be used for timers
**	key		Pointer to the structure maintaining the
**			entire information of the network environment.
**			This is returned back to the caller.
**
** Return Values:
**
**	DUL_KEYCREATEFAILURE
**	DUL_NORMAL
**	DUL_UNRECOGNIZEDAE
**	DUL_UNSUPPORTEDNETWORK
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
createNetworkKey(char *networkType, char *mode,
		 int timeout, unsigned long opt, PRIVATE_NETWORKKEY ** key)
{
    if (strcmp(networkType, DUL_NETWORK_TCP) == 0) {
    } else {
	return COND_PushCondition(DUL_UNSUPPORTEDNETWORK,
			  DUL_Message(DUL_UNSUPPORTEDNETWORK), networkType);
    }

    if (strcmp(mode, AE_REQUESTOR) != 0 &&
	strcmp(mode, AE_ACCEPTOR) != 0 &&
	strcmp(mode, AE_BOTH) != 0) {
	return COND_PushCondition(DUL_UNRECOGNIZEDAE,
				  DUL_Message(DUL_UNRECOGNIZEDAE), mode);
    }
    *key = (DUL_NETWORKKEY *) CTN_MALLOC(sizeof(PRIVATE_NETWORKKEY));
    if (*key == NULL) {
	return COND_PushCondition(DUL_KEYCREATEFAILURE,
				  DUL_Message(DUL_KEYCREATEFAILURE));
    }
    (void) strcpy((*key)->keyType, KEY_NETWORK);

    (void) strcpy((*key)->networkType, networkType);
    (*key)->applicationFunction = 0;

    if (strcmp(mode, AE_REQUESTOR) == 0)
	(*key)->applicationFunction |= PRV_APPLICATION_REQUESTOR;
    else if (strcmp(mode, AE_ACCEPTOR) == 0)
	(*key)->applicationFunction |= PRV_APPLICATION_ACCEPTOR;
    else
	(*key)->applicationFunction |=
	    PRV_APPLICATION_ACCEPTOR | PRV_APPLICATION_REQUESTOR;

    (*key)->networkState = NETWORK_DISCONNECTED;
    (*key)->protocolState = STATE1;

    if (timeout > 0)
	(*key)->timeout = timeout;
    else
	(*key)->timeout = DEFAULT_TIMEOUT;

    (*key)->options = opt;

    return DUL_NORMAL;
}


/* initializeNetworkTCP
**
** Purpose:
**	Create a socket and listen for connections on the port number
**
** Parameter Dictionary:
**	key		Handle to the network environment
**	parameter	port number on which to listen for connection
**
** Return Values:
**
**	DUL_NORMAL
**	DUL_TCPINITERROR
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
initializeNetworkTCP(PRIVATE_NETWORKKEY ** key, void *parameter)
{
    struct linger
        sockarg;
    int
        reuse = 1;

    if ((*key)->applicationFunction & PRV_APPLICATION_ACCEPTOR) {
	int length;
	struct sockaddr_in server;

/* Create socket for internet type communication */
	(*key)->networkSpecific.TCP.port = *(int *) parameter;
	(*key)->networkSpecific.TCP.listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ((*key)->networkSpecific.TCP.listenSocket < 0) {
	    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
	}
	reuse = 1;
	if (setsockopt((*key)->networkSpecific.TCP.listenSocket,
	    SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) < 0) {
	    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
	}
/* Name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = (u_short) htons((*key)->networkSpecific.TCP.port);
	if (bind((*key)->networkSpecific.TCP.listenSocket,
		 (struct sockaddr *) & server, sizeof(server))) {
	    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
	}
/* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname((*key)->networkSpecific.TCP.listenSocket,
			(struct sockaddr *) & server, &length)) {
	    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
	}
	sockarg.l_onoff = 0;
	if (setsockopt((*key)->networkSpecific.TCP.listenSocket,
	   SOL_SOCKET, SO_LINGER, (char *) &sockarg, sizeof(sockarg)) < 0) {
	    return COND_PushCondition(DUL_TCPINITERROR,
			    DUL_Message(DUL_TCPINITERROR), strerror(errno));
	}
	if (debug)
	    fprintf(stdout, "\n\n\n***BEFORE LISTEN***\n");
	listen((*key)->networkSpecific.TCP.listenSocket, PRV_LISTENBACKLOG);
	if (debug)
	    fprintf(stdout, "***AFTER LISTEN***\n\n\n");
    }
    return DUL_NORMAL;
}



/* createAssociationKey
**
** Purpose:
**	Create handle to the Association.
**
** Parameter Dictionary:
**	networkKey		Handle to the network environment
**	remoteNode		The remote node to whcih the association
**				is to be set up.
**	maxPDU			Size of the maximum PDU.
**	associationKey		The handle to the Association that is to be
**				returned.
**
** Return Values:
**
**	DUL_KEYCREATEFAILURE
**	DUL_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
createAssociationKey(PRIVATE_NETWORKKEY ** networkKey,
		     char *remoteNode, unsigned long maxPDU,
		     PRIVATE_ASSOCIATIONKEY ** associationKey)
{
    PRIVATE_ASSOCIATIONKEY
    * key;


    key = (PRIVATE_ASSOCIATIONKEY *) CTN_MALLOC(
			     sizeof(PRIVATE_ASSOCIATIONKEY) + maxPDU + 100);
    if (key == NULL) {
	return COND_PushCondition(DUL_KEYCREATEFAILURE,
				  DUL_Message(DUL_KEYCREATEFAILURE));
    }
    key->receivePDUQueue = NULL;

    (void) strcpy(key->keyType, KEY_ASSOCIATION);
    (void) strcpy(key->networkType, (*networkKey)->networkType);
    key->applicationFunction = (*networkKey)->applicationFunction;
/*    (void) strcpy(key->applicationType, (*networkKey)->applicationType); */

    (void) strcpy(key->remoteNode, remoteNode);
    key->presentationContextID = 0;
    key->timeout = (*networkKey)->timeout;
    key->timerStart = 0;
    key->maxPDVInput = maxPDU;
    key->fragmentBufferLength = maxPDU + 100;
    key->fragmentBuffer = (unsigned char *) key + sizeof(*key);

    key->pdvList.count = 0;
    key->pdvList.scratch = key->fragmentBuffer;
    key->pdvList.scratchLength = maxPDU;
    key->pdvList.abort.result = 0x00;
    key->pdvList.abort.source = 0x00;
    key->pdvList.abort.reason = 0x00;
    key->pdvList.pdv = NULL;

    key->isExternalSocket = 0;
    key->inputPDU = NO_PDU;
    key->nextPDUType = 0x00;
    key->nextPDUReserved = 0;
    key->nextPDULength = 0;

    key->pdvCount = 0;
    key->pdvIndex = -1;
    key->pdvPointer = NULL;
    (void) memset(&key->currentPDV, 0, sizeof(key->currentPDV));

    key->logHandle = NULL;

    *associationKey = key;
    return DUL_NORMAL;
}



/* destroyAssociationKey
**
** Purpose:
**	Destroy the handle to the Association.
**
** Parameter Dictionary:
**	key		Handle to the association.
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
destroyAssociationKey(PRIVATE_ASSOCIATIONKEY ** key)
{
#ifdef BLOG
    if (blog)
	BLG_Close(&(*key)->logHandle);
#endif
    CTN_FREE(*key);
    *key = NULL;
}


/* get_association_parameter
**
** Purpose:
**	Get a single parameter.
**
** Parameter Dictionary:
**	paramAddress		Source parameter
**	paramType		Type of the source parameter
**	paramLength		Size of the source parameter
**	outputType		Type of the destination parameter
**	outputAddress		Destination parameter returned to caller
**	outputLength		Size of the destination parameter
**
** Return Values:
**
**	DUL_INCORRECTBUFFERLENGTH
**	DUL_INSUFFICIENTBUFFERLENGTH
**	DUL_NORMAL
**	DUL_WRONGDATATYPE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
get_association_parameter(void *paramAddress,
			  DUL_DATA_TYPE paramType, size_t paramLength,
	 DUL_DATA_TYPE outputType, void *outputAddress, size_t outputLength)
{
    if (paramType != outputType)
	return COND_PushCondition(DUL_WRONGDATATYPE,
				  DUL_Message(DUL_WRONGDATATYPE));
    if ((paramType == DUL_K_INTEGER) && (outputLength != paramLength))
	return COND_PushCondition(DUL_INCORRECTBUFFERLENGTH,
				  DUL_Message(DUL_INCORRECTBUFFERLENGTH));
    if ((paramType == DUL_K_STRING) && (outputLength < strlen(paramAddress)))
	return COND_PushCondition(DUL_INSUFFICIENTBUFFERLENGTH,
				  DUL_Message(DUL_INSUFFICIENTBUFFERLENGTH));

    switch (paramType) {
    case DUL_K_INTEGER:
	(void) memcpy(outputAddress, paramAddress, paramLength);
	break;
    case DUL_K_STRING:
	strcpy(outputAddress, paramAddress);
	break;
    }
    return DUL_NORMAL;
}


/* setTCPBufferLength
**
** Purpose:
**	Initialize the length of the buffer.
**
** Parameter Dictionary:
**	sock		Socket descriptor.
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
setTCPBufferLength(int sock)
{
    char
       *TCPBufferLength;
    int
        bufLen;

    if ((TCPBufferLength = getenv("TCP_BUFFER_LENGTH")) != NULL) {
	if (sscanf(TCPBufferLength, "%d", &bufLen) == 1) {
	    (void) setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *) &bufLen,
			      sizeof(bufLen));
	    (void) setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *) &bufLen,
			      sizeof(bufLen));
	}
    }
}

/* DUL_DumpParams
**
** Purpose:
**	Display information of various fields of the service parameters.
**
** Parameter Dictionary:
**	params		Pointer to structure holding the service parameters.
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
DUL_DumpParams(DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    printf("APP CTX NAME:%s\n", params->applicationContextName);
    dump_uid(params->applicationContextName, "%13s");
    printf("AP TITLE:     %s\n", params->callingAPTitle);
    printf("AP TITLE:     %s\n", params->calledAPTitle);
    printf("AP TITLE:     %s\n", params->respondingAPTitle);
    printf("MAX PDU:      %d\n", params->maxPDU);
    printf("Peer MAX PDU: %d\n", params->peerMaxPDU);
    printf("PRES ADDR:    %s\n", params->callingPresentationAddress);
    printf("PRES ADDR:    %s\n", params->calledPresentationAddress);
    printf("REQ IMP UID:  %s\n", params->callingImplementationClassUID);
    dump_uid(params->callingImplementationClassUID, "%13s");
    printf("REQ VERSION:  %s\n", params->callingImplementationVersionName);
    printf("ACC IMP UID:  %s\n", params->calledImplementationClassUID);
    dump_uid(params->calledImplementationClassUID, "%13s");
    printf("ACC VERSION:  %s\n", params->calledImplementationVersionName);
    printf("Requested Presentation Ctx\n");
    dump_presentation_ctx(&params->requestedPresentationContext);
    printf("Accepted Presentation Ctx\n");
    dump_presentation_ctx(&params->acceptedPresentationContext);
}

typedef struct {
    DUL_SC_ROLE role;
    char *text;
}   SC_MAP;
static SC_MAP scMap[] = {
    {DUL_SC_ROLE_DEFAULT, "Default"},
    {DUL_SC_ROLE_SCU, "SCU"},
    {DUL_SC_ROLE_SCP, "SCP"},
    {DUL_SC_ROLE_SCUSCP, "SCUSCP"},
};

/* dump_presentation_ctx
**
** Purpose:
**	Display the contents of the presentation context list
**
** Parameter Dictionary:
**	l	Head of the list of various presentation conmtexts.
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dump_presentation_ctx(LST_HEAD ** l)
{
    DUL_PRESENTATIONCONTEXT
    * ctx;
    DUL_TRANSFERSYNTAX
	* transfer;
    int
        index;

    if (*l == NULL)
	return;

    ctx = LST_Head(l);
    if (ctx == NULL)
	return;

    (void) LST_Position(l, ctx);

    while (ctx != NULL) {
	printf("  Context ID:           %d\n", ctx->presentationContextID);
	printf("  Abstract Syntax:      %s\n", ctx->abstractSyntax);
	dump_uid(ctx->abstractSyntax, "%24s");
	printf("  Result field:         %d\n", (int) ctx->result);
	for (index = 0; index < (int) DIM_OF(scMap); index++) {
	    if (ctx->proposedSCRole == scMap[index].role)
		printf("  Proposed SCU/SCP Role:  %s\n", scMap[index].text);
	}
	for (index = 0; index < (int) DIM_OF(scMap); index++) {
	    if (ctx->acceptedSCRole == scMap[index].role)
		printf("  Accepted SCU/SCP Role:  %s\n", scMap[index].text);
	}
	printf("  Proposed Xfer Syntax(es)\n");
	if (ctx->proposedTransferSyntax != NULL) {
	    transfer = LST_Head(&ctx->proposedTransferSyntax);
	    if (transfer != NULL)
		(void) LST_Position(&ctx->proposedTransferSyntax, transfer);

	    while (transfer != NULL) {
		printf("                  %s\n", transfer->transferSyntax);
		dump_uid(transfer->transferSyntax, "%18s");
		transfer = LST_Next(&ctx->proposedTransferSyntax);
	    }
	}
	printf("  Accepted Xfer Syntax: %s\n", ctx->acceptedTransferSyntax);
	dump_uid(ctx->acceptedTransferSyntax, "%24s");
	ctx = LST_Next(l);
    }

}

/* dump_uid
**
** Purpose:
**	Display the UID structure
**
** Parameter Dictionary:
**	UID		The UID associated with the structure
**	indent		Useful for printing purposes.
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dump_uid(char *UID, char *indent)
{
    CONDITION
    cond;
    UID_DESCRIPTION
	d;

    if (UID[0] == '\0') {
	printf(indent, " ");
	printf("No UID\n");
    } else {
	cond = UID_Lookup(UID, &d);
	if (cond == UID_NORMAL) {
	    printf(indent, " ");
	    printf("%s, %s\n", d.description, d.originator);
	} else {
	    (void) COND_PopCondition(FALSE);
	    printf(indent, " ");
	    printf("Unknown UID\n");
	}
    }
}
/* checkNetwork
**
** Purpose:
**	Verify the validity of the network handle.
**
** Parameter Dictionary:
**	networkKey		Handle to the network to be validated.
**	caller			Used only for better error messages, contains
**				the string indicating the name of the
**				routine that called this one.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
checkNetwork(PRIVATE_NETWORKKEY ** networkKey, char *caller)
{
    if (networkKey == NULL)
	return COND_PushCondition(DUL_NULLKEY,
				  DUL_Message(DUL_NULLKEY), caller);
    if (*networkKey == NULL)
	return COND_PushCondition(DUL_NULLKEY,
				  DUL_Message(DUL_NULLKEY), caller);
    if (strcmp((*networkKey)->keyType, KEY_NETWORK) != 0)
	return COND_PushCondition(DUL_ILLEGALKEY,
				  DUL_Message(DUL_ILLEGALKEY), caller);

    return DUL_NORMAL;
}


/* checkAssociation
**
** Purpose:
**	Verify the validity of the Association handle
**
** Parameter Dictionary:
**	association	Handle to the association to be validated.
**	caller		Name of routine that called this one.
**
** Return Values:
**
**	DUL_ILLEGALKEY
**	DUL_NORMAL
**	DUL_NULLKEY
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
checkAssociation(PRIVATE_ASSOCIATIONKEY ** association,
		 char *caller)
{
    if (association == NULL)
	return COND_PushCondition(DUL_NULLKEY,
				  DUL_Message(DUL_NULLKEY), caller);
    if (*association == NULL)
	return COND_PushCondition(DUL_NULLKEY,
				  DUL_Message(DUL_NULLKEY), caller);
    if (strcmp((*association)->keyType, KEY_ASSOCIATION) != 0)
	return COND_PushCondition(DUL_ILLEGALKEY,
				  DUL_Message(DUL_ILLEGALKEY), caller);

    return DUL_NORMAL;
}


/* clearRequestorsParams
**
** Purpose:
**	Reset all the fields of the service parameters.
**
** Parameter Dictionary:
**	params		Pointer to the service parameters to be reset.
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
clearRequestorsParams(DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    params->applicationContextName[0] = '\0';
    params->callingAPTitle[0] = '\0';
    params->calledAPTitle[0] = '\0';
    params->respondingAPTitle[0] = '\0';
    params->result = params->resultSource = params->diagnostic = 0;
    params->callingPresentationAddress[0] = '\0';
    params->calledPresentationAddress[0] = '\0';
    params->requestedPresentationContext = NULL;
    params->acceptedPresentationContext = NULL;
    params->maximumOperationsInvoked = 0;
    params->maximumOperationsPerformed = 0;
    params->callingImplementationClassUID[0] = '\0';
    params->callingImplementationVersionName[0] = '\0';
}

/* clearPresentationContext
**
** Purpose:
**	Free the memory oocupied by the given presentation context.
**
** Parameter Dictionary:
**	l	Head of list of the presentation contexts to be freed.
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
clearPresentationContext(LST_HEAD ** l)
{
    DUL_PRESENTATIONCONTEXT
    * ctx;
    DUL_TRANSFERSYNTAX
	* transfer;

    if (*l == NULL)
	return;

    while ((ctx = LST_Pop(l)) != NULL) {
	if (ctx->proposedTransferSyntax != NULL) {
	    while ((transfer = LST_Pop(&ctx->proposedTransferSyntax)) != NULL)
		CTN_FREE(transfer);
	    (void) LST_Destroy(&ctx->proposedTransferSyntax);
	}
	CTN_FREE(ctx);
    }
    (void) LST_Destroy(l);
}
