
#define  _SCOM_GLOBALS
#include <App.h>
 
//-------------------------------------

const u8 eUsartBps_Def[8]={USART_BPS_300,USART_BPS_600,USART_BPS_1200,USART_BPS_2400,USART_BPS_4800,
	USART_BPS_7200,USART_BPS_9600,USART_BPS_19200};
	
	
void fnScomPk_TxEnDis(u8 ComPort , FunctionalState EnOrDis)  //收发控制DIR
{
}

void fnScomPk_Init(u8 ComPort)  //端口初始化
{
	u8 USART_BPS;
	if(ComPort >= SCOM_PORT_MAX) return;
	
	ComPack[ComPort].EFlag = 0;
 	ComPack[ComPort].RxLen = 0;
 	ComPack[ComPort].TxLen = 0;
  	ComPack[ComPort].TimeOutStamp = 0;	
 	memset(&ComPack[ComPort].RxBuf[0] , 0 , MAX_COMPACK_SIZE);
	memset(&ComPack[ComPort].TxBuf[0] , 0 , MAX_COMPACK_SIZE);
	
	if(ComPack[ComPort].NewBps & 0x01) USART_BPS=eUsartBps_Def[0];
  	else if(ComPack[ComPort].NewBps & 0x02) USART_BPS=eUsartBps_Def[1];
  	else if(ComPack[ComPort].NewBps & 0x04) USART_BPS=eUsartBps_Def[2];
  	else if(ComPack[ComPort].NewBps & 0x08) USART_BPS=eUsartBps_Def[3];
  	else if(ComPack[ComPort].NewBps & 0x10) USART_BPS=eUsartBps_Def[4];
  	else if(ComPack[ComPort].NewBps & 0x20) USART_BPS=eUsartBps_Def[6];
  	else if(ComPack[ComPort].NewBps & 0x40) USART_BPS=eUsartBps_Def[7];
  	else USART_BPS=eUsartBps_Def[3];
	
	switch(ComPort)
	{
	case SCOM_PORT_RS485A:		
		fnUsart_Init(ComPort , USART_BPS | USART_EVEN_PARITY | USART_8BIT_DAT | USART_1STOP_SBIT );
		fnScomPk_TxEnDis(ComPort , DISABLE);
		break;
	case SCOM_PORT_IR:
		fnUsart_Init(ComPort , USART_BPS_1200 | USART_EVEN_PARITY | USART_8BIT_DAT | USART_1STOP_SBIT );
		break;
	case SCOM_PORT_ZBCOM:
		fnUsart_Init(ComPort , USART_BPS_2400 | USART_EVEN_PARITY | USART_8BIT_DAT | USART_1STOP_SBIT );
		break;
	default:
		return;
	}

}

void fnScomPk_ChangeBps(u8 ComPort)   //修改波特率
{
	if(ComPack[ComPort].fBps == 0) return;
	fnScomPk_Init(ComPort);
	ComPack[ComPort].NewBpsStamp = 0;
	ComPack[ComPort].fBps = 0;
}


void fnScom_AllInit(void)
{
	fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_ComPara + 2 , &ComPack[SCOM_PORT_RS485A].NewBps , 1 );	
	fnScomPk_Init(SCOM_PORT_RS485A);
	fnScomPk_Init(SCOM_PORT_IR);
	fnScomPk_Init(SCOM_PORT_ZBCOM);
}

