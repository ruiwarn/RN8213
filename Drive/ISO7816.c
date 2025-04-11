#define  DRIVE_ISO7816_GLOBALS
#include <Drive_Lib.h>
//********************************************************************************* 
// 函数名： ErrorStatus fnCardGetByte(u8 CH, u8 Rpt, u8 *Data )
// 输入：   Data 为要接收的8位数据  ,CH 通道选择 Rpt 重新接收次数
// 返回：   0为正确，1为不正确 
// 描述：   从卡或ESAM接收一个8位字节 
// 注释：   接收成功后等待1个ETU返回，从检测到启始位到成功返回时长大于11个ETU 
//********************************************************************************* 
void fnISO7816_Init(void)
{
  	#if(METERTYPE)
  	ISO7816->CTRL1 = 0x70000201;
  	#endif
    ISO7816->CTRL0 = 0x0800ea01;
    //ISO7816->CTRL0 = 0x0000ea01;
    ISO7816->STA0 = 0x3f; 					//clear status
  	ISO7816->STA1 = 0x3f; 					//clear status
  	//ISO7816->BDDIV0 = 0X216a00;
  	PinWrite_EsamRst(1);
  	memset(&I7816ComExt,0x00,sizeof(sISO7816ComBuf_TypeDef));
}

//********************************************************************************* 
// 函数名： ErrorStatus fnCardGetByte(u8 CH, u8 Rpt, u8 *Data )
// 输入：   Data 为要接收的8位数据  ,CH 通道选择 Rpt 重新接收次数
// 返回：   0为正确，1为不正确 
// 描述：   从卡或ESAM接收一个8位字节 
// 注释：   接收成功后等待1个ETU返回，从检测到启始位到成功返回时长大于11个ETU 
//********************************************************************************* 
ErrorStatus fnCardGetByte(u8 CH, u8 *Data )
{
	u16	Stamptemp;
	Stamptemp = SysStamp;
	if(CH == ESAM)
	{
		ISO7816->CTRL0 &=0xfffffffb;
	  	ISO7816->CTRL0 |=0x08;
	  	ISO7816->STA0 = 0x3f;
		for(Stamptemp=0;Stamptemp<65530;Stamptemp++)
		{
			if(ISO7816->STA0 & 0x100) break;
			fnDelay_1us();
			fnDelay_1us();
			fnDelay_1us();
			fnDelay_1us();
		}
		if(Stamptemp >= 65530) {ISO7816->STA0 = 0x3f;return ERROR;}
		if(ISO7816->STA0 & 0x10)
		{
			ISO7816->STA0 = 0x3f;			// clear err flag
			return ERROR;
		}
		*Data = ISO7816->DATA0;
		ISO7816->STA0 = 0x3f;
	}
	else
	{
		ISO7816->CTRL1 &=0xfffffffb;
	  	ISO7816->CTRL1 |=0x08;
	  	ISO7816->STA1 = 0x3f;
		
		while(!(ISO7816->STA1 & 0x100))
		{
			if(fnStamp_Through(Stamptemp)>500)  {ISO7816->STA1 = 0x3f;return ERROR;}		// 防止死循环
			fnWDT_Restart();
		}
		if(ISO7816->STA1 & 0x10)
		{
			ISO7816->STA1 = 0x3f;			// clear err flag
			return ERROR;
		}
		*Data = ISO7816->DATA1;
		ISO7816->STA1 = 0x3f;
	}
	return	SUCCESS;
} 


//********************************************************************************* 
// 函数名： ErrorStatus fnCardSendByte(u8 CH, u8 Rpt, u8 *Data)  
// 输入：   c 为要发送的8位数据 
// 返回：   0为正确，1为不正确 
// 描述：   向卡或ESAM发送一个8位字节 
//********************************************************************************* 
ErrorStatus fnCardSendByte(u8 CH , u8 Data) 
{
	u16	Stamptemp;
	Stamptemp = SysStamp;
	if(CH == ESAM)
	{
	  	ISO7816->CTRL0 &=0xfffffff7;
	  	ISO7816->CTRL0 |=0x04;
	  	ISO7816->STA0 = 0x3f;
	  	ISO7816->DATA0 = Data;
		for(Stamptemp=0;Stamptemp<65530;Stamptemp++)
		{
			if(!(ISO7816->STA0 & 0x40)) break;
			fnDelay_1us();
			fnDelay_1us();
			fnDelay_1us();
			fnDelay_1us();
		}
		if(Stamptemp >= 65530) {ISO7816->STA0 = 0x3f;return ERROR;}
		if((ISO7816->STA0 & 0x20)||(ISO7816->STA0 & 0x08))			// 判断发送错误标志
		{
			ISO7816->STA0 = 0x3f;			// clear err flag
			return ERROR;
		}
		ISO7816->STA0 = 0x3f;
	}
	else
	{
		ISO7816->CTRL1 &=0xfffffff7;
	  	ISO7816->CTRL1 |=0x04;
	  	ISO7816->STA1 = 0x3f;
	  	ISO7816->DATA1 = Data;
		while(ISO7816->STA1 & 0x40)
		{
			if(fnStamp_Through(Stamptemp)>500)  {ISO7816->STA1 = 0x3f;return ERROR;}		// 防止死循环
			fnWDT_Restart();
		}
		if((ISO7816->STA1 & 0x20)||(ISO7816->STA1 & 0x08))			// 判断发送错误标志
		{
			ISO7816->STA1 = 0x3f;			// clear err flag
			return ERROR;
		}
		ISO7816->STA1 = 0x3f;
	}
	return	SUCCESS;
}

