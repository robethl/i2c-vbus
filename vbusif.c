/*==============================================================================
* Copyright (C) 2015-2020 
* File name: vbusif.c
* Abstract : interface of the virtual bus protocol 
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#include "vbusim.h"
#include "list_queue.h"

#define VBTP_TOTAL (64) /*vbus 协议包上限*/
#define VBCB_TOTAL (3) /*vbus链路数*/

#define IsReadable(stat)     (stat & (1 << 7)) ? TRUE : FALSE
#define IsWriteable(stat)    (stat & (1 << 6)) ? TRUE : FALSE
#define GetReadByteLen(stat) (stat & 0x3F)

/****************************************************************************/
//								Variable defined
/****************************************************************************/
static ListQueue_t gLqHeap;
static VbCb_t gVbCb[VBCB_TOTAL];

/****************************************************************************/
//								function defined
/****************************************************************************/
/*
*vltbus堆内存管理协议包指针
*/
static VOID* VbusGetVbTp(UINT index)PARAM_PAST_IN_STACK
{
	static VbTp_t VbTpArray[VBTP_TOTAL];
	return &VbTpArray[index];
}

VOID VbusEnterCriticalSection(VOID)PARAM_PAST_IN_STACK
{
	return;
}
VOID VbusLeaveCriticalSection(VOID)PARAM_PAST_IN_STACK
{
	return;
}

/*
*vltbus堆内存协议包空间 分配
*/
pVbTp_t VbusAllocateVbTp(VOID)PARAM_PAST_IN_STACK
{
	pVbTp_t pVbTp;
	VbusEnterCriticalSection();
	pVbTp = RemoveNode(&gLqHeap, NULL);
	VbusLeaveCriticalSection();
	return pVbTp;
}

/*
*vltbus堆内存协议包 回收
*/
VOID VbusReleaseVbTp(pVbTp_t pVbTp)PARAM_PAST_IN_STACK
{
	VbusEnterCriticalSection();
	AppendNode(&gLqHeap, pVbTp);
	VbusLeaveCriticalSection();
}
/*
*vltbus i2c总线模式: master or slave
*/
BOOL VbusIsSlvMode(UI08 bus)PARAM_PAST_IN_STACK
{
	return FALSE;
}

/*
*vbus i2c总线状态.
* master模式 根据从机状态可返回三种状态： 可用(可写、可读)、不可用
* 可用时返回TRUE； 不可用返回FASLE；
*/
BOOL VbusGetStatus(UI08 bus, BOOL *pReadable)PARAM_PAST_IN_STACK
{
	*pReadable = FALSE;
	return TRUE;
}

/*
*vbus i2c接收
*/
UI16 VbusReadLpdu(UI08 bus, VOID *pBuffer, UI16 maxSize)PARAM_PAST_IN_STACK
{
	return 0;
}

/*
*vbus i2c发送
*/
VOID VbusWriteLpdu(UI08 bus, VOID const *pBuffer, UI16 size)PARAM_PAST_IN_STACK
{
	return;
}

VOID VbusInitialize(VOID)PARAM_PAST_IN_STACK
{
	InitLqHeap(&gLqHeap, VbusGetVbTp, VBTP_TOTAL);
}


/*
*vbus callback 
*非阻塞
*/
static VOID VbusCallback(UI08 bus, pVbTp_t pVbTp)PARAM_PAST_IN_STACK
{ 
	switch(pVbTp->id){
		case TPID_APP:
			break;
		
		case TPID_TFTP:	
			break;
		
		default:
			break;
	} 
}

VOID VbusTask(VOID)PARAM_PAST_IN_STACK
{
	while (TRUE){
		VbusPolling(VbusCallback, VBCB_TOTAL, gVbCb);
	}
}
