#define  _RTCDrive_GLOBALS
#include <Drive_Lib.h>


//�������ܣ�RTC��ȡ����
//��ڲ�����Dst��ȡ���ݻ�����Ŀ��ָ��
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnRTCDrive_Read(u8 *Dst)
{
	Dst[0]=RTC->SC;    
	Dst[1]=RTC->MN;    
	Dst[2]=RTC->HR;    
	Dst[4]=RTC->DT;    
	Dst[5]=RTC->MO;    
	Dst[6]=RTC->YR;    
	Dst[3]=RTC->DW;
	return(SUCCESS);
}

//�������ܣ�RTCд������
//��ڲ�����Dstд��������Դָ�룬Repeatʧ���ظ�����
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnRTCDrive_Write(u8 *Dst , u8 Repeat)
{
	u8 Buffer[7],i;
	RTC->CTRL |= 0x00000100;		//RTC Write enable
	for(;Repeat>0;Repeat--)
	{
		if(!(RTC->CTRL & 0x200)) break;
		fnRTCDrive_Delay();
	}
	if(RTC->CTRL & 0x200) return(ERROR);
	RTC->DW = Dst[3];
	fnRTCDrive_Delay();  
	RTC->YR = Dst[6];
	fnRTCDrive_Delay(); 
	RTC->MO = Dst[5];
	fnRTCDrive_Delay(); 
	RTC->DT = Dst[4];
	fnRTCDrive_Delay(); 
	RTC->HR = Dst[2];
	fnRTCDrive_Delay(); 
	RTC->MN = Dst[1];
	fnRTCDrive_Delay(); 
	RTC->SC = Dst[0];
	fnRTCDrive_Delay(); 
	if(!fnRTCDrive_Read(Buffer))return(ERROR);
	for(i=6;i>0;i--)
	{
		if(Buffer[i]!=Dst[i]) 
			return(ERROR);
	}
	RTC->CTRL &= 0xfffffeff;      // RTC Write Disable
	return(SUCCESS);
}


//�������ܣ�RTC��ʼ������
//��ڲ�������
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnRTCDrive_Init(void)
{
	u16 AddingTimes=0;
	RTC->CTRL = 0x100;
	while(RTC->CTRL & 0x200)
	{
		if(++AddingTimes>65000)return(ERROR); //��ֹ��ѭ��
	}
	RTC->CTRL = 0x100;
	fnRTCDrive_Delay(); 
	RTC->CTRL = 0x101;
	fnRTCDrive_Delay(); 

	return(SUCCESS);
}

//�������ܣ�ϵͳʱ�Ӷ�ȡ
//��ڲ�����ClockĿ��ָ��
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnSysClock_Read(sDF01 *Clock)
{
	u8 Buffer[7];
	if(!fnRTCDrive_Read(Buffer))return(ERROR);
	Clock->Second=(Buffer[0])& 0x7f;    
	Clock->Minute=(Buffer[1])& 0x7f;    
	Clock->Hour=(Buffer[2]) & 0x3f;    
	Clock->Day=(Buffer[4]) & 0x3f;    
	Clock->Month=(Buffer[5]) & 0x1f;    
	Clock->Year=(Buffer[6]);    
	Clock->Week=(Buffer[3])&0x07;
	return(SUCCESS);
}

//�������ܣ�ϵͳʱ��д��
//��ڲ�����ClockԴָ��
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnSysClock_Write(sDF01 *Clock)
{
	u8 Buffer[7];
	Buffer[0]=(Clock->Second)& 0x7f;
	Buffer[1]=(Clock->Minute)& 0x7f;
	Buffer[2]=(Clock->Hour) & 0x3f;
	Buffer[3]=(Clock->Week)&0x07;
	Buffer[4]=(Clock->Day) & 0x3f;
	Buffer[5]=(Clock->Month) & 0x1f;
	Buffer[6]=(Clock->Year);
	return(fnRTCDrive_Write(Buffer, 3));
}






