
#ifndef __DL645_TYPE_H
#define __DL645_TYPE_H

//===基本数据格式定义=============
#pragma pack(1)

typedef struct {
	u8 Second;
	u8 Minute;
	u8 Hour;
	u8 Day;
	u8 Month : 5;
	u8 Week : 3;	
	u8 Year;
} sDF01;

typedef struct {
	u16 Dat : 12;
	u16 S : 1;
	u16 G : 3;
} sDF02;

typedef struct {
	u32 Dat : 28;
	u32 S : 1;
	u32 Reserve1 : 1;	
	u32 G : 1;
	u32 Reserve2 : 1;	
} sDF03;

typedef struct {
	u8 Dat : 7;
	u8 S : 1;
} sDF04;

typedef struct {
	u16 Dat : 15;
	u16 S : 1;
} sDF05;

typedef struct {
	u16 Dat : 15;
	u16 S : 1;
} sDF06;

typedef struct {
	u16 Dat;
} sDF07;

typedef struct {
	u16 Dat;
} sDF08;

typedef struct {
//	u32 Dat : 23;
//	u32 S : 1;
	u8 Dat0;
	u8 Dat1;
	u8 Dat2 : 7;
	u8 S : 1;			
} sDF09;

typedef struct {
//	u32 Dat : 24;
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;	
} sDF10;

typedef struct {
	u32 Dat;
} sDF11;

typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;
	u8 Dat3;
	u8 Dat4;
	u8 Dat5;		
} sDF12;

typedef struct {
	u32 Dat;
} sDF13;

typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;
	u8 Dat3;
	u8 Dat4;	
} sDF14;

typedef struct {
	u8 Minute;
	u8 Hour;
	u8 Day;
	u8 Month;
	u8 Year;
} sDF15;

typedef struct {
	u8 Second;
	u8 Minute;
	u8 Hour;
	u8 Day;
} sDF16;

typedef struct {
	u8 Minute;
	u8 Hour;
	u8 Day;
	u8 Month;
} sDF17;

typedef struct {
	u8 Minute;
	u8 Hour;
	u8 Day;
} sDF18;

typedef struct {
	u8 Minute;
	u8 Hour;
} sDF19;

typedef struct {
	u8 Day;
	u8 Month;
	u8 Year;
} sDF20;

typedef struct {
	u8 Month;
	u8 Year;
} sDF21;

typedef struct {
	u8 Dat;
} sDF22;

typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;		
} sDF23;

typedef struct {
	u8 Hour;
	u8 Day;
} sDF24;

typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2 : 7;	
	u8 S : 1;	
} sDF25;

typedef struct {
	u16 Dat;	
} sDF26;

typedef struct {
	u32 Dat;	
} sDF27;

typedef struct {
	u8 Hour;
	u8 Day;
	u8 Month;
} sDF28;

typedef struct {
	u8 Hour;
	u8 Day;
	u8 Month;
	u8 Year;
} sDF29;

//===以下为增加的基本数据格式=================
typedef struct {
	u16 Dat;
} sDF30;

typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;	
} sDF31;

typedef struct {
	u32 Dat : 31;
	u32 S : 1;	
} sDF32;

typedef struct {
	u16 Dat;
} sDF33;

typedef struct {
	u8 Second;
	u8 Minute;
	u8 Hour;
	u8 Day;
	u8 Month;
	u8 Year;
} sDF34;


typedef struct {
	u32 Dat0;
	u32 Dat1;		
} sDF35;

typedef struct {
	u16 Dat0;
	u32 Dat1;
	u16 Dat2;	
} sDF36;

typedef struct {
	u16 Dat0;
	u16 Dat1;
	u16 Dat2;	
	u16 Dat3;	
} sDF37;

typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;	
	u8 Dat3;
	u8 Dat4;
	u8 Dat5;
	u8 Dat6;	
	u8 Dat7;
	u8 Dat8;
	u8 Dat9;	
} sDF38;


typedef union {
	sDF35	DF01	;
	sDF36	DF02	;
	sDF37	DF03	;
} u64;


//-----------------------

typedef union {
	sDF01	DF01	;
	sDF02	DF02	;
	sDF03	DF03	;
	sDF04	DF04	;
	sDF05	DF05	;
	sDF06	DF06	;
	sDF07	DF07	;
	sDF08	DF08	;
	sDF09	DF09	;
	sDF10	DF10	;
	sDF11	DF11	;
	sDF12	DF12	;
	sDF13	DF13	;
	sDF14	DF14	;
	sDF15	DF15	;
	sDF16	DF16	;
	sDF17	DF17	;
	sDF18	DF18	;
	sDF19	DF19	;
	sDF20	DF20	;
	sDF21	DF21	;
	sDF22	DF22	;
	sDF23	DF23	;
	sDF24	DF24	;
	sDF25	DF25	;
	sDF26	DF26	;
	sDF27	DF27	;
//	sDF28	DF28	;
//	sDF29	DF29	;
	sDF30	DF30	;
	sDF31	DF31	;
	sDF32	DF32	;
	sDF33	DF33	;
	sDF33	DF34	;
} uDFAll;

//===以下为应用数据格式=================
typedef	sDF11 sFsEgSh_TypeDef;	

typedef struct {
	sDF09	Need;
	sDF15	NeedTime;
} sFsNeed_TypeDef;

typedef struct {
	u8		nPOTTable;
	u8		Day;
	u8		Month;	
} sTimeZoneTable_TypeDef;

typedef struct {
	u8		Fl;
	u8 		Minute;
	u8		Hour;
} sPOTTable_TypeDef;

typedef struct {	
	u8		nPOTTable;
	u8		Day;
	u8		Month;
	u8		Year;
} sHolidayTable_TypeDef;


#pragma pack()


#endif	//__DL645_TYPE_H