void fnScomPk_Exec(u8 ComPort)  //通讯包执行--
{
	u16			Len = 0;

	if(ComPort >= SCOM_PORT_MAX) return;
	//波特率变更处理
	
	if((Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON)||(Dl645Inactive.PubData.InactiveStamp)) return;	
			
	if((ComPack[ComPort].EFlag == SCOMPK_EFLAG_RXHEAD1) || (ComPack[ComPort].EFlag == SCOMPK_EFLAG_RXHEAD2) || (ComPack[ComPort].EFlag == SCOMPK_EFLAG_RXDATALEN) || (ComPack[ComPort].EFlag == SCOMPK_EFLAG_RXEND))  //接收超时处理
	{
		if(fnStamp_Through(ComPack[ComPort].TimeOutStamp) > 500 ) ComPack[ComPort].EFlag = SCOMPK_EFLAG_INIT;
		else return;	
	}
	if(ComPack[ComPort].EFlag == SCOMPK_EFLAG_RXFINISH)  //包接收完，规约处理，是否需要发送
	{
		Len = fnPt_Exec(ComPort);
		if(Len) 
		{
			ComPack[ComPort].TxLen = Len;
			ComPack[ComPort].EFlag = SCOMPK_EFLAG_TXSTART;
			Dl645Bkgrd.PubData.CommStatus|=F_Comm1Event;
			
		}
		else 
		{
			ComPack[ComPort].EFlag = SCOMPK_EFLAG_INIT;
		}		
	}
	if(ComPack[ComPort].EFlag == SCOMPK_EFLAG_TXSTART)  //发送开启?
	{
		if(fnStamp_Through(ComPack[ComPort].TimeOutStamp) < 20) return;	//  接收完成到发送进行20MS的延时
		ComPack[ComPort].EFlag = SCOMPK_EFLAG_TXBUSY;
		memset(&ComPack[ComPort].TxBuf[0] , DL645B_LEADERCODE , DL645B_LEADERCODE_LEN);
		ComPack[ComPort].TxLen += DL645B_LEADERCODE_LEN;
		fnScomPk_TxEnDis(ComPort , ENABLE);	
		ComPack[ComPort].pTx=&ComPack[ComPort].TxBuf[0];
		USART_ITConfig(ComPort , USART_IT_RX, DISABLE);         //禁止接收
		USART_ITConfig(ComPort , USART_IT_TX, ENABLE);         //发送使能
		ComPack[ComPort].TimeOutStamp = SysStamp ? SysStamp : (SysStamp - 1);
	}
	if(ComPack[ComPort].EFlag == SCOMPK_EFLAG_TXBUSY)  //发送超时处理
	{
		if(fnStamp_Through(ComPack[ComPort].TimeOutStamp) > (OS_TICKS_PER_SEC*3) ) ComPack[ComPort].EFlag = SCOMPK_EFLAG_TXFINISH;
		else return;
	}
	if(ComPack[ComPort].EFlag == SCOMPK_EFLAG_TXFINISH)  //发送完成
	{
		ComPack[ComPort].EFlag = SCOMPK_EFLAG_INIT;
		fnScomPk_TxEnDis(ComPort , DISABLE);
		USART_ITConfig(ComPort , USART_IT_TX, DISABLE);         //禁止发送
	}	
	if(ComPack[ComPort].EFlag == SCOMPK_EFLAG_INIT)  //端口恢复
	{
		fnScomPk_ChangeBps(ComPort);
		fnScomPk_Init(ComPort);
	}	
}

//------------------------------------------
u8 *fnPt_Check(u8 *Src , u16 Len , ePtType_TypeDef *PtType)  //帧格式检测
{
	u16 i;
	u8 	err;
	
	for(i = 0 ; i < Len ; i++ , Src++)
	{
		err = fnDl645B_FrameCheck( (sFrmDl645B_TypeDef *) Src);				// 07规约判断
		if(err == SUCCESS) 
		{
			if(Len < (  (*(sFrmDl645B_TypeDef *)Src).Len + 12) )
			{
				*PtType = PTTYEP_NONE;
				return(NULL);	
			}
			else
			{	
				*PtType = PTTYEP_DL645B;
				return(Src);
			}
		}
	}
	
	*PtType = PTTYEP_NONE;
	return(NULL);	
}

u16 fnPt_Exec(u8 ComPort)  //协议执行
{
	u8 				*sFrame;
	ePtType_TypeDef	PtType;
	u16				Len;
//	u8				TxAdd;

	sFrame = fnPt_Check(&ComPack[ComPort].RxBuf[0] , ComPack[ComPort].RxLen , &PtType);
	if(sFrame != NULL)	
	{
		if(PtType == PTTYEP_DL645B)
		{			
			Len = fnDl645B_Exec(ComPort , (sFrmDl645B_TypeDef *)sFrame);	//07规约处理						
		}
		else Len = 0;					
	}
	else Len = 0;
	return(Len);
}









