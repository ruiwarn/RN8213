#define  _FM24CXX_GLOBALS
#include <Drive_Lib.h>


/*
**-----------------------------------------------------------------------------
**EEPROM存储器操作子程序
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
Address~AAddress+Length-1不可跨节，每节64Bytes
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
//函数功能：I2C存储器读取数据，在此实现跨页操作
//入口参数：Dst取数缓冲区目标指针，Src取数源地址，DatLen要取的数据长度，Repeat失败重复次数
//出口参数：SUCCESS成功，ERROR失败
ErrorStatus fnFM24CXX_Read(u8 *Dst , u32 Src , u32 DatLen , u8 Repeat)
{
	u16 AddressL,AddressH;
	u8 B;
	u16 AddingTimes=0;
	
    if (DatLen==0) return(ERROR);
	if ( EEPROM_MAXSize <= (Src+DatLen))  return(ERROR);

	while (DatLen)
	{
		if(++AddingTimes>600)return(ERROR); //防止死循环
		AddressH=(u16)(Src>>16);
		AddressL=(u16)Src;
		if ((((~AddressL)&EEPROM_PageSize)+1)>=DatLen)/*与运算其实相当于求模运算，该语句保证从NVRAM中每一页的任意点开始*/
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
//函数功能：I2C存储器写入数据，在此实现跨页操作
//入口参数：Dst写入数据的目标地址，Src写数缓冲区源指针，DatLen要写的数据长度，Repeat失败重复次数
//出口参数：SUCCESS成功，ERROR失败
ErrorStatus fnFM24CXX_Write(u32 Dst , u8 *Src , u32 DatLen , u8 Repeat)
{
	u16 AddressL,AddressH;
	u8 B;
	u16 AddingTimes=0;
	
    if (DatLen==0) return(ERROR);
	if ( EEPROM_MAXSize <= (Dst+DatLen))  return(ERROR);

	while (DatLen)
	{
		if(++AddingTimes>600)return(ERROR); //防止死循环
			
		AddressH=(u16)(Dst>>16);
		AddressL=(u16)Dst;
		if ((((~AddressL)&EEPROM_PageSize)+1)>=DatLen)/*与运算其实相当于求模运算，该语句保证从NVRAM中每一页的任意点开始*/
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







