/*==============================================================================
* Copyright (C) 2015-2020 
* File name: vbusim.c
* Abstract : implementation of the virtual bus protocol
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "list_queue.h"
#include "vbusif.h"
#include "vbusim.h"

#ifdef REALESE
#define LogPrint(x, ...) 
#else
#define LogPrint printf 
#endif /*REALESE*/

#define IsLastFromHead(head)  ((head) & 0x80)
#define GetTpIdFromHead(head) ((head) & 0x7f)
#define MergeLastFrmHead(id)  ((id) | 0x80)
#define MergeMoreFrmHead(id)  (id)

static SI32	VbusCmpTpdu(UI08 const *pTpId, VbTp_t const *pVbTp)PARAM_PAST_IN_STACK
{
	if((*pTpId) < pVbTp->id) return -1;
	else if((*pTpId) > pVbTp->id) return 1;
	else return 0;
}

static VOID	VbusSendLpdu(UI08 bus, BOOL slave, pListQueue_t pSendQ)PARAM_PAST_IN_STACK
{
	if(pSendQ->pHead != NULL){
		pVbTp_t	pVbTp = (pVbTp_t)pSendQ->pHead;
		if(pVbTp->size){
			UI16 bytes;
			UI08 buffer[VBLPDU_MAXLEN];
			
			if(pVbTp->size < VBLPDU_MAXLEN) {
				bytes = pVbTp->size;
				buffer[0] = MergeLastFrmHead(pVbTp->id);
			} else {
				bytes = VBLPDU_MAXLEN - 1;
				buffer[0] = MergeMoreFrmHead(pVbTp->id);
			}
			memcpy(&buffer[1], &pVbTp->data[pVbTp->head], bytes); pVbTp->head += bytes; pVbTp->size -= bytes;
			VbusWriteLpdu(bus, buffer, bytes + 1);
			
			if(0 == pVbTp->size){
				RemoveNode(pSendQ, pVbTp);
				VbusReleaseVbTp(pVbTp);
			}
		} /*else {
			if(slave)VbusWriteLpdu(bus, NULL, 0);
			RemoveNode(pSendQ, pVbTp);
			VbusReleaseVbTp(pVbTp);
		}*/
	}else if(slave){
		VbusWriteLpdu(bus, NULL, 0);
	}
}

static VOID	VbusRecvLpdu(UI08 bus, pListQueue_t pRecvQ, VbNotify_fp fNotify)PARAM_PAST_IN_STACK
{
	UI16 bytes;
	UI08 buffer[VBLPDU_MAXLEN];
	
	bytes = VbusReadLpdu(bus, buffer, VBLPDU_MAXLEN);
	if(bytes > 1){
		UI08	id;
		pVbTp_t	pVbTp;
		
		--bytes; 
		id = GetTpIdFromHead(buffer[0]);
		pVbTp = (pVbTp_t)LookupNode(pRecvQ, (CmpObj_fp)VbusCmpTpdu, &id);
		if(pVbTp) {
			memcpy(&pVbTp->data[pVbTp->head], &buffer[1], bytes);
			pVbTp->size += bytes;
			if(IsLastFromHead(buffer[0])) {
				pVbTp->head = 0;
				fNotify(bus, pVbTp);
				RemoveNode(pRecvQ, pVbTp);
				VbusReleaseVbTp(pVbTp);
			} else {
				pVbTp->head += bytes;
			}
		} else {
			pVbTp = VbusAllocateVbTp();
			if(pVbTp) {
				pVbTp->size = bytes; pVbTp->id = id;
				memcpy(pVbTp->data, &buffer[1], bytes);
				if(IsLastFromHead(buffer[0])) {
					pVbTp->head = 0;
					fNotify(bus,pVbTp);
					VbusReleaseVbTp(pVbTp);
				} else {
					pVbTp->head = bytes;
					AppendNode(pRecvQ, pVbTp);
				}
			}else{
				LogPrint("VbusAllocateVbTp failed\n");
			}
		}
	}
}

VOID VbusPolling(VbNotify_fp fNotify, UI08 sumOfVbCb, VbCb_t *pVbCbList)PARAM_PAST_IN_STACK
{
	UI08 i;
	
	for(i = 0; i < sumOfVbCb; ++i){
		BOOL Readable;
		
		if(VbusGetStatus(i, &Readable)){
			BOOL slave = VbusIsSlvMode(i);
			if(Readable){
				VbusRecvLpdu(i, &pVbCbList[i].recvQ, fNotify);
				if(slave) VbusSendLpdu(i, slave, &pVbCbList[i].sendQ);
			} else {
				VbusSendLpdu(i, slave, &pVbCbList[i].sendQ);
			}
		}
	}
}
