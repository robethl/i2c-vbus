/*==============================================================================
* Copyright (C) 2015-2020 
* File name: list_queue.h
* Abstract : implementation of list_queue management
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#ifndef _LIST_QUEUE_H_
#define _LIST_QUEUE_H_

#include "type.h"

typedef struct
{
	VOID *pHead;
	VOID *pTail;
}ListQueue_t, *pListQueue_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef VOID *(*GetObj_fp)(UINT index)PARAM_PAST_IN_STACK;
typedef SI32 (*CmpObj_fp)(VOID const *pObj1, VOID const *pObj2)PARAM_PAST_IN_STACK;

VOID InitLqHeap(pListQueue_t pLq, GetObj_fp fGetObj, UINT nodesMax)PARAM_PAST_IN_STACK;
VOID* RemoveNode(pListQueue_t pLq, VOID *pNode)PARAM_PAST_IN_STACK;
VOID AppendNode(pListQueue_t pLq, VOID *pNode)PARAM_PAST_IN_STACK;
VOID* LookupNode(pListQueue_t const pLq, CmpObj_fp fCmpObj, VOID const *pObj)PARAM_PAST_IN_STACK;

#ifdef __cplusplus
}
#endif

#endif
