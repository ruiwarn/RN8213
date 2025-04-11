#define  DRIVE_ISO7816_GLOBALS
#include <Drive_Lib.h>
//********************************************************************************* 
// �������� ErrorStatus fnCardGetByte(u8 CH, u8 Rpt, u8 *Data )
// ���룺   Data ΪҪ���յ�8λ����  ,CH ͨ��ѡ�� Rpt ���½��մ���
// ���أ�   0Ϊ��ȷ��1Ϊ����ȷ 
// ������   �ӿ���ESAM����һ��8λ�ֽ� 
// ע�ͣ�   ���ճɹ���ȴ�1��ETU���أ��Ӽ�⵽��ʼλ���ɹ�����ʱ������11��ETU 
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
// �������� ErrorStatus fnCardGetByte(u8 CH, u8 Rpt, u8 *Data )
// ���룺   Data ΪҪ���յ�8λ����  ,CH ͨ��ѡ�� Rpt ���½��մ���
// ���أ�   0Ϊ��ȷ��1Ϊ����ȷ 
// ������   �ӿ���ESAM����һ��8λ�ֽ� 
// ע�ͣ�   ���ճɹ���ȴ�1��ETU���أ��Ӽ�⵽��ʼλ���ɹ�����ʱ������11��ETU 
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
			if(fnStamp_Through(Stamptemp)>500)  {ISO7816->STA1 = 0x3f;return ERROR;}		// ��ֹ��ѭ��
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
// �������� ErrorStatus fnCardSendByte(u8 CH, u8 Rpt, u8 *Data)  
// ���룺   c ΪҪ���͵�8λ���� 
// ���أ�   0Ϊ��ȷ��1Ϊ����ȷ 
// ������   �򿨻�ESAM����һ��8λ�ֽ� 
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
		if((ISO7816->STA0 & 0x20)||(ISO7816->STA0 & 0x08))			// �жϷ��ʹ����־
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
			if(fnStamp_Through(Stamptemp)>500)  {ISO7816->STA1 = 0x3f;return ERROR;}		// ��ֹ��ѭ��
			fnWDT_Restart();
		}
		if((ISO7816->STA1 & 0x20)||(ISO7816->STA1 & 0x08))			// �жϷ��ʹ����־
		{
			ISO7816->STA1 = 0x3f;			// clear err flag
			return ERROR;
		}
		ISO7816->STA1 = 0x3f;
	}
	return	SUCCESS;
}

