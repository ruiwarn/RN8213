#define  _Drive_ALLINT
#include <Drive_Lib.h>

#include <Scom.h>
#include <Dl645_Bkgrd.h>

extern 	sComPack_TypeDef 		ComPack[SCOM_PORT_MAX];	//在Dl645_Globals.h中定义。
extern	sDl645Bkgrd_TypeDef 	Dl645Bkgrd;
extern	sD645Inactive_TypeDef	Dl645Inactive;
extern	sDl645GlobalsFlag_TypeDef	Dl645IntFlag;
extern	u16		SysStamp500;
extern	sDF01	SysClock;
extern	u32 CMPFlag;
extern	u16		GataSysStamp;


void TC0_HANDLER(void)    //系统主节拍.1ms
{
	TC0->STA = 0x0f;
	fnKey_Scan();
	SysStamp++;		//1MS
	SysStamp500++;
}



void CMP_HANDLER(void)
{
	if(!(MADC->LVD_STAT & 0x04))
	{
		Dl645Inactive.PubData.InactiveStamp= SysStamp ? SysStamp:(SysStamp-1);//延时判断，保存数据，使电能表进入睡眠状态
	}
	else 
	{	
		if(Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON) ;
	}
	MADC->LVD_STAT = 0x01ff;
}

void VCH_HANDLER(void)
{
	if((MADC->LVD_STAT & 0x80))
	{
		if(Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON) ;
	}
	MADC->LVD_STAT = 0x0180;
}


void UART0_HANDLER(void)
{
	u32  status;
	u8	 temp;
  	status = UART0->STA;
  
  	/* UART error irq */
	if((UART0->CTRL & 0x8) && (status & 0x3c))
	{
		ComPack[SCOM_PORT_RS485A].RxLen=0;
		ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_IDLE;
		UART0->STA = status;
	}
  
	/* receive data complete irq */
  	if((UART0->CTRL & 0x4) && (status & 0x1))
  	{
    	temp = UART0->RXD;
    	if(ComPack[SCOM_PORT_RS485A].EFlag == SCOMPK_EFLAG_IDLE)
    	{
    		if(temp == 0x68) 
    		{
    			ComPack[SCOM_PORT_RS485A].RxBuf[ComPack[SCOM_PORT_RS485A].RxLen++]=temp;
    			ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_RXHEAD1;
    			ComPack[SCOM_PORT_RS485A].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		}
    	}
    	else if(ComPack[SCOM_PORT_RS485A].EFlag == SCOMPK_EFLAG_RXHEAD1)
    	{
    		if(ComPack[SCOM_PORT_RS485A].RxLen>6) 
    		{
    			if(temp == 0x68) 
    			{
    				ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_RXHEAD2;
    				ComPack[SCOM_PORT_RS485A].RxBuf[ComPack[SCOM_PORT_RS485A].RxLen++]=temp;
    				ComPack[SCOM_PORT_RS485A].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    			}
    			else ComPack[SCOM_PORT_RS485A].RxLen=0x0;
    		}
    		else 
    		{
    			ComPack[SCOM_PORT_RS485A].RxBuf[ComPack[SCOM_PORT_RS485A].RxLen++]=temp;
    			ComPack[SCOM_PORT_RS485A].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		}
    	}
    	else if(ComPack[SCOM_PORT_RS485A].EFlag == SCOMPK_EFLAG_RXHEAD2)
    	{
    		ComPack[SCOM_PORT_RS485A].RxBuf[ComPack[SCOM_PORT_RS485A].RxLen++]=temp;
    		ComPack[SCOM_PORT_RS485A].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		if(ComPack[SCOM_PORT_RS485A].RxLen>8) ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_RXDATALEN;
    	}
    	else if(ComPack[SCOM_PORT_RS485A].EFlag == SCOMPK_EFLAG_RXDATALEN)
    	{
    		ComPack[SCOM_PORT_RS485A].RxBuf[ComPack[SCOM_PORT_RS485A].RxLen++]=temp;
    		ComPack[SCOM_PORT_RS485A].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		if(ComPack[SCOM_PORT_RS485A].RxLen>(ComPack[SCOM_PORT_RS485A].RxBuf[9]+10)) 
    			ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_RXEND;
    	}
    	else if(ComPack[SCOM_PORT_RS485A].EFlag == SCOMPK_EFLAG_RXEND)
    	{
    		if(temp == 0x16) 
			{
				ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_RXFINISH;
				ComPack[SCOM_PORT_RS485A].RxBuf[ComPack[SCOM_PORT_RS485A].RxLen++]=temp;
				ComPack[SCOM_PORT_RS485A].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
			}
			else ComPack[SCOM_PORT_RS485A].RxLen=0x0;
    	}
    	UART0->STA = 0x1;			// clear receive IF
  	}
  
  	/* transmit data complete irq */
  	if((UART0->CTRL & 0x2) && (status & 0x2))
  	{
    	if(ComPack[SCOM_PORT_RS485A].EFlag==SCOMPK_EFLAG_TXBUSY)
		{
			if(ComPack[SCOM_PORT_RS485A].TxLen>0)
			{
				UART0->TXD=*(ComPack[SCOM_PORT_RS485A].pTx++);
				ComPack[SCOM_PORT_RS485A].TxLen--;
			}
			else
			{
				ComPack[SCOM_PORT_RS485A].EFlag=SCOMPK_EFLAG_TXFINISH;
				fnScomPk_Init(SCOM_PORT_RS485A);//此处新增加的，接收数据完成后初始化
			}
		}
    	UART0->STA = 0x2;
  	}
  	return;
}

