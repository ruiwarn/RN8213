// 07规约国网单相表

#define  DL645B_COM_GLOBALS
#include <App.h>

void fnDelay_5ms(void)
{
	u16 i;
	for(i=0;i<=14000;i++)
	{
		__NOP();
		__NOP(); 
	}	//大约20MS 8M。
	return;
}

void fnDl645B_Inverse(u8 *Buf, u8 Len)
{
	u8 i, Temp08;
	
	for( i=0; i<Len/2; i++ )
	{
		Temp08 = Buf[i];
		Buf[i] = Buf[Len-1-i];
		Buf[Len-1-i] = Temp08;
	}
}

ErrorStatus fnDl645B_ByteCheckBCD( u8 Value )
{
	if( ((Value&0xF0)<=0x90) && ((Value&0x0F)<=0x09) )
	{
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

ErrorStatus fnDl645B_FramCheckBCD( u8 *Buf, u8 Len )
{
	u8 i;
	
	for( i=0; i<Len; i++ )
	{
		if( fnDl645B_ByteCheckBCD(Buf[i]) != SUCCESS )		return ERROR;
	}
	
	return SUCCESS;
}


// 比较设置的切换时间是否合法
ErrorStatus fnDl645B_JudgeClockNoWeek( u8 Start, u8 *Ck, u8 Len )		
{
	u8  i;
	u8 Max[]={ 0x59,0x59,0x23,0x31,0x12,0x99 }, Min[]={ 0x00,0x00,0x00,0x01,0x01,0x00 };

	for( i=0; i<Len; i++ )
	{
		if( (Ck[i] > Max[i+Start]) || (Ck[i] < Min[i+Start]) ) return ERROR;//合法性检查错误
		if( fnDl645B_ByteCheckBCD(Ck[i]) != SUCCESS ) return ERROR;	//BCD码检查错误
	}
	
	return SUCCESS;//正确
}

ErrorStatus fnDl645B_AddrCheck(u8 *ComAddr , u8 AFN)
{
	const u8	BcAddr[] = {0x99,0x99,0x99,0x99,0x99,0x99};	
	const u8	RdAddr[] = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
	
	u8			MtAddr[6];		
	s8 			i , Flag;	
	
	if( (memcmp(&ComAddr[0] , &BcAddr , 6) == 0) &&
		( (AFN == 0x08) || (AFN == 0x16) || (AFN == 0x1D)) ) return(SUCCESS);							// 12个9只允许在广播校时和冻结命令中出现
	
	
	if(memcmp(&ComAddr[0] , &RdAddr , 6) != 0)
	{
		fnDl645File_Read(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_ComAddr , &MtAddr , 6 );
	
		for(Flag = 0 , i = 5 ; i >= 0 ; i--)
		{
			if(ComAddr[i] != MtAddr[i])
			{
				if( ComAddr[i] != 0xAA  ) return(ERROR);
				if(Flag) return(ERROR);
			}
			else Flag = 1;
		}
	}
	return(SUCCESS);
}

ErrorStatus	fnDl645B_FrameCheck(sFrmDl645B_TypeDef *pCheck)
{
	const u8	BcAddr[] = {0x99,0x99,0x99,0x99,0x99,0x99};	
	if(pCheck->Head1 != 0x68) return(ERROR);
	if(pCheck->Head2 != 0x68) return(ERROR);
	
	
	if((pCheck->AFN == 0x11) && (pCheck->UDat.DI0 == 0x33) && (pCheck->UDat.DI1 == 0x36) && (pCheck->UDat.DI2 == 0x37) && (pCheck->UDat.DI3 == 0x37)) // 液晶查看命令
	{
		if( memcmp(pCheck->Addr , &BcAddr , 6) != 0)
		{
			if(fnDl645B_AddrCheck(pCheck->Addr , pCheck->AFN) != SUCCESS) return(ERROR);
		}
	}
	else
	{
		if(fnDl645B_AddrCheck(pCheck->Addr , pCheck->AFN) != SUCCESS) return(ERROR);
	}
	if(pCheck->AFN!=0x03 && pCheck->AFN!=0x08 && pCheck->AFN<0x10) return(ERROR);
	
	if(pCheck->UDat.Dat[pCheck->Len - 4 + 0] != fnSum(&pCheck->Head1 , pCheck->Len + 10) ) return(ERROR);				
	if(pCheck->UDat.Dat[pCheck->Len - 4 + 1] != 0x16) return(ERROR);		
	
	return(SUCCESS);
}

void fnDl645B_Modify(u8 *pModify , u16 Len , u8 OpFlag)
{
	if(OpFlag)
	{
		for( ; Len != 0 ; Len--)
		{ 
			*pModify = *pModify + 0x33;
			pModify++;
		}
	}	
	else
	{
		for( ; Len != 0 ; Len--)
		{
			*pModify = *pModify - 0x33;
			pModify++;
		}
	}	
}

u16	fnDl645_PackErr(sFrmDl645B_TypeDef *TxFrm , sFrmDl645B_TypeDef *RxFrm , eDL645B_ErrInfo_TypeDef ErrInfo)
{																		// 返回错误桢
	TxFrm->Head1 = 0x68;
	fnDl645File_Read(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_ComAddr , &TxFrm->Addr , 6 );	
	TxFrm->Head2 = 0x68;
	
	TxFrm->AFN = RxFrm->AFN;
	TxFrm->Persist = 0;
//	TxFrm->Ack = 1;
	TxFrm->Dir = 1;
	
	if(ErrInfo != DL645B_ERRINFO_OK)
	{
		TxFrm->Ack = 1;
		TxFrm->Len = 1;	
		TxFrm->UDat.DI0 = ErrInfo;
		
		fnDl645B_Modify(&TxFrm->UDat.DI0 , TxFrm->Len , 1);	
		TxFrm->UDat.DI1 = fnSum(&TxFrm->Head1 , TxFrm->Len + 10);
		TxFrm->UDat.DI2 = 0x16;
	}	
	else 
	{
		TxFrm->Ack = 0;
		TxFrm->Len = 0;		
			
		fnDl645B_Modify(&TxFrm->UDat.DI0 , TxFrm->Len , 1);	
		TxFrm->UDat.DI0 = fnSum(&TxFrm->Head1 , TxFrm->Len + 10);
		TxFrm->UDat.DI1 = 0x16;
	}	
	
	return(TxFrm->Len + 12);
}

u16	fnDl645_PackOk(sFrmDl645B_TypeDef *TxFrm , sFrmDl645B_TypeDef *RxFrm , u16 Len)
{																
	TxFrm->Head1 = 0x68;
	fnDl645File_Read(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_ComAddr , &TxFrm->Addr , 6 );	
	TxFrm->Head2 = 0x68;
	
	TxFrm->AFN = RxFrm->AFN;
//	TxFrm->AFN = 0x11;
	TxFrm->Persist = 0;
	TxFrm->Ack = 0;
	TxFrm->Dir = 1;
	
	TxFrm->Len = Len;
	
	TxFrm->UDat.DI0 = RxFrm->UDat.DI0;
	TxFrm->UDat.DI1 = RxFrm->UDat.DI1;
	TxFrm->UDat.DI2 = RxFrm->UDat.DI2;
	TxFrm->UDat.DI3 = RxFrm->UDat.DI3;	
	fnDl645B_Modify(&TxFrm->UDat.DI0 , TxFrm->Len , 1);
	
	TxFrm->UDat.Dat[TxFrm->Len - 4 + 0] = fnSum(&TxFrm->Head1 , TxFrm->Len + 10);
	TxFrm->UDat.Dat[TxFrm->Len - 4 + 1] = 0x16;
	
	return(TxFrm->Len + 12);
}

u16	fnDl645_PackNoDI(sFrmDl645B_TypeDef *TxFrm , sFrmDl645B_TypeDef *RxFrm , u16 Len)
{
	TxFrm->Head1 = 0x68;
	fnDl645File_Read(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_ComAddr , &TxFrm->Addr , 6 );	
	TxFrm->Head2 = 0x68;
	
	TxFrm->AFN = RxFrm->AFN;
	TxFrm->Persist = 0;
	TxFrm->Ack = 0;
	TxFrm->Dir = 1;
	
	TxFrm->Len = Len;
	
	fnDl645B_Modify(&TxFrm->UDat.DI0 , TxFrm->Len , 1);
	
	TxFrm->UDat.Dat[TxFrm->Len - 4 + 0] = fnSum(&TxFrm->Head1 , TxFrm->Len + 10);
	TxFrm->UDat.Dat[TxFrm->Len - 4 + 1] = 0x16;
	
	return(TxFrm->Len + 12);
}

eDL645B_ErrInfo_TypeDef fnDl645_GetSocMemory(sFrmDl645B_TypeDef *RxFrm,u8 *Dst , u16 *Len)
{
	u32 DI;
	
	*Len = 0;
	DI =0;
	DI = ((u32)RxFrm->UDat.DI2 << 8) + (RxFrm->UDat.DI1 );
	DI = (DI<<8);
    DI = DI +(RxFrm->UDat.DI0);
    DI = DI &0xffffff00;
	
	switch(DI)
	{
		case 0x00f81300:
			*Len = 4;
			memcpy(Dst , (u8 *)&Dl645B_EMUCom->Buf[RxFrm->UDat.DI0] , *Len);
			break;
		case 0x00f91100:
			*Len = 4;
			memcpy(Dst , (u8 *)&Dl645B_RTCCom->Buf[RxFrm->UDat.DI0] , *Len);
			break;
		default:
			return(DL645B_ERRINFO_DATA);
	}
	return(DL645B_ERRINFO_OK);
}

// 获取参数信息文件
eDL645B_ErrInfo_TypeDef fnDl645_GetParaVar(u8 DI2 , u8 DI1 , u8 DI0 , u8 *Dst , u16 *Len)
{
	u32 DI;
	
	*Len = 0;
	DI = 0;
	DI = ((u32)DI2 << 8) + (DI1 );
	DI = (DI<<8);
    DI = DI+(DI0);
	
	switch(DI)
	{
	case 0x00000101:
		if(SysClock.Week == 0x07) Dst[0] = 0;	
		else Dst[0] = SysClock.Week;
		Dst[1] = SysClock.Day; Dst[2] = SysClock.Month; Dst[3] = SysClock.Year;
		*Len = 4;
		break;
	case 0x00000102:
		Dst[0] = SysClock.Second; Dst[1] = SysClock.Minute; Dst[2] = SysClock.Hour;
		*Len = 3;
		break;
	case 0x0000010c:
		if(SysClock.Week == 0x07) Dst[3] = 0;	
		else Dst[3] = SysClock.Week;
		Dst[4] = SysClock.Day; Dst[5] = SysClock.Month; Dst[6] = SysClock.Year;
		Dst[0] = SysClock.Second; Dst[1] = SysClock.Minute; Dst[2] = SysClock.Hour;
		*Len = 7;
		break;
	default:
		return(DL645B_ERRINFO_MISC);
	}
	return(DL645B_ERRINFO_OK);	
}

//---获取当前变量数据------------------------------------------------
eDL645B_ErrInfo_TypeDef fnDl645_GetVar(u8 DI2 , u8 DI1 , u8 DI0 , u8 *Dst , u16 *Len)
{
	u32 DI;
	uDFAll DFAll;
	u16 ItemLen , i;
	
	
	*Len = 0;
    DI = ((u32)DI2 << 8) + (DI1 );
    DI = (DI<<8);
    DI = DI +(DI0);

	switch(DI)
	{
	case 0x00010100: //电压
		DFAll.DF07 = fnDFConver_Bcd16ToDF07(Dl645Front.PubData.U);
		ItemLen = sizeof(sDF07) ; memcpy(Dst , &DFAll.DF07 , ItemLen);
		*Len = ItemLen;
		break;

	case 0x0001FF00: 			
		for(i = 0 ; i < 3 ; i++)
		{
			if(i==0) DFAll.DF07 = fnDFConver_Bcd16ToDF07(Dl645Front.PubData.U);
			else DFAll.DF07.Dat = 0xffff;
			ItemLen = sizeof(sDF07) ; memcpy(Dst , &DFAll.DF07 , ItemLen);
			*Len += ItemLen; Dst += ItemLen;
		}			
		break;
	case 0x00020100:	//电流
		DFAll.DF25 = fnDFConver_Bcd32ToDF25(Dl645Front.PubData.Ia);
		ItemLen = sizeof(sDF25) ; memcpy(Dst , &DFAll.DF25 , ItemLen);
		*Len = ItemLen;
		break;

	case 0x0002FF00:			
		DFAll.DF25 = fnDFConver_Bcd32ToDF25(Dl645Front.PubData.Ia);
		ItemLen = sizeof(sDF25) ; memcpy(Dst , &DFAll.DF25 , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);
		*Len = ItemLen*3;	
		break;
//-----------	
	case 0x00030000:	//瞬时有功
	case 0x00030100:
		ItemLen = sizeof(sDF09) ; memcpy(Dst , &Dl645Front.PubData.Pw , ItemLen);
		*Len += ItemLen; 
		break;

	case 0x0003FF00:
		ItemLen = sizeof(sDF09) ; memcpy(Dst , &Dl645Front.PubData.Pw , ItemLen); 
		Dst += ItemLen;
		memcpy(Dst , &Dl645Front.PubData.Pw , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);
		Dst += ItemLen;
		*Len = ItemLen*4;
		break;

	case 0x00060000:	//功率因数
	case 0x00060100:
		ItemLen = sizeof(sDF26) ; memcpy(Dst , &Dl645Front.PubData.Pf , ItemLen);
		*Len += ItemLen; 
		break;
	case 0x0006FF00:	
		ItemLen = sizeof(sDF26) ; memcpy(Dst , &Dl645Front.PubData.Pf , ItemLen); 
		Dst += ItemLen;
		memcpy(Dst , &Dl645Front.PubData.Pf , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);
		Dst += ItemLen;
		*Len = ItemLen*4;
		break;	
	case 0x00070100:      //相角
		ItemLen = sizeof(sDF26) ; memcpy(Dst , &Dl645Front.PubData.Angle , ItemLen);
		*Len = ItemLen;
		break;
	case 0x0007FF00:
		ItemLen = sizeof(sDF26) ; memcpy(Dst , &Dl645Front.PubData.Angle , ItemLen);
		Dst += ItemLen;		
		memset(Dst , 0xff , ItemLen);
		Dst += ItemLen;
		memset(Dst , 0xff , ItemLen);	
		Dst += ItemLen;		
		*Len = ItemLen*3;
		break;
	case 0x00800001:									// 零线电流
		DFAll.DF25 = fnDFConver_Bcd32ToDF25(Dl645Front.PubData.In);
		ItemLen = sizeof(sDF25) ; memcpy(Dst , &DFAll.DF25 , ItemLen);
		*Len = ItemLen;
		break;	
	case 0x00800002:								// 电网频率
		DFAll.DF33 = fnDFConver_Bcd16ToDF33(Dl645Front.PubData.Frequency);
		ItemLen = sizeof(sDF33) ; memcpy(Dst , &DFAll.DF33 , ItemLen);
		*Len = ItemLen;
		break;
	case 0x00800007:								// 当前表内温度
		ItemLen = sizeof(u16) ; memcpy(Dst , &Dl645Front.PubData.Temperature , ItemLen);
		*Len = ItemLen;
		break;
	case 0x00800008:								// 时钟电池电压
		DFAll.DF33 = fnDFConver_Bcd16ToDF33(Dl645Front.PubData.ClockBat);
		ItemLen = sizeof(sDF33) ; memcpy(Dst , &DFAll.DF33 , ItemLen);
		*Len = ItemLen;
		break;	
	default:
		return(DL645B_ERRINFO_DATA);		
	}	
	return(DL645B_ERRINFO_OK);	
}  

//参数设置
eDL645B_ErrInfo_TypeDef fnDl645_PutParaVar(sFrmDl645B_TypeDef *RxFrm)
{
	u8  i;
	u32 DI;
	u16 tempconst;
	u16	temphfconst;
	u32 tempus,tempis,tempub,tempib;
	sDF10	ConstE;
	float ADErr;
	u32	temppw[2];
	
	if((RxFrm->UDat.Dat[0] == 0x03)&&(RxFrm->UDat.Dat[1] == 0x33)&&(RxFrm->UDat.Dat[2] == 0x22)&&(RxFrm->UDat.Dat[3] == 0x11)) ;
	else return(DL645B_ERRINFO_PSW); 
	
	DI =0;
	DI = ((u32)RxFrm->UDat.DI2 << 8) + (RxFrm->UDat.DI1 );
	DI = (DI<<8);
    DI = DI +(RxFrm->UDat.DI0);
	switch(DI)
	{
	case 0x000101:
		if(RxFrm->Len != (12 + 4) ) return(DL645B_ERRINFO_MISC);
		if(fnDl645B_JudgeClockNoWeek( 3,&RxFrm->UDat.Dat[9] ,3 ) == ERROR) return(DL645B_ERRINFO_MISC); 
		if(RxFrm->UDat.Dat[9] > fnGetDay(RxFrm->UDat.Dat[10] , RxFrm->UDat.Dat[11])) return(DL645B_ERRINFO_MISC); 
		SysClock.Day = RxFrm->UDat.Dat[9];	
		SysClock.Month = RxFrm->UDat.Dat[10];	
		SysClock.Year = RxFrm->UDat.Dat[11];	
		
	    SysClock.Week = fnWeek_Calculate(SysClock.Year , SysClock.Month , SysClock.Day);
	    if(SysClock.Week == 7) SysClock.Week = 0; 
		
		if(fnSysClock_Write(&SysClock) == ERROR) return(DL645B_ERRINFO_MISC);
		Dl645Bkgrd.PubData.fChange.FlagBit.XiugaiClock=1;
		break;
	case 0x000102:
		if(RxFrm->Len != (12 + 3) ) return(DL645B_ERRINFO_MISC);
		if(fnDl645B_JudgeClockNoWeek( 0,&RxFrm->UDat.Dat[8] ,3 ) == ERROR) return(DL645B_ERRINFO_MISC); 		
		SysClock.Second = RxFrm->UDat.Dat[8];	
		SysClock.Minute = RxFrm->UDat.Dat[9];	
		SysClock.Hour = RxFrm->UDat.Dat[10];				
		Dl645Bkgrd.PubData.ClockPiece = 0;
		Dl645Bkgrd.PubData.Second = SysClock.Second;
		if(fnSysClock_Write(&SysClock) == ERROR) return(DL645B_ERRINFO_MISC);      
		Dl645Bkgrd.PubData.fChange.FlagBit.XiugaiClock=1;
		break;
	case 0x00010c:					// 时间块设置
		if(RxFrm->Len != (12 + 7) ) return(DL645B_ERRINFO_MISC);
		if(fnDl645B_JudgeClockNoWeek( 0,&RxFrm->UDat.Dat[8] ,3 ) == ERROR) return(DL645B_ERRINFO_MISC); 
		if(fnDl645B_JudgeClockNoWeek( 3,&RxFrm->UDat.Dat[12] ,3 ) == ERROR) return(DL645B_ERRINFO_MISC); 
		SysClock.Second = RxFrm->UDat.Dat[8];	
		SysClock.Minute = RxFrm->UDat.Dat[9];	
		SysClock.Hour = RxFrm->UDat.Dat[10];
		SysClock.Day = RxFrm->UDat.Dat[12];	
		SysClock.Month = RxFrm->UDat.Dat[13];	
		SysClock.Year = RxFrm->UDat.Dat[14];	
		
	    SysClock.Week = fnWeek_Calculate(SysClock.Year , SysClock.Month , SysClock.Day);
	    if(SysClock.Week == 7) SysClock.Week = 0; 
		Dl645Bkgrd.PubData.ClockPiece = 0;
		Dl645Bkgrd.PubData.Second = SysClock.Second;
		if(fnSysClock_Write(&SysClock) == ERROR) return(DL645B_ERRINFO_MISC);
		break;
	case 0x00f81000:						// A通道校正
		if(RxFrm->Len != (12+24)) return(DL645B_ERRINFO_MISC);
		EMU->SPCMD = 0xe5;			 // 写使能位
		memcpy(&tempis , &(RxFrm->UDat.Dat[24]) , 4);
		if(tempis != 0)		// 标准电压电流方式校准功率
		{
			memcpy(&tempconst , &(RxFrm->UDat.Dat[8]) , 2);
			memcpy(&temphfconst , &(RxFrm->UDat.Dat[10]) , 2);
			memcpy(&tempub , &(RxFrm->UDat.Dat[12]) , 4);
			memcpy(&tempus , &(RxFrm->UDat.Dat[16]) , 4);
			memcpy(&tempib , &(RxFrm->UDat.Dat[20]) , 4);
			memcpy(&tempis , &(RxFrm->UDat.Dat[24]) , 4);
			Dl645FirmPara.HFConst = temphfconst;
			EMU->HFConst = Dl645FirmPara.HFConst;
			fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_HFConst,(u8 *)&Dl645FirmPara.HFConst,2);
			
			temppw[0] = EMU->URMS;
			ADErr=((float)temppw[0]-(float)tempus)/tempus;
			ADErr=(-ADErr/(1+ADErr));
			if(ADErr>0) Dl645FirmPara.UGain=(u16)(ADErr*32768);
			else Dl645FirmPara.UGain = (u16)(65535 + ADErr*32768);
			EMU->UGAIN = Dl645FirmPara.UGain;
	
			
			temppw[0] = EMU->IARMS;
			ADErr=((float)temppw[0]-(float)tempis)/tempis;
			ADErr=((-ADErr)/(1+ADErr));
			if(ADErr>0) Dl645FirmPara.IAGain=(u16)(ADErr*32768);
			else Dl645FirmPara.IAGain = (u16)(65535 + ADErr*32768);
			EMU->IAGAIN = Dl645FirmPara.IAGain;
			fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_IAGAIN,(u8 *)&Dl645FirmPara.IAGain,4);
			
			
			tempconst = fnHexToBcd_u16(tempconst) ;
			ConstE.Dat0 = (((u8)(tempconst & 0x000f))<<4)&0xf0;
			ConstE.Dat1 = (tempconst & 0x0ff0)>>4;
			ConstE.Dat2 = (u8)((tempconst & 0xf000)>>12);
			
			fnDl645File_Write(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_PConstE , (u8 *)&ConstE , 3); 
			
			Dl645FirmPara.KIArms=(float)tempis/((float)tempib);
			Dl645FirmPara.KUrms=(float)tempus*100/(tempub);
			// 功率系数计算
			ADErr = ((float)tempus*(float)tempis)/32768 ;
			Dl645FirmPara.PStart = (u16)((ADErr*0.003)/256);
			EMU->PStart 	= 	Dl645FirmPara.PStart;
			Dl645FirmPara.KPrms=(ADErr/((float)tempub*(float)tempib*0.0000001));
		}
		else			// 误差方式校准功率
		{
			memcpy(&tempconst , &(RxFrm->UDat.Dat[8]) , 2);
			memcpy(&temphfconst , &(RxFrm->UDat.Dat[10]) , 2);
			memcpy(&tempub , &(RxFrm->UDat.Dat[12]) , 4);
			memcpy(&tempib , &(RxFrm->UDat.Dat[20]) , 4);
			memcpy(&tempis , &(RxFrm->UDat.Dat[28]) , 4);
			Dl645FirmPara.HFConst = temphfconst;
			EMU->HFConst = Dl645FirmPara.HFConst;
			fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_HFConst,(u8 *)&Dl645FirmPara.HFConst,2);
			
			if(tempis > 0x7fffffff) ADErr=-(float)(0xffffffff - tempis)/100000;
        	else ADErr=(float)tempis/100000;
			ADErr=((-ADErr)/(1+ADErr));
			if(ADErr>=0) Dl645FirmPara.GPQA = (u16)(ADErr*32768);
			else Dl645FirmPara.GPQA = (u16)(65535 + ADErr*32768);
			EMU->GPQA = Dl645FirmPara.GPQA;
			fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_GPQA,(u8 *)&Dl645FirmPara.GPQA,2);
			tempis = EMU->IARMS;
			Dl645FirmPara.KIArms=(float)tempis/((float)tempib);
			tempus = EMU->URMS;
			Dl645FirmPara.KUrms=(float)tempus*100/(tempub);
			
			ADErr =  ((float)temphfconst*4*(float)tempconst*10*(float)tempub*(float)tempib*21.47483648)/(3.6*1000000000000*1.8432);
			Dl645FirmPara.PStart = (u16)((ADErr*0.003)/256);
			EMU->PStart 	= 	Dl645FirmPara.PStart;
			Dl645FirmPara.KPrms=(ADErr/((float)tempub*(float)tempib*0.0000001));
			
			tempconst = fnHexToBcd_u16(tempconst) ;
			ConstE.Dat0 = (((u8)(tempconst & 0x000f))<<4)&0xf0;
			ConstE.Dat1 = (tempconst & 0x0ff0)>>4;
			ConstE.Dat2 = (u8)((tempconst & 0xf000)>>12);
			
			fnDl645File_Write(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_PConstE , (u8 *)&ConstE , 3); 
		}
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_KUrms,(u8 *)&Dl645FirmPara.KUrms,16);
		Dl645FirmPara.ChkSum = 0;
		SystemDelay(1);
		Dl645FirmPara.ChkSum = EMU->EMUStatus; 
		Dl645FirmPara.ChkSum &=0x00ffffff;
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_ChkSum,(u8 *)&Dl645FirmPara.ChkSum,4);
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_PStart,(u8 *)&Dl645FirmPara.PStart,2);
		EMU->SPCMD = 0xdc; 			// 写保护	
		break;
	case 0x00f81100:					    // A通道相位校正
		if(RxFrm->Len != (12+12)) return(DL645B_ERRINFO_MISC);
		EMU->SPCMD = 0xe5;			 // 写使能位
		memcpy(&tempub , &(RxFrm->UDat.Dat[8]) , 4);
		memcpy(&tempus , &(RxFrm->UDat.Dat[12]) , 4);
		memcpy(&tempis , &(RxFrm->UDat.Dat[16]) , 4);
		if(tempub != 0 )
		{
			//Dl645FirmPara.KPrms=(float)tempus/(tempub);
			temppw[0] = EMU->PowerPA;
			ADErr=((float)temppw[0]-(float)tempus)/tempus;
			ADErr= (asin(-ADErr/1.732))*100*57.29578;				// 弧度转成角度
			if(ADErr>0) Dl645FirmPara.PhsA=(u16)(ADErr);
			else Dl645FirmPara.PhsA = (u16)(512 + ADErr);
		}
		else								// 使用误差对相位进行校正
		{
			if(tempis > 0x7fffffff) ADErr=-(float)(0xffffffff - tempis)/100000;
        	else ADErr=(float)tempis/100000;
			ADErr = (asin(-ADErr/1.732))*100*57.29578;				// 弧度转成角度
			if(ADErr>0) Dl645FirmPara.PhsA=(u16)(ADErr);
			else Dl645FirmPara.PhsA = (u16)(512 + ADErr);
		}
		EMU->PhsA = Dl645FirmPara.PhsA;
		EMU->SPCMD = 0xdc; 			// 写保护	
		Dl645FirmPara.ChkSum = 0;
		SystemDelay(1);
		Dl645FirmPara.ChkSum = EMU->EMUStatus; 
		Dl645FirmPara.ChkSum &=0x00ffffff;
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_ChkSum,(u8 *)&Dl645FirmPara.ChkSum,4);
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_PhsA,(u8 *)&Dl645FirmPara.PhsA,2);
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_KPrms,(u8 *)&Dl645FirmPara.KPrms,4);
		break;
	case 0x00f81200:						// 小电流校正
		if(RxFrm->Len != (12+6)) return(DL645B_ERRINFO_MISC);
		EMU->SPCMD = 0xe5;			 // 写使能位
		memcpy(&tempus , &(RxFrm->UDat.Dat[8]) , 4);
		memcpy(&tempis , &(RxFrm->UDat.Dat[12]) , 2);
		if(tempus == 0 )			// 以输入的误差校正小电流
		{
			EMU->APOSA = tempis;
			Dl645FirmPara.APOSA=tempis;
		}
		else
		{
			temppw[0] = EMU->PowerPA;
			for(i=0;i<3;i++)
			{
				SystemDelay(250);
				temppw[1] = EMU->PowerPA;
				temppw[0] = (temppw[0] + temppw[1])/2;
			}
			ADErr=((float)temppw[0]-(float)tempus)/tempus;
			ADErr= -ADErr*tempus;
			if(ADErr>0) Dl645FirmPara.APOSA=(u16)(ADErr);
			else Dl645FirmPara.APOSA = (u16)(65535 + ADErr);
			EMU->APOSA = Dl645FirmPara.APOSA;
		}
		
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_APOSA,(u8 *)&Dl645FirmPara.APOSA,2);
		Dl645FirmPara.ChkSum = 0;
		SystemDelay(1);
		Dl645FirmPara.ChkSum = EMU->EMUStatus; 
		Dl645FirmPara.ChkSum &=0x00ffffff;
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_ChkSum,(u8 *)&Dl645FirmPara.ChkSum,4);
		EMU->SPCMD = 0xdc; 			// 写保护	
		break;
	case 0x00f81300:				// B通道电流校正
		if(RxFrm->Len != (12+12)) return(DL645B_ERRINFO_MISC);
		memcpy(&tempib , &(RxFrm->UDat.Dat[8]) , 4);
		tempis = EMU->IBRMS;
		Dl645FirmPara.KIBrms=(float)tempis/((float)tempib);
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_KIBrms,(u8 *)&Dl645FirmPara.KIBrms,4);	
		break;
	case 0x00f81400:				// 对寄存器读写操作
		tempis = 0;
		memcpy(&tempis ,&(RxFrm->UDat.Dat[8]) ,(RxFrm->Len - 13)  );
		EMU->SPCMD = 0xe5;			 // 写使能位
		Dl645B_EMUCom->Buf[RxFrm->UDat.Dat[10]] = tempis;
		EMU->SPCMD = 0xe5; 			// 写保护
		break;
	case 0x00f81500:				// 电流通道A offset校正
		if(RxFrm->Len != (12+3)) return(DL645B_ERRINFO_MISC);
		tempus = 0;
		EMU->SPCMD = 0xe5;			 // 写使能位
		if(RxFrm->UDat.Dat[8]==0x00)
		{
			memcpy(&tempus , &(RxFrm->UDat.Dat[9]) , 2);
			EMU->IARMSOS = tempus;
			Dl645FirmPara.IARMSOS=tempus;
			fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_IARMSOS,(u8 *)&Dl645FirmPara.IARMSOS,2);
		}
		else
		{
			memcpy(&tempus , &(RxFrm->UDat.Dat[9]) , 2);
			EMU->IBRMSOS = tempus;
			Dl645FirmPara.IBRMSOS=tempus;
			fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_IBRMSOS,(u8 *)&Dl645FirmPara.IBRMSOS,2);
		}
		Dl645FirmPara.ChkSum = 0;
		SystemDelay(1);
		Dl645FirmPara.ChkSum = EMU->EMUStatus; 
		Dl645FirmPara.ChkSum &=0x00ffffff;
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_ChkSum,(u8 *)&Dl645FirmPara.ChkSum,4);
		EMU->SPCMD = 0xdc; 			// 写保护
		break;
	case 0x00f81600:						// 清零
		if((RxFrm->Len != (12 + 1)) && (RxFrm->UDat.Dat[8] != 0x5A)) return(DL645B_ERRINFO_MISC);
		EMU->SPCMD = 0xe5;			 // 写使能位
		Dl645FirmPara.UGain =0;
		Dl645FirmPara.IAGain =0;
		Dl645FirmPara.IBGain =0xbfff;
		Dl645FirmPara.PhsA =0;
		Dl645FirmPara.PhsB =0;
		Dl645FirmPara.APOSA=0;
		Dl645FirmPara.APOSB=0;
		Dl645FirmPara.GPQA =0;
		Dl645FirmPara.GPQB=0;
		Dl645FirmPara.PStart=0x60;
		Dl645FirmPara.QStart=0x0120;
		Dl645FirmPara.IARMSOS = 0;
		Dl645FirmPara.IBRMSOS = 0;
		EMU->PStart = 0x60;
		EMU->QStart = 0x0120;
		EMU->UGAIN = 0;
		EMU->IAGAIN = 0;
		EMU->IBGAIN = 0xbfff;
		EMU->PhsA = 0;
		EMU->PhsB = 0;
		EMU->APOSA = 0;
		EMU->APOSB = 0;
		EMU->GPQA = 0;
		EMU->GPQB = 0;
		EMU->IARMSOS 	= 0;
		EMU->IBRMSOS 	= 0;
		EMU->SPCMD = 0xdc; 			// 写保护								
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_PStart,(u8 *)&Dl645FirmPara.PStart,22);
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_IARMSOS,(u8 *)&Dl645FirmPara.IARMSOS,4);
		Dl645FirmPara.ChkSum = 0;
		SystemDelay(1);
		Dl645FirmPara.ChkSum = EMU->EMUStatus; 
		Dl645FirmPara.ChkSum &=0x00ffffff;
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_ChkSum,(u8 *)&Dl645FirmPara.ChkSum,4);
		break;
	case 0x00f81601:					// B通道清零
		if((RxFrm->Len != (12 + 1)) && (RxFrm->UDat.Dat[8] != 0x5A)) return(DL645B_ERRINFO_MISC);
		EMU->SPCMD = 0xe5;			 // 写使能位
		Dl645FirmPara.IBGain =0;
		Dl645FirmPara.PhsB =0;
		Dl645FirmPara.APOSB=0;
		Dl645FirmPara.GPQB =0;
		EMU->IBGAIN = 0;
		EMU->PhsB = 0;
		EMU->APOSB = Dl645FirmPara.APOSB;
		EMU->GPQB = 0;
		EMU->SPCMD = 0xe5; 			// 写保护								
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_GPQA,(u8 *)&Dl645FirmPara.GPQA,14);
		Dl645FirmPara.ChkSum = 0;
		SystemDelay(1);
		Dl645FirmPara.ChkSum = EMU->EMUStatus; 
		Dl645FirmPara.ChkSum &=0x00ffffff;
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_ChkSum,(u8 *)&Dl645FirmPara.ChkSum,4);
		break;
	case 0x00f91100:				// 设置RTC的DOTA0
		if(RxFrm->Len != (12 + 2)) return(DL645B_ERRINFO_MISC);
		memcpy((u8 *)&Dl645FirmPara.RTCDota0 ,&(RxFrm->UDat.Dat[8]) ,2 );
		RtcWriteDota(Dl645FirmPara.RTCDota0);
		fnDl645File_Write(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_RTCDota0,(u8 *)&Dl645FirmPara.RTCDota0,2);
		break;
	default:
		return(DL645B_ERRINFO_MISC);
	}
		
	return(DL645B_ERRINFO_OK);
}

