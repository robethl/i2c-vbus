/*==============================================================================
* Copyright (C) 2015-2020 
* File name: vbusif.h
* Abstract : interface of the virtual bus protocol 
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#ifndef	_VBUS_IF_H_
#define _VBUS_IF_H_

#include "type.h"
#include "vbusim.h"

#ifdef __cplusplus
extern "C" {
#endif

VOID VbusEnterCriticalSection(VOID)PARAM_PAST_IN_STACK;
VOID VbusLeaveCriticalSection(VOID)PARAM_PAST_IN_STACK;

pVbTp_t VbusAllocateVbTp(VOID)PARAM_PAST_IN_STACK;
VOID VbusReleaseVbTp(pVbTp_t pVbTp)PARAM_PAST_IN_STACK;

BOOL VbusIsSlvMode(UI08 bus)PARAM_PAST_IN_STACK;                   /* 总线是否工作在从模式 */
BOOL VbusGetStatus(UI08 bus, BOOL *pReadable)PARAM_PAST_IN_STACK;   /* 总线是否可用 */
UI16 VbusReadLpdu(UI08 bus, VOID *pBuffer, UI16 maxSize)PARAM_PAST_IN_STACK;
VOID VbusWriteLpdu(UI08 bus, VOID const *pBuffer, UI16 size)PARAM_PAST_IN_STACK;

VOID VbusInitialize(VOID)PARAM_PAST_IN_STACK;

#ifdef __cplusplus
}
#endif

#endif