void UART1_HANDLER(void)
{
	u32  status;
	u8	 temp;
  	status = UART1->STA;
  
  	/* UART error irq */
	if((UART1->CTRL & 0x8) && (status & 0x3c))
	{
		ComPack[SCOM_PORT_IR].RxLen=0;
		ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_IDLE;
		UART1->STA = status;
	}
  
	/* receive data complete irq */
  	if((UART1->CTRL & 0x4) && (status & 0x1))
  	{
    	temp = UART1->RXD;
    	if(ComPack[SCOM_PORT_IR].EFlag == SCOMPK_EFLAG_IDLE)
    	{
    		if(temp == 0x68) 
    		{
    			ComPack[SCOM_PORT_IR].RxBuf[ComPack[SCOM_PORT_IR].RxLen++]=temp;
    			ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_RXHEAD1;
    			ComPack[SCOM_PORT_IR].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		}
    	}
    	else if(ComPack[SCOM_PORT_IR].EFlag == SCOMPK_EFLAG_RXHEAD1)
    	{
    		if(ComPack[SCOM_PORT_IR].RxLen>6) 
    		{
    			if(temp == 0x68) 
    			{
    				ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_RXHEAD2;
    				ComPack[SCOM_PORT_IR].RxBuf[ComPack[SCOM_PORT_IR].RxLen++]=temp;
    				ComPack[SCOM_PORT_IR].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    			}
    			else ComPack[SCOM_PORT_IR].RxLen=0x0;
    		}
    		else 
    		{
    			ComPack[SCOM_PORT_IR].RxBuf[ComPack[SCOM_PORT_IR].RxLen++]=temp;
    			ComPack[SCOM_PORT_IR].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		}
    	}
    	else if(ComPack[SCOM_PORT_IR].EFlag == SCOMPK_EFLAG_RXHEAD2)
    	{
    		ComPack[SCOM_PORT_IR].RxBuf[ComPack[SCOM_PORT_IR].RxLen++]=temp;
    		ComPack[SCOM_PORT_IR].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		if(ComPack[SCOM_PORT_IR].RxLen>8) ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_RXDATALEN;
    	}
    	else if(ComPack[SCOM_PORT_IR].EFlag == SCOMPK_EFLAG_RXDATALEN)
    	{
    		ComPack[SCOM_PORT_IR].RxBuf[ComPack[SCOM_PORT_IR].RxLen++]=temp;
    		ComPack[SCOM_PORT_IR].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		if(ComPack[SCOM_PORT_IR].RxLen>(ComPack[SCOM_PORT_IR].RxBuf[9]+10)) ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_RXEND;
    	}
    	else if(ComPack[SCOM_PORT_IR].EFlag == SCOMPK_EFLAG_RXEND)
    	{
    		if(temp == 0x16) 
			{
				ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_RXFINISH;
				ComPack[SCOM_PORT_IR].RxBuf[ComPack[SCOM_PORT_IR].RxLen++]=temp;
				ComPack[SCOM_PORT_IR].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
			}
			else ComPack[SCOM_PORT_IR].RxLen=0x0;
    	}
    	UART1->STA = 0x1;			// clear receive IF
  	}
  
  	/* transmit data complete irq */
  	if((UART1->CTRL & 0x2) && (status & 0x2))
  	{
    	if(ComPack[SCOM_PORT_IR].EFlag==SCOMPK_EFLAG_TXBUSY)
		{
			if(ComPack[SCOM_PORT_IR].TxLen>0)
			{
				UART1->TXD=*(ComPack[SCOM_PORT_IR].pTx++);
				ComPack[SCOM_PORT_IR].TxLen--;
			}
			else
			{
				ComPack[SCOM_PORT_IR].EFlag=SCOMPK_EFLAG_TXFINISH;
				fnScomPk_Init(SCOM_PORT_IR);//此处新增加的，接收数据完成后初始化
			}
		}
    	UART1->STA = 0x2;
  	}
  	UART1->STA = status;
  	return;
}