u16 fnDl645B_Unpack11(u8 ComPort , sFrmDl645B_TypeDef *RxFrm)	//读数据
{
	eDL645B_ErrInfo_TypeDef 	err;
	u16 Len;
	
	sFrmDl645B_TypeDef *TxFrm;
	TxFrm = (sFrmDl645B_TypeDef *)&ComPack[ComPort].TxBuf[DL645B_LEADERCODE_LEN];
	
	if(RxFrm->Len == 4)
	{
		switch(RxFrm->UDat.DI3)
		{
		case 0x02: err = fnDl645_GetVar(RxFrm->UDat.DI2 , RxFrm->UDat.DI1 , RxFrm->UDat.DI0 , &TxFrm->UDat.Dat[0] , &Len); break;
		case 0x04: err = fnDl645_GetParaVar(RxFrm->UDat.DI2 , RxFrm->UDat.DI1 , RxFrm->UDat.DI0 , &TxFrm->UDat.Dat[0] , &Len); break;
		case 0x06: err = fnDl645_GetSocMemory(RxFrm,&TxFrm->UDat.Dat[0] , &Len); break;
		default: err = DL645B_ERRINFO_DATA; Len = 0; break;	
		}
	}
	else
	{
		err = DL645B_ERRINFO_DATA; Len = 0;
	}
	if(err == DL645B_ERRINFO_OK) Len = fnDl645_PackOk(TxFrm , RxFrm , Len + 4);
	else Len = fnDl645_PackErr(TxFrm , RxFrm , err);		
	return(Len);
}

