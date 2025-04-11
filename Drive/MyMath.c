
#define  _MYMATH_GLOBALS
#include <Drive_Lib.h>


u8	fnSum(u8 *Dat , u16 Len)
{
	u8 	Sum;
	u16 i;
	
	for(Sum = 0 , i = 0 ; i < Len ; i++)
	{
		Sum += Dat[i];	
	}	
	return(Sum);
}

//根据年月日计算星期几
//返回：0无效、1星期一、2星期二、3星期三、4星期四、5星期五、6星期六、7星期日
u8 fnWeek_Calculate(u8 Year , u8 Month , u8 Day)
{
	Year = fnBcdToHex_u8(Year) + 2000;
	Month = fnBcdToHex_u8(Month);
	Day = fnBcdToHex_u8(Day);
	
	if(Month <= 2)
	{
		Year -=1;
		Month += 12;
	}		
	return( (Day + 2 * Month + 3 * (Month + 1) / 5 + Year + Year / 4 - Year / 100 + Year / 400 + 1) % 7 + 1);	
}



u8 fnHexToBcd_u8(u8 Dat)
{
	u8 Result = 0;
	
	Dat = Dat % 100;
	Result += (Dat / 10) * 0x10;
	Dat = Dat % 10;	
	Result += Dat;
	
	return(Result);	
}

u16 fnHexToBcd_u16(u16 Dat)
{
	u16 Result = 0;

	Dat = Dat % 10000;
	Result += (Dat / 1000) * 0x1000;
	Dat = Dat % 1000;	
	Result += (Dat / 100) * 0x100;
	Dat = Dat % 100;	
	Result += (Dat / 10) * 0x10;
	Dat = Dat % 10;	
	Result += Dat;
		
	return(Result);	
}

u32	fnHexToBcd_u32(u32 Dat)
{	
	u32 result = 0;
	
	Dat = Dat % 100000000;
	result += (Dat / 10000000) * 0x10000000;
	Dat = Dat % 10000000;	
	result += (Dat / 1000000) * 0x1000000;
	Dat = Dat % 1000000;
	result += (Dat / 100000) * 0x100000;
	Dat = Dat % 100000;
	result += (Dat / 10000) * 0x10000;
	Dat = Dat % 10000;	
	result += (Dat / 1000) * 0x1000;
	Dat = Dat % 1000;
	result += (Dat / 100) * 0x100;
	Dat = Dat % 100;
	result += (Dat / 10) * 0x10;
	Dat = Dat % 10;	
	result += Dat;
	
	return(result);
}

u8 fnBcdToHex_u8(u8 Dat)
{
	u8 Result;
	u8 tmp = 0x0f;
	Result = (Dat & tmp) +
		   	 ( (Dat >> 4) & tmp) * 10;
			
	return(Result);	
}

//========================================

u32 fnBcdAdd_u32(u32 Augend,u32 Addend)
{
	u8 ResultTmp;		
	u8 CBit = 0,i;
	u32 Result = 0;
	
	for(i = 0 ; i < 32 ; i += 4)
	{
		ResultTmp = ( (Augend >> i) & 0x0f) + ( (Addend >> i) & 0x0f) + CBit;
		if(ResultTmp > 9) ResultTmp += 6;
		CBit = ResultTmp >> 4;	
		Result += ((u32)(ResultTmp & 0x0f) << i);
	}		
	return(Result);
}
	


s8	fnDFCmp_DF20(sDF20 *Dat1, sDF20 *Dat2)
{
	u32 Tmp1 , Tmp2;
	
	Tmp1 =  ((u32)Dat1->Year << 16) + 
			( ((u32)Dat1->Month & 0x3f) << 8) +
			((u32)Dat1->Day & 0x3f) ;
	Tmp2 =  ((u32)Dat2->Year << 16) + 
			( ((u32)Dat2->Month & 0x3f) << 8) +
			((u32)Dat2->Day & 0x3f) ;
	if(Tmp1 > Tmp2) return(1);
	else if(Tmp1 < Tmp2) return(-1);
	else return(0);
}

// 得到当前年、月得到当前月的天数
u8	 fnGetDay(u8 SMonth , u8 SYear)
{
	u8	SDay;
	SMonth = fnBcdToHex_u8(SMonth);
	SYear = fnBcdToHex_u8(SYear);	
	switch(SMonth)
	{
		case 1:
		case 3:
		case 5:
		case 7:	
		case 8:
		case 10:
		case 12:
			SDay = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			SDay = 30;
			break;
		case 2:
			if( (SYear % 4) == 0) SDay = 29;
			else SDay = 28;	
			break;				
	}
	SDay = fnHexToBcd_u8(SDay);
	return(SDay);
}

