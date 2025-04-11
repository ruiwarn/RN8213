
#ifndef  _MYMATH_H
#define  _MYMATH_H


extern u8 fnHexToBcd_u8(u8 Dat);
extern u16 fnHexToBcd_u16(u16 Dat);
extern u32 fnHexToBcd_u32(u32 Dat);


extern u8 fnBcdToHex_u8(u8 Dat);

extern u8	fnSum(u8 *Dat , u16 Len);
extern u8 fnWeek_Calculate(u8 Year , u8 Month , u8 Day);

extern s8 fnDFCmp_DF20(sDF20 *Dat1, sDF20 *Dat2);
extern s8 fnDFCmp_DF15(sDF15 *Dat1, sDF15 *Dat2);

extern s32 fnTwoDateApart_DayCount(sDF20 *Date1 , sDF20 *Date2);

extern u32 fnBcdAdd_u32(u32 Augend,u32 Addend);

extern sDF09	fnDFConver_Hex32ToDF09(u32 Dat);

extern u16 fnDFConver_Bcd16To16(s16 Dat);
extern u32 fnDFConver_Bcd32To32(s32 Dat);
extern	sDF07 fnDFConver_Bcd16ToDF07(u16 Dat);
extern	sDF25 fnDFConver_Bcd32ToDF25(u32 Dat);
extern	sDF33	fnDFConver_Bcd16ToDF33(u16 Dat);
extern	u8	 fnGetDay(u8 SMonth , u8 SYear);
#endif	//_MYMATH_H