u16 fnDl645B_Unpack13(u8 ComPort , sFrmDl645B_TypeDef *RxFrm) 	//读通信地址
{
	u8 i;
	u16 Len;
	sFrmDl645B_TypeDef *TxFrm;
	TxFrm = (sFrmDl645B_TypeDef *)&ComPack[ComPort].TxBuf[DL645B_LEADERCODE_LEN];

	for(i = 0 ; i < 6 ; i++)
	{
		if(RxFrm->Addr[i] != 0xAA) return(0);
	}	
	if(RxFrm->Len != 0) return(0);
	
	fnDl645File_Read(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_ComAddr , &TxFrm->UDat.DI0 , 6);	
	Len = fnDl645_PackNoDI(TxFrm , RxFrm , 6);
	return(Len);
}

u16 fnDl645B_Unpack14(u8 ComPort , sFrmDl645B_TypeDef *RxFrm)	//写参数
{
	eDL645B_ErrInfo_TypeDef 	err;
	u16 Len;
	sFrmDl645B_TypeDef *TxFrm;
	
	TxFrm = (sFrmDl645B_TypeDef *)&ComPack[ComPort].TxBuf[DL645B_LEADERCODE_LEN];
	switch(RxFrm->UDat.DI3)
	{
	case 0x04 :
		err = fnDl645_PutParaVar(RxFrm);
		break;
	default:
		err = DL645B_ERRINFO_DATA;
		break;
	}
	Len = fnDl645_PackErr( TxFrm , RxFrm , err);
	return(Len);
}