//函数:获取两个日期间的天数
//参数:Date1、Date2日期(sDF20)
//返回:天数(Date1 > Date2 返回正、否则返回负)
s32 fnTwoDateApart_DayCount(sDF20 *Date1 , sDF20 *Date2)
{
	s32 Total;
	u16 i , SYear , EYear;
	u8 SMonth , EMonth;
	
	sDF20 *pSDate , *pEDate;
	
	if(fnDFCmp_DF20( (sDF20 *)Date1 , (sDF20 *)Date2) > 0) 
	{
		pSDate = Date2;
		pEDate = Date1;					
	}
	else
	{
		pSDate = Date1;
		pEDate = Date2;		
	}
		
	SYear = 2000 + fnBcdToHex_u8(pSDate->Year);
	EYear = 2000 + fnBcdToHex_u8(pEDate->Year);
	
	for(Total = 0 , i = SYear ; i < EYear ; i++)
	{
		Total += 365;
		if( (i % 4) == 0) Total += 1;		
	}
	
	EMonth = fnBcdToHex_u8(pEDate->Month & 0x1f);
	for(i = 1 ; i < EMonth ; i++)
	{	
		switch(i)
		{
		case 1:
		case 3:
		case 5:
		case 7:	
		case 8:
		case 10:
		case 12:
			Total += 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			Total += 30;
			break;
		case 2:
			if( (EYear % 4) == 0) Total += 29;
			else Total += 28;	
			break;				
		}
	}
	Total += fnBcdToHex_u8(pEDate->Day & 0x3f);
	
	SMonth = fnBcdToHex_u8(pSDate->Month & 0x1f);
	for(i = 1 ; i < SMonth ; i++)
	{	
		switch(i)
		{
		case 1:
		case 3:
		case 5:
		case 7:	
		case 8:
		case 10:
		case 12:
			Total -= 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			Total -= 30;
			break;
		case 2:
			if( (SYear % 4) == 0) Total -= 29;
			else Total -= 28;	
			break;				
		}
	}
	Total -= fnBcdToHex_u8(pSDate->Day & 0x3f);
	
	if(fnDFCmp_DF20( (sDF20 *)Date1 , (sDF20 *)Date2) < 0) Total = -Total;	
	return(Total);
}



u16 fnDFConver_Bcd16To16(s16 Dat)
{
	u16 Result;
	Result = abs(Dat) % 8000;			
	Result = fnHexToBcd_u16(Result);
        if(Dat < 0 ) Result |= 0x8000;
	else Result &= 0x7fff;	
	return(Result);
}

u32 fnDFConver_Bcd32To32(s32 Dat)
{
	u32 Result;
	Result = labs(Dat) % 80000000;			
	Result = fnHexToBcd_u32(Result);
        if(Dat < 0 ) Result |= 0x80000000;
	else Result &= 0x7fffffff;	
	return(Result);
}

sDF07 fnDFConver_Bcd16ToDF07(u16 Dat)
{
	sDF07 Result;	
	Result.Dat = Dat;
	return(Result);
}

sDF25 fnDFConver_Bcd32ToDF25(u32 Dat)
{
	sDF25 Result;
	
	if(Dat & 0x80000000) Result.S = 1;
	else Result.S = 0;	
	
	Dat &= 0x7ffffff;
	
	Result.Dat0 = Dat >> 4;
	Result.Dat1 = Dat >> 12;
	Result.Dat2 = (Dat >> 20) & 0x7f;	
	return(Result);
}

sDF33	fnDFConver_Bcd16ToDF33(u16 Dat)
{
	sDF33 Result;	
	Result.Dat = Dat;
	return(Result);
}


sDF09	fnDFConver_Hex32ToDF09(u32 Dat)
{	
	sDF09	Result;	
	memset(&Result , 0 , sizeof(sDF09) );
	//if(Dat < 0) Result.S = 1;
	//else Result.S = 0;	
	
	//Dat = abs(Dat) % 800000;
	Dat = fnHexToBcd_u32(Dat);
	
	Result.Dat0 = Dat;
	Result.Dat1 = Dat >> 8;
	Result.Dat2 = Dat >> 16;
	return(Result);	
}












