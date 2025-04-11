
#define  _DL645_ICEsam_GLOBALS
#include <App.h>

//SOC与卡隔离专用芯片进行同步，进行卡复位信息的获取
ErrorStatus fnICEsam_CheckCard(void)
{
	ErrorStatus result;
	u8 tempData,i;
	u32	temp0;    
	ISO7816->CTRL1 = 0x70000201;    
  	ISO7816->STA1 = 0x3f; 					//clear status
  	ISO7816->BDDIV1 = 0X230000;				// F/D =768 2400
  	if(fnCardSendByte(CARD , 0x00) == ERROR) return(ERROR);
	ISO7816->BDDIV1 = 0X213300;				// F/D =307 6000
	if(fnCardSendByte(CARD , 0x16) == ERROR) return(ERROR);
	tempData = ISO7816->DATA1;       
    ISO7816->CTRL1 = 0x70000201;
    ISO7816->STA0 = 0x3f; 					//clear status
	result = fnCardGetByte( CARD, &tempData ); //receive procedure byte.
	if(( result != SUCCESS )||(tempData != 0x16)) return(ERROR);
	if(fnCardGetByte( CARD, &tempData ) == ERROR) return(ERROR); //receive procedure byte.
	temp0 = (u32)(307 * 372) /(u32)(tempData<<1);
	temp0 = (temp0 << 8) & 0x002fff00;
	ISO7816->BDDIV1 = (temp0 | 0x00200000);
	for(i=0;i<200;i++)
	{
		fnDelay_1us();
		fnDelay_1us();
	}
			
	if(fnCardSendByte(CARD , 0x16) == ERROR) return(ERROR);
	result = fnCardGetByte( CARD, &tempData ); //receive procedure byte.
	if(( result != SUCCESS )||(tempData != 0x16)) return(ERROR);
	for(i=0; i<13; i++)
	{
		result = fnCardGetByte( CARD, &tempData ); //receive TS.
		if( result != SUCCESS ) 
			return(ERROR);
		ICEsamComm.Ic_Comm_RxBuf[i] = tempData;
	}
	return(SUCCESS);
}


void fnICEsam_Card_Proc( void )
{
#if(METERTYPE ==1)
	eICEsam_ErrInfo_TypeDef Ic_Err_Flag;
	if((Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON)||(Dl645Inactive.PubData.InactiveStamp)) return;
	if( !I7816ComExt.EFlag) return;
	if(Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON) return;
	if(Dl645Front.PubData.U < 0x1750) 
	{
		Ic_Err_Flag = IC_PowerDn_ERR;
	}
	else
	{
		PinWrite_LCDLight(1);
		Dl645Session.BkLightFlag=0x5a;
		Dl645Session.BkLightStamp=194;					//启动背光点亮计时
		CardFoutOpen();
		if(fnICEsam_CheckCard()== ERROR) 
		{
			ISO7816->BDDIV1 = 0X230000;				// F/D =768 2400
	  		fnCardSendByte(CARD , 0x00);			// close clk
			I7816ComExt.EFlag = 0;
			fnLcd_SetSegment(SEG_DuKa,1);
			fnLcd_SetSegment(SEG_Fail,1);
			fnLcd_RefreshLcdBuf();
			Dl645Session.Stamp=3;
			Dl645Session.CardDispStamp=3;
			Dl645Session.DisplyErrFlag=0;
			ISO7816->CTRL1 |= 0x80000000;
			memset(&ICEsamComm.Ic_Comm_RxBuf[9] , 0x00 , 7);
			ICEsamComm.Ic_Comm_RxBuf[8] = Ic_Err_Flag;
			memset(&ICEsamComm.Ic_Comm_RxBuf[0] , 0xFF , 8);
			return;
		}
	}
	ISO7816->BDDIV1 = 0X230000;				// F/D =768 2400
	fnLcd_SetSegment(SEG_DuKa,1);			//显示“读卡中”
	fnLcd_SetSegment(SEG_Success,1);
	Dl645Session.Stamp=3;
	Dl645Session.CardDispStamp=3;
	Dl645Session.DisplyErrFlag=0;
	fnLcd_RefreshLcdBuf();
	//卡操作完成，关闭CLK，判断卡是否拔出
	ISO7816->BDDIV1 = 0X230000;				// F/D =768 2400
  	fnCardSendByte(CARD , 0x00);			// close clk
    I7816ComExt.EFlag = 0;
    ISO7816->CTRL1 |= 0x80000000;
    CardFoutClose();
#endif
}


