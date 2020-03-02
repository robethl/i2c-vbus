/*==============================================================================
* Copyright (C) 2015-2020 
* File name: list_queue.c
* Abstract : implementation of list_queue management
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#include "type.h"
#include "list_queue.h"

#ifdef REALESE
#define LogPrint(x, ...) 
#else
#define LogPrint printf 
#endif /*REALESE*/

#define LIST_SET_NEXT(addr, val) *(UI32*)addr = (UI32)val
#define LIST_GET_NEXT(addr)      ((VOID*)*(UI32*)addr)

VOID InitLqHeap(pListQueue_t pLq, GetObj_fp fGetObj, UINT nodesMax)PARAM_PAST_IN_STACK
{
    pLq->pHead = pLq->pTail = NULL;
    for(UINT i = 0; i < nodesMax; ++i){
        VOID* ptr = fGetObj(i);
        LogPrint("ptr=%p\n", ptr);
        if(NULL == pLq->pHead){
            pLq->pHead = ptr;
            pLq->pTail = ptr;
        } else{
            LIST_SET_NEXT(pLq->pTail, ptr);
            pLq->pTail = ptr;
        }
    }
    LogPrint("head=%p tail=%p\n", pLq->pHead, pLq->pTail);
}

VOID *RemoveNode(pListQueue_t pLq, VOID *pNode)PARAM_PAST_IN_STACK
{
    VOID *ptr = NULL, *ptr_n = NULL;
    if(NULL == pLq || NULL == pLq->pHead)return NULL;

    /*移除pNode, 若pNode==NULL,则移除头结点*/
    if(NULL == pNode || pLq->pHead == pNode){
        ptr = pLq->pHead;
        pLq->pHead = LIST_GET_NEXT(ptr);
        LIST_SET_NEXT(ptr, NULL);
        if(NULL == pLq->pHead) pLq->pTail = NULL;
        return ptr;
    }

    ptr = pLq->pHead;
    while(ptr){
        ptr_n = LIST_GET_NEXT(ptr);
        if( ptr_n == pNode){
            if(pNode == pLq->pTail)pLq->pTail = ptr;
            LIST_SET_NEXT(ptr, LIST_GET_NEXT(ptr_n));
            LIST_SET_NEXT(ptr_n, NULL);
            ptr = ptr_n;
            break;
        }
        ptr = ptr_n;
    }
    return ptr;
}

VOID AppendNode(pListQueue_t pLq, VOID *pNode)PARAM_PAST_IN_STACK
{
    if(NULL == pLq || NULL == pNode)return;

    if(NULL == pLq->pHead){
        pLq->pHead = pNode;
        pLq->pTail = pNode;
    }else{
        LIST_SET_NEXT(pLq->pTail, pNode);
        pLq->pTail = pNode;
    }
}

VOID *LookupNode(pListQueue_t const pLq, CmpObj_fp fCmpObj, VOID const *pObj)PARAM_PAST_IN_STACK
{
    VOID *ptr = NULL;
    if(NULL == pLq)return NULL;

    ptr = pLq->pHead;
    while(ptr != NULL){
        if(fCmpObj(pObj, ptr) == 0){
            break;
        }
        ptr = LIST_GET_NEXT(ptr);
    }
    return ptr;
}