u16 fnDl645B_Unpack15(u8 ComPort , sFrmDl645B_TypeDef *RxFrm)	//写通信地址
{
	u8 i;
	u16 Len;
	sFrmDl645B_TypeDef *TxFrm;
	TxFrm = (sFrmDl645B_TypeDef *)&ComPack[ComPort].TxBuf[DL645B_LEADERCODE_LEN];
	
	for(i = 0 ; i < 6 ; i++)
	{
		if(RxFrm->Addr[i] != 0xAA) return(0);
	}	
	if(RxFrm->Len != 6) return(0);
	if(fnDl645B_FramCheckBCD(&RxFrm->UDat.DI0,6)== ERROR) return(fnDl645_PackErr(TxFrm , RxFrm , DL645B_ERRINFO_DATA) );
	fnDl645File_Write(Dl645FileId_HighPara , Dl645FileItemInfoOffAddr_HighPara_ComAddr , &RxFrm->UDat.DI0 , 6);	
	Len = fnDl645_PackErr(TxFrm , RxFrm , DL645B_ERRINFO_OK);
	return(Len);
}

u16 fnDl645B_RxAnalyse(u8 ComPort , sFrmDl645B_TypeDef *RxFrm)	
{
	u16 Len;
	
	if(RxFrm->Dir) return(0);
	
	switch(RxFrm->AFN)
	{	
	case 0x11:	Len = fnDl645B_Unpack11(ComPort , RxFrm); break;			
	case 0x13:	Len = fnDl645B_Unpack13(ComPort , RxFrm); break;	
	case 0x14:	Len = fnDl645B_Unpack14(ComPort , RxFrm); break;	
	case 0x15:	Len = fnDl645B_Unpack15(ComPort , RxFrm); break;			
	default: Len = 0 ; break;			
	}
	return(Len);
}

u16 fnDl645B_Exec(u8 ComPort , sFrmDl645B_TypeDef *RxFrm)
{
	u16 Len;
	fnDl645B_Modify( (u8 *)&RxFrm->UDat , RxFrm->Len , 0);
	Len = fnDl645B_RxAnalyse(ComPort , RxFrm);
	return(Len);	
}

