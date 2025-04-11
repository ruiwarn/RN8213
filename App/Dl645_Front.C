
#define  _DL645_FRONT_GLOBALS
#include <App.h>


//函数功能：时钟电池电压测量
//入口参数：无
//出口参数：无
void fnFront_ClockBatCalculate(void)
{
	u32 ClockBat;
	u8	i;
	
	for(i=0;i<255;i++)
	{
		if(!(MADC->AD_STAT&0x02))  break;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
	MADC->AD_CTRL = 0x01;									// set ADCChannel 2  vbat  0.5倍增益
	MADC->AD_START = 0x01;									// star ADC
	SystemDelay(1);
	for(i=0;i<150;i++)
	{
		if(MADC->AD_STAT&0x01)  break;
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
	ClockBat = MADC->AD_DATA;
	ClockBat = (ClockBat * 508)/1024;
	Dl645Front.PubData.ClockBat = fnHexToBcd_u16((u16)(ClockBat));	//时钟电池电压 U16
	// 当前温度值
	ClockBat = RTC->TEMP;
	ClockBat &= 0x3ff;
	if(ClockBat&0x200) 
	{	
		ClockBat = 0x3ff - ClockBat;
		Dl645Front.PubData.Temperature = fnHexToBcd_u16((u16)((ClockBat*10)/4)) | 0x8000;
	}
	else Dl645Front.PubData.Temperature = fnHexToBcd_u16((u16)((ClockBat*10)/4));
}
/*****************************************************************************
** Function name:	fnEMU_Init(void)
**
** Description:		计量EMU初始化函数，写入校表寄存器
**
** Parameters:		NONE
**
** Returned value:	NONE
**
******************************************************************************/
void fnEMU_Init(void)
{
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->ADC_CTRL = 0x0003;							// 将I1通道配置为16倍增益
	SYSCTL->SYS_PS = 0x00;
	EMU->SPCMD = 0xe5;			 							// 写使能位
	EMU->HFConst 	= 	Dl645FirmPara.HFConst;
	EMU->IAGAIN 	= 	Dl645FirmPara.IAGain;
	EMU->IBGAIN 	= 	Dl645FirmPara.IBGain;
	EMU->UGAIN 		= 	Dl645FirmPara.UGain;
	EMU->GPQA		= 	Dl645FirmPara.GPQA;
	EMU->GPQB		= 	Dl645FirmPara.GPQB;
	EMU->PhsA 		= 	Dl645FirmPara.PhsA;
	EMU->PhsB 		= 	Dl645FirmPara.PhsB;
	EMU->APOSA 		= 	Dl645FirmPara.APOSA;
	EMU->APOSB 		= 	Dl645FirmPara.APOSB;
	EMU->PStart 	= 	Dl645FirmPara.PStart;
	EMU->QStart 	= 	Dl645FirmPara.QStart;
	EMU->IARMSOS 	= 	Dl645FirmPara.IARMSOS;
	EMU->IBRMSOS 	= 	Dl645FirmPara.IBRMSOS;
	EMU->SPCMD 		= 	0xdc; 							// 写保护
}
/*****************************************************************************
** Function name:	fnDl645Front_Init(void)
**
** Description:		计量模块初始化函数，将计量用到的RAM数据进行初始化
**
** Parameters:		NONE
**
** Returned value:	NONE
**
******************************************************************************/
void fnDl645Front_Init(void)
{
	u8 	i,j;
	u32 ChkSum;	
	memset(&Dl645FrontTmp , 0 , sizeof(sDl645FrontTmp_TypeDef) );
	memset(&Dl645Front , 0 , sizeof(sDl645Front_TypeDef) );
    fnDl645File_Read(Dl645FileId_FirmPara , 0 , (u8 *)&Dl645FirmPara , sizeof(sDl645FirmParaFile_TypeDef) );
	Dl645FrontTmp.ChkSum1 = Dl645FirmPara.ChkSum;
	//写入校表寄存器，检查校验是否正确。正确退出，不正确再次写入。最多循环写入次数5次。
	for(i=0;i<5;i++)
	{
        fnEMU_Init();
        SystemDelay(10);
        for(j=0;j<5;j++)
        {
        	if(!(EMU->EMUStatus & 0x01000000))	break;
        	SystemDelay(10);
        }
        fnWDT_Restart();
        ChkSum = EMU->EMUStatus & 0x00ffffff;
    	if(Dl645FrontTmp.ChkSum1 == ChkSum) break;
    }
    RtcWriteDota(Dl645FirmPara.RTCDota0);
    fnWDT_Restart();
    fnFront_ClockBatCalculate();
    fnWDT_Restart();
}

//函数功能：从计量芯片读取数据
//入口参数：无
//出口参数：无
void fnDl645Front_Exec(void)
{ 
    u8 i;
    u32 TempU,TempI,TempIn;
    u32 TempStatus;
    u16 TempAngle;	
   	//有LVD标志，为更快速检测达到掉电，直接退出
	if((Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON)||(Dl645Inactive.PubData.InactiveStamp)) return;
		
  	
    //与系统1mS节拍定时器作差，每1000mS执行一次。时间不到1000mS，退出
    //if(fnStamp_Through(Dl645Front.PubData.FrontStamp) < 1000 )
	//{
	//	return;
	//}
	//Dl645Front.PubData.FrontStamp = SysStamp;	
	if(SysStamp500 > 500 )
	{
		Dl645Bkgrd.PubData.fChange.FlagBit.t500ms = 1;
		SysStamp500 -=500;
	}
	   
    if(!Dl645Bkgrd.PubData.fChange.FlagBit.t500ms) return;
    //连续三次校验出错，重新初始化校表寄存器	
    //if(Dl645FrontTmp.ChkErrCnt > 3) Dl645Front.PriData.Flag = FRONT_FLAG_RN8209RST;    	 
    //if(Dl645Front.PriData.Flag == FRONT_FLAG_RN8209RST)   
    //{
    //	fnDl645Front_Init();
    //	//Dl645Front.PriData.Flag = 0 ;
    //	return;
    //}
	
	//时钟电池电压测量
    fnFront_ClockBatCalculate(); 
	if(Dl645Front.PriData.Flag & FRONT_FLAG_RN8209RST)
	{
		fnDl645Front_Init();
		Dl645Front.PriData.Flag &= ~FRONT_FLAG_RN8209RST;
		return;
	}
	fnDl645File_Read(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_RTCDota0,(u8 *)&Dl645FirmPara.RTCDota0,2);
    if(RtcReadDota() !=Dl645FirmPara.RTCDota0 ) RtcWriteDota(Dl645FirmPara.RTCDota0);
    //读状态寄存器，得到校验；功率方向；通道
    TempStatus = EMU->EMUStatus;
    if(!(TempStatus & 0x01000000))
    { 
		if(Dl645FrontTmp.ChkSum1 == (TempStatus&0x00ffffff)) {Dl645FrontTmp.ChkErrCnt = 0 ;}
		else 
		{
			Dl645FrontTmp.ChkErrCnt++; 
			if(Dl645FrontTmp.ChkErrCnt > 3) 
			{
				Dl645Front.PriData.Flag |= FRONT_FLAG_RN8209RST;
				Dl645FrontTmp.ChkErrCnt = 0 ;
			}
			return;
		}
    }
    //判断功率方向
    if(TempStatus & 0x02000000) {Dl645Front.PubData.PDirect =  INVERSION ;}  //判断功率方向是反的。
    else {Dl645Front.PubData.PDirect = POSITIVE ;}    

//	if(!((TempStatus & 0x20000000)?(0x01):(0x00)))			//检查当前不是A通道强制切换到A通道
//	{
//		EMU->SPCMD = 0xe5;
//		EMU->SPCMD = 0x5a ;   							//选择A通道为计量通道
//		EMU->SPCMD = 0xdc;
//	}
    
    //读电压、电流到缓冲区，读数为负进行处理
	Dl645FrontTmp.UI[0] = EMU->IARMS;
	Dl645FrontTmp.UI[1] = EMU->IBRMS;
	Dl645FrontTmp.UI[2] = EMU->URMS;
    for(i = 0 ; i < 3 ; i++)
    {
		if(Dl645FrontTmp.UI[i]&0x00800000) Dl645FrontTmp.UI[i]=0;
    }
    //读频率
	Dl645FrontTmp.Frequency =  EMU->Ufreq;
    //读功率到缓冲区，读数为负进行处理
    Dl645FrontTmp.Pw[0] = EMU->PowerPA;
    Dl645FrontTmp.Pw[1] = EMU->PowerPB;   
	for(i = 0 ; i < 2 ; i++)
    {
		if(Dl645FrontTmp.Pw[i]&0x80000000) Dl645FrontTmp.Pw[i]=(~Dl645FrontTmp.Pw[i])+1;
	}
	//计算功率
	Dl645Front.PubData.Pw = fnDFConver_Hex32ToDF09((s32)((Dl645FrontTmp.Pw[0])/((Dl645FirmPara.KPrms))));
	//电流电压计算
	Dl645Front.PubData.U = 0x7fff&(fnDFConver_Bcd16To16((s16)(Dl645FrontTmp.UI[2]/(10*(Dl645FirmPara.KUrms))))); //电压
    TempI = (s32)(Dl645FrontTmp.UI[0]/(Dl645FirmPara.KIArms));   
    TempIn = (s32)(Dl645FrontTmp.UI[1]/(Dl645FirmPara.KIBrms));	
	Dl645Front.PubData.Ia = fnDFConver_Bcd32To32(TempI);	
	Dl645Front.PubData.In = fnDFConver_Bcd32To32(TempIn);
	//功率有效值计算,功率因数计算
	if(Dl645Front.PubData.Chnsel)
	{
		//功率
		Dl645Front.PubData.Pw = fnDFConver_Hex32ToDF09((s32)((Dl645FrontTmp.Pw[1])/((Dl645FirmPara.KPrms))));
		//功率因数
		TempU = EMU->PFB;
	    TempAngle = EMU->ANGLEB;
	}	
	else
	{
		Dl645Front.PubData.Pw = fnDFConver_Hex32ToDF09((s32)((Dl645FrontTmp.Pw[0])/((Dl645FirmPara.KPrms))));
		TempU = EMU->PFA;
        TempAngle = EMU->ANGLEA;
		
	}
	//计算功率因数
	TempU &=0x00ffffff; 
	if(TempU&0x00800000) TempU=((~TempU)&0x00ffffff)+1;
	Dl645Front.PubData.Pf = fnHexToBcd_u16((u16)((float)TempU/8388.608));
	Dl645Front.PubData.Angle=	fnHexToBcd_u16(TempAngle*3600/32768); 
	//功率小于0.0030，清零，功率因数置0.999
	if(((Dl645Front.PubData.Pw.Dat2&0x7f)==0)&&(Dl645Front.PubData.Pw.Dat1==0)&&(Dl645Front.PubData.Pw.Dat0<0x30)) 
	{
		Dl645Front.PubData.Pw.Dat0 = 0;
		Dl645Front.PubData.Pw.Dat1 = 0;
		Dl645Front.PubData.Pw.Dat2 = 0;
	}
	//电流小于起动电流，清零
	if((Dl645Front.PubData.Ia) < 0x00000150) {Dl645Front.PubData.Ia = 0;Dl645Front.PubData.Pf = 0x0999;}
	if((Dl645Front.PubData.In) < 0x00000150) Dl645Front.PubData.In = 0;
	
	//电压频率计算	
	Dl645Front.PubData.Frequency = fnHexToBcd_u16((u16)(((u32)184320000)/((u32)4*Dl645FrontTmp.Frequency)));	
	
	//读电能脉冲，加入脉冲计数器
	Dl645FrontTmp.Pulse = EMU->EnergyP;
	if(Dl645FrontTmp.Pulse > 100) Dl645FrontTmp.Pulse = 0;	//容错，脉冲个数过大，清除
	Dl645FrontTmp.Pulse_Eg+=Dl645FrontTmp.Pulse;
	#if  DL645SOFT_DEBUG
	Dl645FrontTmp.Pulse_Eg+=1;
	#endif
}



