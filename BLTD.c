/*
 * BLTD.c
 *
 * Created: 31/07/2015 12:35:22 ص
 *  Author: hossam
 */ 
#include "Basic_Types.h"
#include "UART_Drv.h"
#include "BLTD.h"

static u8 IsRespRecieved = 0;
static u8 RxBuffer[100];
static void BTCommandSend(const u8* const Command,u16 CommandLength);
static  void MemCpy(u8 *Des,const u8 *Src,u16 Length);
static void RxcCallBackFun(void);
static u8 MemCmp(const u8 * Src1,const u8 *Src2,u16 CmpLength);
/***************************************************************************************************************/
void BLTD_SendInitCmd(void)
{	IsRespRecieved = 0u;
	UART_StartReception(RxBuffer,4u,&RxcCallBackFun);
	BTCommandSend((u8*)"+INIT",5u);
}
/***************************************************************************************************************/
void BLTD_SendInquireCmd(void)
{	IsRespRecieved = 0u;
	UART_StartReception(RxBuffer,4u,&RxcCallBackFun);
	BTCommandSend((u8*)"+INQ",4u);
}
u8 BLTD_CheckForResponse( u8 const Response[],u16 RespLength)
{
	u8 RespStatus;
	u8 IsEqual;
	if(IsRespRecieved == 1u)
	{
		IsRespRecieved = 0u;
		IsEqual = MemCmp(Response,RxBuffer,RespLength);
		if(IsEqual == 0u)
		{
			RespStatus = BLTD_RESP_STATUS_OK;
			
		}
		else
		{
			RespStatus = BLTD_RESP_STATUS_NOK;
		}
		
	}
	else
	{

		RespStatus = BLTD_RESP_STATUS_NON;
	}
	return RespStatus;
}	
/***************************************************************************************************************/
void BLTD_StartWaitPairing(void)
{
	UART_StopRception();
	UART_StartReception(RxBuffer,4u,&RxcCallBackFun);
	/*BTCommandSend(0,0);*/
	
}
/***************************************************************************************************************/	
void BLTD_SendMessage(const u8* const Message,u16 MsgLength)
{
	UART_TxBuffer(Message,MsgLength);
}	
/***************************************************************************************************************/
u8 BLTD_GetRecievedData(u8 Data[], u16 Length)
{
	u8 RespStatus1;
	u8 i;
	if(IsRespRecieved == 1u)
	{
		IsRespRecieved = 0u;
		RespStatus1 = BLTD_RESP_STATUS_OK;
		for( i = 0u; i< Length ; i++)
		{
			Data[i] = RxBuffer[i];
		}
	}
	else
	{
		RespStatus1 = BLTD_RESP_STATUS_NON;
	}		
		
	return RespStatus1;
}
/***************************************************************************************************************/
void BLTD_StartReceivingData(u8* DataBuffer,u16 BufferLength,CbkPfnType CbkFnPtr)
{
	UART_StartReception(DataBuffer,BufferLength,CbkFnPtr);
	
}
/***************************************************************************************************************/
u8 BLTD_CheckForData(u8* Data)
{
	u16 RxBytesNum;
	u8 IsReceived;
	RxBytesNum = UART_GetNumOfRxbytes();
	if(RxBytesNum > 0u)
	{
		IsReceived = 0x01u;
		*Data = RxBuffer[RxBytesNum+1u];
		UART_StopRception();
		UART_StartReception(RxBuffer,100u,&RxcCallBackFun);
	}
	else
	{
		IsReceived = 0x00u;
		*Data  = 0u;
	}
	return IsReceived;
}

/***************************************************************************************************************/	
void BLTD_SenTestCmd(void)
{
	UART_StartReception(RxBuffer,4u,&RxcCallBackFun);
	BTCommandSend((u8*)0,0u);
	
}
/***************************************************************************************************************/
static void BTCommandSend(const u8* const Command,u16 CommandLength)
	{

    static char BTCommandBuffer[100];
		BTCommandBuffer[0u] = 'A';
		BTCommandBuffer[1u] = 'T';
		MemCpy(&BTCommandBuffer[2u],Command,CommandLength);
		BTCommandBuffer[CommandLength+2u] =(char) 0x0du;
		BTCommandBuffer[CommandLength+3u] =(char) 0x0au;
		UART_TxBuffer(BTCommandBuffer,CommandLength + 4u);
		
	}
/***************************************************************************************************************/
static  void MemCpy(u8 Des[],const u8 Src[],u16 Length)
	{
	u16 j;
	for(j = 0u ; j<Length ; j++)
		{
		Des[j] = Src[j];
		}
	}
/***************************************************************************************************************/	
static void RxcCallBackFun(void)
{

	IsRespRecieved = 1u;
}
/***************************************************************************************************************/
static u8 MemCmp(const u8 Src1[],const u8 Src2[],u16 CmpLength)
	{
	u8 RetVal = 0;
	u16 x;
	for(x = 0u ;(x < CmpLength); x++)
		{
		if(Src1[x] != Src2[x])
			{
			RetVal = 1u;
			break;
			}
		}
	return RetVal;
	}
/***************************************************************************************************************/