void UART3_HANDLER(void)
{
	u32  status;
	u8	 temp;
  	status = UART3->STA;
  
  	/* UART error irq */
	if((UART3->CTRL & 0x8) && (status & 0x3c))
	{
		ComPack[SCOM_PORT_ZBCOM].RxLen=0;
		ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_IDLE;
		UART3->STA = status;
	}
  
	/* receive data complete irq */
  	if((UART3->CTRL & 0x4) && (status & 0x1))
  	{
    	temp = UART3->RXD;
    	if(ComPack[SCOM_PORT_ZBCOM].EFlag == SCOMPK_EFLAG_IDLE)
    	{
    		if(temp == 0x68) 
    		{
    			ComPack[SCOM_PORT_ZBCOM].RxBuf[ComPack[SCOM_PORT_ZBCOM].RxLen++]=temp;
    			ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_RXHEAD1;
    			ComPack[SCOM_PORT_ZBCOM].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		}
    	}
    	else if(ComPack[SCOM_PORT_ZBCOM].EFlag == SCOMPK_EFLAG_RXHEAD1)
    	{
    		if(ComPack[SCOM_PORT_ZBCOM].RxLen>6) 
    		{
    			if(temp == 0x68) 
    			{
    				ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_RXHEAD2;
    				ComPack[SCOM_PORT_ZBCOM].RxBuf[ComPack[SCOM_PORT_ZBCOM].RxLen++]=temp;
    				ComPack[SCOM_PORT_ZBCOM].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    			}
    			else ComPack[SCOM_PORT_ZBCOM].RxLen=0x0;
    		}
    		else 
    		{
    			ComPack[SCOM_PORT_ZBCOM].RxBuf[ComPack[SCOM_PORT_ZBCOM].RxLen++]=temp;
    			ComPack[SCOM_PORT_ZBCOM].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		}
    	}
    	else if(ComPack[SCOM_PORT_ZBCOM].EFlag == SCOMPK_EFLAG_RXHEAD2)
    	{
    		ComPack[SCOM_PORT_ZBCOM].RxBuf[ComPack[SCOM_PORT_ZBCOM].RxLen++]=temp;
    		ComPack[SCOM_PORT_ZBCOM].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		if(ComPack[SCOM_PORT_ZBCOM].RxLen>8) ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_RXDATALEN;
    	}
    	else if(ComPack[SCOM_PORT_ZBCOM].EFlag == SCOMPK_EFLAG_RXDATALEN)
    	{
    		
    		ComPack[SCOM_PORT_ZBCOM].RxBuf[ComPack[SCOM_PORT_ZBCOM].RxLen++]=temp;
    		ComPack[SCOM_PORT_ZBCOM].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
    		if(ComPack[SCOM_PORT_ZBCOM].RxLen>(ComPack[SCOM_PORT_ZBCOM].RxBuf[9]+10)) ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_RXEND;
    	}
    	else if(ComPack[SCOM_PORT_ZBCOM].EFlag == SCOMPK_EFLAG_RXEND)
    	{
    		if(temp == 0x16) 
			{
				ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_RXFINISH;
				ComPack[SCOM_PORT_ZBCOM].RxBuf[ComPack[SCOM_PORT_ZBCOM].RxLen++]=temp;
				ComPack[SCOM_PORT_ZBCOM].TimeOutStamp=SysStamp?SysStamp:(SysStamp-1);
			}
			else ComPack[SCOM_PORT_ZBCOM].RxLen=0x0;
    	}
    	UART3->STA = 0x1;			// clear receive IF
  	}
  
  	/* transmit data complete irq */
  	if((UART3->CTRL & 0x2) && (status & 0x2))
  	{
    	if(ComPack[SCOM_PORT_ZBCOM].EFlag==SCOMPK_EFLAG_TXBUSY)
		{
			if(ComPack[SCOM_PORT_ZBCOM].TxLen>0)
			{
				UART3->TXD=*(ComPack[SCOM_PORT_ZBCOM].pTx++);
				ComPack[SCOM_PORT_ZBCOM].TxLen--;
			}
			else
			{
				ComPack[SCOM_PORT_ZBCOM].EFlag=SCOMPK_EFLAG_TXFINISH;
				fnScomPk_Init(SCOM_PORT_ZBCOM);//此处新增加的，接收数据完成后初始化
			}
		}
    	UART3->STA = 0x2;
  	}
  	return;
}

