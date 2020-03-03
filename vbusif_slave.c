
/*==============================================================================
* Copyright (C) 2015-2020 
* File name: vbusif_slave.c
* Abstract : interface of the virtual bus protocol 
* History  :
*-------------------------------------------------------------------------------
* Author        Date        Modification  Version  Description
**============================================================================*/
#include "vbusim.h"
#include "list_queue.h"

#define VBTP_TOTAL	2//	3
#define PROTOCOL_VERSN	0xe0

#define SLAVE_REG_DEVH	0x00
#define SLAVE_REG_DEVL	0x01
#define SLAVE_REG_VERS	0x02
#define SLAVE_REG_STAT	0x03
#define SLAVE_REG_FIFO	0x04

#define SIZE_OF_FIFO	VBLPDU_MAXLEN
#define SIZE_OF_BUFF	(SLAVE_REG_FIFO + SIZE_OF_FIFO)

#define V_BS_STATUS			0x00
#define V_WO_STATUS			(0x40 | SIZE_OF_FIFO)
#define V_RO_STATUS			(0x80 | SlaveFifoTxSize)

static ListQueue_t	 gLqHeap;
static UI08 volatile SlaveBuffer[SIZE_OF_BUFF];
static UI08 volatile SlaveFifoTxSize = 0, SlaveFifoRxSize = 0;
static VOID	*VtlBusGetVbTp(UINT Index)PARAM_PAST_IN_STACK
{
	static VbTp_t	VbTpArray[VBTP_TOTAL];
	
	return &VbTpArray[Index];
}

VbTp_t *VtlBusAllocate(VOID)PARAM_PAST_IN_STACK
{
	return (VbTp_t *)RemoveNode(&gLqHeap,NULL);
}

VOID	VtlBusRelease(VbTp_t *pVbTp)PARAM_PAST_IN_STACK
{
	AppendNode(&gLqHeap,pVbTp);
}

BOOL	VtlBusIsSlvMode(UI08 Bus)PARAM_PAST_IN_STACK
{
	return	TRUE;
}

BOOL VtlBusGetStatus(BOOL *pReadable,UI08 Bus)PARAM_PAST_IN_STACK
{
	BOOL	Usable;
	
	if(SlaveFifoRxSize){
		Usable = TRUE;
		*pReadable = TRUE;
	} else if(SlaveFifoTxSize) {
		Usable = FALSE;
	} else {
		Usable = TRUE;
		*pReadable = FALSE;
	}
	
	return	Usable;
}

UI16	VtlBusReadLpdu(VOID *pBuffer,UI16 Size,UI08 Bus)PARAM_PAST_IN_STACK
{
	UI16	Length;
	
	Length = Size > SlaveFifoRxSize ? SlaveFifoRxSize : Size;
	memcpy(pBuffer,(VOID const *)&SlaveBuffer[SLAVE_REG_FIFO],Length);
	SlaveFifoRxSize -= Length;
	
	return	Length;
}

VOID	VtlBusWriteLpdu(UI08 Bus,UI16 Size,VOID const *pBuffer)PARAM_PAST_IN_STACK
{
	if(Size && pBuffer){
		UI16	Length;
		
		Length = Size > SIZE_OF_FIFO ? SIZE_OF_FIFO : Size;
		memcpy((VOID *)&SlaveBuffer[SLAVE_REG_FIFO],pBuffer,Length);
		SlaveFifoTxSize = Length;
		SlaveBuffer[SLAVE_REG_STAT] = V_RO_STATUS;
	} else {
		SlaveBuffer[SLAVE_REG_STAT] = V_WO_STATUS;
	}
}

void I2C0_IRQHandler(void)
{
	if(I2C_GET_TIMEOUT_FLAG(I2C0)){
	    I2C_ClearTimeoutFlag(I2C0);/* Clear I2C0 Timeout Flag */
	}	
	else
	{
		UI08	Dumy;
		UI32	Status;
		static UI08	SetReg,SlaveReg;
		static UI08	FifoTxLen,FifoRxLen;
		
		Status = I2C_GET_STATUS(I2C0);

		switch(Status)
		{
			case 0x80:/* Data has been received; ACK has been returned */
				if(SetReg)
				{
					SetReg = 0;FifoRxLen = 0;
					SlaveReg = (UI08)I2C_GET_DATA(I2C0);
				}
				else if((SlaveReg == SLAVE_REG_FIFO) && (FifoRxLen < SIZE_OF_FIFO))
				{
					SlaveBuffer[SLAVE_REG_FIFO + FifoRxLen++] = (UI08)I2C_GET_DATA(I2C0);
				}
				else
				{
					Dumy = (UI08)I2C_GET_DATA(I2C0);
				}
				I2C_SET_CONTROL_REG(I2C0,I2C_CTL_SI_AA);
				break;
				
			case 0xa8:/* Own SLA+R has been receive; ACK has been return */
				FifoTxLen = 0;
			case 0xb8:/* Data has been transmitted and ACK has been received */
				if(SlaveReg > SLAVE_REG_FIFO)
				{
					I2C_SET_DATA(I2C0,0);
				}
				else if(SlaveReg < SLAVE_REG_FIFO)
				{
					I2C_SET_DATA(I2C0,SlaveBuffer[SlaveReg++]);
				}
				else if(FifoTxLen < SlaveFifoTxSize)
				{
					I2C_SET_DATA(I2C0,SlaveBuffer[SLAVE_REG_FIFO + FifoTxLen++]);
				}
				else
				{
					I2C_SET_DATA(I2C0,0);
				}
				I2C_SET_CONTROL_REG(I2C0,I2C_CTL_SI_AA);
				break;
				
			case 0x60:/* Own SLA+W has been receive; ACK has been return */
				SetReg = 1;
				I2C_SET_CONTROL_REG(I2C0,I2C_CTL_SI_AA);
				break;
				
			case 0xc0:/* Last data has been transmitted; Not ACK has been received */
				I2C_SET_CONTROL_REG(I2C0,I2C_CTL_SI_AA);
				break;
				
			case 0xa0:/* A STOP or repeated START has been received while still addressed as Slave/Receiver */
				if(FifoRxLen)
				{
					SlaveBuffer[SLAVE_REG_STAT] = V_BS_STATUS;
					SlaveFifoRxSize = FifoRxLen;
				}
				else if(FifoTxLen)
				{
					SlaveBuffer[SLAVE_REG_STAT] = V_BS_STATUS;
					SlaveFifoTxSize = 0;
				}
				I2C_SET_CONTROL_REG(I2C0,I2C_CTL_SI_AA);
			default:
				break;
		}
	}
}
