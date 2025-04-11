#define  _FM24CXX_GLOBALS
#include <Drive_Lib.h>


/*
**-----------------------------------------------------------------------------
**EEPROM�洢�������ӳ���
**-----------------------------------------------------------------------------
*/

bool fnCheckNVRAM(u8 DeviceNo,u16 Address,u8 *Buff,u8 Length)
{
	u8 i;
	u8 Data;

	for(i=0;i<Length;i++){
		Data=*(Read_EEPROM + Address + i);    
		if(Data!=Buff[i]){return false;}
	}
	return true;
}
//---------------------------------------------------
/************************
Address~AAddress+Length-1���ɿ�ڣ�ÿ��64Bytes
*************************/
ErrorStatus fnOperateNVRAM(BOOL Read_Write,u8 DeviceNo,u16 Address,u8 *Buff,u8 Length,u8 Repeat)
{
	u8 i;
	u8 ReReadTimes;
	if (Length==0) {return ERROR;}
	
	
	for(ReReadTimes=0;ReReadTimes<=Repeat;ReReadTimes++)
	{
		if(Read==Read_Write)
		{
			for(i=0;i<Length;i++)
			{
				*(Buff+i) = *(Read_EEPROM + Address + i);
			}
		}
		else
		{
			i = eepromProgram(EEPROM_MEMORY_BASE + Address , (uint32_t)Buff , Length);
			if(i) continue;
		}
		if((fnCheckNVRAM(DeviceNo,Address,Buff,Length)==true)||(ReReadTimes>Repeat)) {break; }
	}
 	
 	if(ReReadTimes>=Repeat){return ERROR;}
 	return(SUCCESS);
}

//---------------------------------------------------
//�������ܣ�I2C�洢����ȡ���ݣ��ڴ�ʵ�ֿ�ҳ����
//��ڲ�����Dstȡ��������Ŀ��ָ�룬Srcȡ��Դ��ַ��DatLenҪȡ�����ݳ��ȣ�Repeatʧ���ظ�����
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnFM24CXX_Read(u8 *Dst , u32 Src , u32 DatLen , u8 Repeat)
{
	u16 AddressL,AddressH;
	u8 B;
	u16 AddingTimes=0;
	
    if (DatLen==0) return(ERROR);
	if ( EEPROM_MAXSize <= (Src+DatLen))  return(ERROR);

	while (DatLen)
	{
		if(++AddingTimes>600)return(ERROR); //��ֹ��ѭ��
		AddressH=(u16)(Src>>16);
		AddressL=(u16)Src;
		if ((((~AddressL)&EEPROM_PageSize)+1)>=DatLen)/*��������ʵ�൱����ģ���㣬����䱣֤��NVRAM��ÿһҳ������㿪ʼ*/
		{B=DatLen;}
		else
		{B=((~AddressL)&EEPROM_PageSize)+1;}

		if(ERROR==fnOperateNVRAM(Read,AddressH,AddressL,Dst,B,Repeat))
		{ 
			return(ERROR);
		}
		Dst+=B;
		Src+=B;
		DatLen-=B;
	}
	return(SUCCESS);
}

//---------------------------------------------------
//�������ܣ�I2C�洢��д�����ݣ��ڴ�ʵ�ֿ�ҳ����
//��ڲ�����Dstд�����ݵ�Ŀ���ַ��Srcд��������Դָ�룬DatLenҪд�����ݳ��ȣ�Repeatʧ���ظ�����
//���ڲ�����SUCCESS�ɹ���ERRORʧ��
ErrorStatus fnFM24CXX_Write(u32 Dst , u8 *Src , u32 DatLen , u8 Repeat)
{
	u16 AddressL,AddressH;
	u8 B;
	u16 AddingTimes=0;
	
    if (DatLen==0) return(ERROR);
	if ( EEPROM_MAXSize <= (Dst+DatLen))  return(ERROR);

	while (DatLen)
	{
		if(++AddingTimes>600)return(ERROR); //��ֹ��ѭ��
			
		AddressH=(u16)(Dst>>16);
		AddressL=(u16)Dst;
		if ((((~AddressL)&EEPROM_PageSize)+1)>=DatLen)/*��������ʵ�൱����ģ���㣬����䱣֤��NVRAM��ÿһҳ������㿪ʼ*/
		{B=DatLen;}
		else
		{B=((~AddressL)&EEPROM_PageSize)+1;}

		if(ERROR==fnOperateNVRAM(Write,AddressH,AddressL,Src,B,Repeat))
		{
			return(ERROR);
		}
		Dst+=B;
		Src+=B;
		DatLen-=B;
	}
	return(SUCCESS);
}