void KBI_HANDLER(void)
{
	if(KBI->DATA&0x01) 
	{
		Dl645Inactive.InactiveFlag.fChange |=0x01;
	}
	if(KBI->DATA&0x02) Dl645Inactive.InactiveFlag.fChange |=0x04;
	KBI->DATA = 0xff;
}

void ISO78161_HANDLER(void)
{
	if(ISO7816->STA1&0x1000)	 //接收到OLD信号
	{
		if(!(Key.Press & KEY_VAL_CARD))
		{
			Key.Press |= KEY_VAL_CARD;
			Key.Release &= ~KEY_VAL_CARD;
			I7816ComExt.EFlag = 1;
		}
	}
	else if(ISO7816->STA1&0x2000)   // 检测到卡拔出
	{
		if(!(Key.Release & KEY_VAL_CARD))
		{
			Key.Release |= KEY_VAL_CARD;
			Key.Press &= ~KEY_VAL_CARD;
			ISO7816->CTRL1 &= 0x7fffffff;
		}
	}
	ISO7816->STA1 |=0x3000;
}

void RTC_HANDLER(void)
{
	if(RTC->IF&0x02)        // 定时1
	{
		Dl645Inactive.PubData.ActiveStamp++;
		if(Dl645Inactive.PubData.PowerErrFlag == 0x5a)		// 检测电源异常事件1s
		{
			Dl645Inactive.PubData.PowerErrFlag = 0x5b;
		}
	}
	if(Dl645Inactive.PubData.ActiveState==Running_State)
	{
		fnSysClock_Read(&SysClock);
	}
	Dl645Inactive.InactiveFlag.fChange |=0x02;
	RTC->IF = 0xff;
}


void NMI_Handler(void)
{
	
}

void HardFault_Handler(void)
{
	WDT->EN = 0xbc;
}

void SVC_Handler(void)
{
	WDT->EN = 0xbc;
}

void PendSV_Handler(void)
{
	WDT->EN = 0xbc;
}

void SysTick_Handler(void)
{
	
}

void EMU_HANDLER(void)
{
	
}

void MADC_HANDLER(void)
{
	
}
//---------------------------------------------------


