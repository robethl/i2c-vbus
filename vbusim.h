/*==============================================================================
* Copyright (C) 2015-2020 
* File name: vbusim.h
* Abstract : implementation of the virtual bus protocol
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#ifndef	_VBUS_IM_H_
#define _VBUS_IM_H_

#include "type.h"
#include "list_queue.h"

/*Identifier of vbus transport protocol*/
#define TPID_APP  (0)
#define TPID_TFTP (1)

#define VBLPDU_MAXLEN (14)
#define VBTP_MAXLEN  (515)

typedef struct
{
	VOID *pNext;
    UI08 id;
	UI16 head;
	UI16 size;
	UI08 data[VBTP_MAXLEN];
}VbTp_t, *pVbTp_t;

typedef struct
{
	ListQueue_t	sendQ;
	ListQueue_t	recvQ;
}VbCb_t, *pVbCb_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef VOID (*VbNotify_fp)(UI08 Bus,VbTp_t *pVbTp)PARAM_PAST_IN_STACK;
VOID VbusPolling(VbNotify_fp fNotify, UI08 sumOfVbCb, VbCb_t *pVbCbList)PARAM_PAST_IN_STACK;

#ifdef __cplusplus
}
#endif

#endif
