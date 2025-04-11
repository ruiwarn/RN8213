
#ifndef _DL645_FILE_H
#define _DL645_FILE_H

#ifdef   _DL645_FILE_GLOBALS
	#define  DL645FILE_EXT
#else
	#define  DL645FILE_EXT  extern 
#endif

#pragma pack(1)	

//---参变量文件-------
typedef struct {
	//---电压---
	sDF07		Ua;
	//---火线电流---
	sDF25		Ia;
	//---零线电流---
	sDF25		In;  
	//---瞬时有功功率---
	sDF09		PPw;
	//---功率因数---
	sDF26		Pf;
	//---相角---
	sDF05		AngleIa;
	//---其他---
	sDF30		Fre;	
	sDF07		Temperature;
	sDF30		ClockBat;
	sDF27		tWorkBat;
} sDl645VarFile_TypeDef;


typedef struct {				//用于电能脉冲尾数
    u32	    PieceEg;
} sEgPiece_TypeDef;				//sEgPiece_TypeDef;

//---通用参数文件-------
typedef struct {
	u8		Ir1Style;	//调制型红外口波特率特征字
	u8		Ir2Style;	//接触型红外口波特率特征字
	u8		Com1Style;	//通信口1波特率特征字
	u8		Com2Style;	//通信口2波特率特征字
	u8		Com3Style;	//通信口3波特率特征字	
} sComPara_TypeDef;

typedef struct {
	u8		nCycDisp;	//循显屏数
	u8		DispTime;	//显示时间
	u8		DecimalEg;	//显示电能小数位数
	u8		DecimalPw;	//显示功率小数位数
	
	u8		nKeyDisp;	//键显屏数
	
	u8		CycDispItem[DL645_MAX_CYCDISP][5];
	u8		KeyDispItem[DL645_MAX_KEYDISP][5];
	u8		AllOnDispTime;	// 上电全显时间5-30S设置默认5S
} sDispPara_TypeDef;

typedef struct {
	sComPara_TypeDef	ComPara;
	sDispPara_TypeDef	DispPara;
} sDl645GeneralParaFile_TypeDef;	//

//---高级参数文件-------
typedef struct {
	u8 		PExpression;       	//有功组合方式特征字
	
	sDF12	ComAddr;			//通信地址
	sDF12	MeterNo;			//表号
	sDF10	PConstE;			//有功常数
} sDl645HighParaFile_TypeDef;		//229 Byte

//---校表参数文件-------（可根据计量芯片更改）
typedef struct 
{		
	u16		SYSCON;
	u16		EMUCON;
	u16		HFConst;
	u16		PStart;	
	u16		QStart;   //10
	u16		GPQA;
	u16		GPQB;
	u16		IAGain;
	u16		UGain;
	u16		IBGain;
	u16		PhsA;	
	u16		PhsB;
	u16		QPhsCal;  //22
	u16		APOSA;
	u16		APOSB;	
	u16		RPOSA;
	u16		RPOSB;
	u16		IARMSOS;  	//32
	u16		IBRMSOS;	//34
	float   KUrms;		// 电压系数
	float	KIArms;		// A通道电流系数
	float   KIBrms;		// B通道电流系数
	float	KPrms;		// 功率系数
	u32		PStarDisp;	// 功率显示值，功率大于此值时显示 0.2%
	u32		IStarDisp;	// 电流显示值，电流大于此值时显示 0.2%
	u32		ChkSum;
	u16		RTCDota0;		// RTC校正寄存器
}sDl645FirmParaFile_TypeDef;		//58 Byte

//---存储器特性文件-------
typedef struct {
	u8    	FristPowOn[4];
	
	u8		pRmEgSh;			//0
	u8		pTimeFrz;			//1
	u8		pWinkFrz;			//2
	u8		pDayFrz;			//3
	u8		pTZoneSwFrz;		//4
	u8		pPOTSwFrz;			//5
	u8		pRatePriceSwFrz;	//6
	u8		pLadderSwFrz;		//7
	u8		pHourFrz;			//8
	u8		pStat;				//9
	u8		pRmApplyEg;			//10
} sDl645NvRamFile_TypeDef;
#pragma pack()

//=============================================================================
//===定义文件项信息========================================
typedef enum{
	//项编号
	Dl645FileItemInfoNum_GeneralPara_ComPara 	=	0	,
	Dl645FileItemInfoNum_GeneralPara_DispPara 	=	1	,
	Dl645FileItemInfoNum_GeneralPara_Max 	    =	2	,	
	//项长度	
	Dl645FileItemInfoLen_GeneralPara_ComPara 	    =	(sizeof(sComPara_TypeDef) )	,
	Dl645FileItemInfoLen_GeneralPara_DispPara 	    =	(sizeof(sDispPara_TypeDef) )	,
	//项偏移地址
	Dl645FileItemInfoOffAddr_GeneralPara_ComPara 	=	(	0 	),
	Dl645FileItemInfoOffAddr_GeneralPara_DispPara 	=	(	Dl645FileItemInfoOffAddr_GeneralPara_ComPara 	+	Dl645FileItemInfoLen_GeneralPara_ComPara 	),
	Dl645FileItemInfoOffAddr_GeneralPara_End	=	(	Dl645FileItemInfoOffAddr_GeneralPara_DispPara 	+	Dl645FileItemInfoLen_GeneralPara_DispPara 	)
} eDl645FileItemInfo_GeneralPara_TypeDef;//通用参数文件（校表脉冲宽度，通讯波特率，有功功率电压上下限，冻结时间等）
typedef enum{
	//项编号
	Dl645FileItemInfoNum_HighPara_PExpression  	=	0	,

	Dl645FileItemInfoNum_HighPara_ComAddr	=	1	,
	Dl645FileItemInfoNum_HighPara_MeterNo	=	2	,
	Dl645FileItemInfoNum_HighPara_PConstE	=	3	,
	Dl645FileItemInfoNum_HighPara_Max 	=	4	,
	
	//项长度	
	Dl645FileItemInfoLen_HighPara_PExpression  	=	(	1	),

	Dl645FileItemInfoLen_HighPara_ComAddr	=	(	sizeof(	sDF12	)	),
	Dl645FileItemInfoLen_HighPara_MeterNo	=	(	sizeof(	sDF12	)	),
	Dl645FileItemInfoLen_HighPara_PConstE	=		sizeof(	sDF10	),
	//项偏移地址
	Dl645FileItemInfoOffAddr_HighPara_PExpression  	=	(	0			),
	Dl645FileItemInfoOffAddr_HighPara_ComAddr	=	(	Dl645FileItemInfoOffAddr_HighPara_PExpression  	+	Dl645FileItemInfoLen_HighPara_PExpression	),
	Dl645FileItemInfoOffAddr_HighPara_MeterNo	=	(	Dl645FileItemInfoOffAddr_HighPara_ComAddr	+	Dl645FileItemInfoLen_HighPara_ComAddr	),
	Dl645FileItemInfoOffAddr_HighPara_PConstE	=	(	Dl645FileItemInfoOffAddr_HighPara_MeterNo	+	Dl645FileItemInfoLen_HighPara_MeterNo	),
	Dl645FileItemInfoOffAddr_HighPara_End 	=	(	Dl645FileItemInfoOffAddr_HighPara_PConstE	+	Dl645FileItemInfoLen_HighPara_PConstE	)
} eDl645FileItemInfo_HighPara_TypeDef;//高级参数文件（表号常数资产表号，有功等级等参数）
typedef enum{
	//项编号
	Dl645FileItemInfoNum_FirmPara_SYSCON	=	0	,
	Dl645FileItemInfoNum_FirmPara_EMUCON	=	1	,
	Dl645FileItemInfoNum_FirmPara_HFConst	=	2	,
	Dl645FileItemInfoNum_FirmPara_PStart	=	3	,
	Dl645FileItemInfoNum_FirmPara_QStart	=	4	,
	Dl645FileItemInfoNum_FirmPara_GPQA		=	5	,
	Dl645FileItemInfoNum_FirmPara_GPQB		=	6	,
	Dl645FileItemInfoNum_FirmPara_IAGAIN	=	7	,
	Dl645FileItemInfoNum_FirmPara_UGAIN		=	8	,
	Dl645FileItemInfoNum_FirmPara_IBGAIN	=	9	,
	Dl645FileItemInfoNum_FirmPara_PhsA		=	10	,
	Dl645FileItemInfoNum_FirmPara_PhsB		=	11	,
	Dl645FileItemInfoNum_FirmPara_QPhsCal	=	12	,
	Dl645FileItemInfoNum_FirmPara_APOSA		=	13	,
	Dl645FileItemInfoNum_FirmPara_APOSB		=	14	,
	Dl645FileItemInfoNum_FirmPara_RPOSA		=	15	,
	Dl645FileItemInfoNum_FirmPara_RPOSB		=	16	,
	Dl645FileItemInfoNum_FirmPara_IARMSOS	=	17	,
	Dl645FileItemInfoNum_FirmPara_IBRMSOS	=	18	,
	Dl645FileItemInfoNum_FirmPara_KUrms		=	19	,
	Dl645FileItemInfoNum_FirmPara_KIArms	=	20	,
	Dl645FileItemInfoNum_FirmPara_KIBrms	=	21	,
	Dl645FileItemInfoNum_FirmPara_KPrms		=	22	,
	Dl645FileItemInfoNum_FirmPara_PStarDisp	=	23	,
	Dl645FileItemInfoNum_FirmPara_IStarDisp	=	24	,
	Dl645FileItemInfoNum_FirmPara_ChkSum	=	25	,
	Dl645FileItemInfoNum_FirmPara_RTCDota0	=	26	,
	Dl645FileItemInfoNum_FirmPara_Max		=	27	,
		
	//项长度	
	Dl645FileItemInfoLen_FirmPara_SYSCON	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_EMUCON	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_HFConst	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_PStart	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_QStart	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_GPQA		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_GPQB		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_IAGAIN	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_UGAIN		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_IBGAIN	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_PhsA		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_PhsB		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_QPhsCal	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_APOSA		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_APOSB		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_RPOSA		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_RPOSB		=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_IARMSOS	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_IBRMSOS	=	(sizeof(	u16 	)  ),
	Dl645FileItemInfoLen_FirmPara_KUrms		=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_KIArms	=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_KIBrms	=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_KPrms		=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_PStarDisp	=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_IStarDisp	=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_ChkSum	=	(sizeof(	u32 	)  ),
	Dl645FileItemInfoLen_FirmPara_RTCDota0	=	(sizeof(	u16 	)  ),
	
	//项偏移地址
	Dl645FileItemInfoOffAddr_FirmPara_SYSCON	=	(		0		),
	Dl645FileItemInfoOffAddr_FirmPara_EMUCON	=	(	Dl645FileItemInfoOffAddr_FirmPara_SYSCON	+	Dl645FileItemInfoLen_FirmPara_SYSCON	),
	Dl645FileItemInfoOffAddr_FirmPara_HFConst	=	(	Dl645FileItemInfoOffAddr_FirmPara_EMUCON	+	Dl645FileItemInfoLen_FirmPara_EMUCON	),
	Dl645FileItemInfoOffAddr_FirmPara_PStart	=	(	Dl645FileItemInfoOffAddr_FirmPara_HFConst	+	Dl645FileItemInfoLen_FirmPara_HFConst	),
	Dl645FileItemInfoOffAddr_FirmPara_QStart	=	(	Dl645FileItemInfoOffAddr_FirmPara_PStart	+	Dl645FileItemInfoLen_FirmPara_PStart	),
	Dl645FileItemInfoOffAddr_FirmPara_GPQA		=	(	Dl645FileItemInfoOffAddr_FirmPara_QStart	+	Dl645FileItemInfoLen_FirmPara_QStart	),
	Dl645FileItemInfoOffAddr_FirmPara_GPQB		=	(	Dl645FileItemInfoOffAddr_FirmPara_GPQA	+	Dl645FileItemInfoLen_FirmPara_GPQA	),
	Dl645FileItemInfoOffAddr_FirmPara_IAGAIN	=	(	Dl645FileItemInfoOffAddr_FirmPara_GPQB	+	Dl645FileItemInfoLen_FirmPara_GPQB	),
	Dl645FileItemInfoOffAddr_FirmPara_UGAIN		=	(	Dl645FileItemInfoOffAddr_FirmPara_IAGAIN	+	Dl645FileItemInfoLen_FirmPara_IAGAIN	),
	Dl645FileItemInfoOffAddr_FirmPara_IBGAIN	=	(	Dl645FileItemInfoOffAddr_FirmPara_UGAIN	+	Dl645FileItemInfoLen_FirmPara_UGAIN	),
	Dl645FileItemInfoOffAddr_FirmPara_PhsA		=	(	Dl645FileItemInfoOffAddr_FirmPara_IBGAIN	+	Dl645FileItemInfoLen_FirmPara_IBGAIN	),
	Dl645FileItemInfoOffAddr_FirmPara_PhsB		=	(	Dl645FileItemInfoOffAddr_FirmPara_PhsA	+	Dl645FileItemInfoLen_FirmPara_PhsA	),
	Dl645FileItemInfoOffAddr_FirmPara_QPhsCal	=	(	Dl645FileItemInfoOffAddr_FirmPara_PhsB	+	Dl645FileItemInfoLen_FirmPara_PhsB	),
	Dl645FileItemInfoOffAddr_FirmPara_APOSA		=	(	Dl645FileItemInfoOffAddr_FirmPara_QPhsCal	+	Dl645FileItemInfoLen_FirmPara_QPhsCal	),
	Dl645FileItemInfoOffAddr_FirmPara_APOSB		=	(	Dl645FileItemInfoOffAddr_FirmPara_APOSA	+	Dl645FileItemInfoLen_FirmPara_APOSA	),
	Dl645FileItemInfoOffAddr_FirmPara_RPOSA		=	(	Dl645FileItemInfoOffAddr_FirmPara_APOSB	+	Dl645FileItemInfoLen_FirmPara_APOSB	),
	Dl645FileItemInfoOffAddr_FirmPara_RPOSB		=	(	Dl645FileItemInfoOffAddr_FirmPara_RPOSA	+	Dl645FileItemInfoLen_FirmPara_RPOSA	),
	Dl645FileItemInfoOffAddr_FirmPara_IARMSOS	=	(	Dl645FileItemInfoOffAddr_FirmPara_RPOSB	+	Dl645FileItemInfoLen_FirmPara_RPOSB	),
	Dl645FileItemInfoOffAddr_FirmPara_IBRMSOS	=	(	Dl645FileItemInfoOffAddr_FirmPara_IARMSOS	+	Dl645FileItemInfoLen_FirmPara_IARMSOS	),
	Dl645FileItemInfoOffAddr_FirmPara_KUrms		=	(	Dl645FileItemInfoOffAddr_FirmPara_IBRMSOS	+	Dl645FileItemInfoLen_FirmPara_IBRMSOS	),
	Dl645FileItemInfoOffAddr_FirmPara_KIArms	=	(	Dl645FileItemInfoOffAddr_FirmPara_KUrms	+	Dl645FileItemInfoLen_FirmPara_KUrms	),
	Dl645FileItemInfoOffAddr_FirmPara_KIBrms	=	(	Dl645FileItemInfoOffAddr_FirmPara_KIArms	+	Dl645FileItemInfoLen_FirmPara_KIArms	),
	Dl645FileItemInfoOffAddr_FirmPara_KPrms		=	(	Dl645FileItemInfoOffAddr_FirmPara_KIBrms	+	Dl645FileItemInfoLen_FirmPara_KIBrms	),
	Dl645FileItemInfoOffAddr_FirmPara_PStarDisp	=	(	Dl645FileItemInfoOffAddr_FirmPara_KPrms	+	Dl645FileItemInfoLen_FirmPara_KPrms	),
	Dl645FileItemInfoOffAddr_FirmPara_IStarDisp	=	(	Dl645FileItemInfoOffAddr_FirmPara_PStarDisp	+	Dl645FileItemInfoLen_FirmPara_PStarDisp	),
	Dl645FileItemInfoOffAddr_FirmPara_ChkSum	=	(	Dl645FileItemInfoOffAddr_FirmPara_IStarDisp	+	Dl645FileItemInfoLen_FirmPara_IStarDisp	),
	Dl645FileItemInfoOffAddr_FirmPara_RTCDota0	=	(	Dl645FileItemInfoOffAddr_FirmPara_ChkSum	+	Dl645FileItemInfoLen_FirmPara_ChkSum	),
	Dl645FileItemInfoOffAddr_FirmPara_End		=	(	Dl645FileItemInfoOffAddr_FirmPara_RTCDota0	+	Dl645FileItemInfoLen_FirmPara_RTCDota0	)
} eDl645FileItemInfo_FirmPara_TypeDef;//校表参数（可根据计量芯片更改）


typedef enum{
	//项编号
	Dl645FileItemInfoNum_NvRam_FristPowOn 	=	0	,
	Dl645FileItemInfoNum_NvRam_pFile 	=	1	,	
	Dl645FileItemInfoNum_NvRam_Max 	=	2	,	
	//项长度	
	Dl645FileItemInfoLen_NvRam_FristPowOn	=	(4) ,
	Dl645FileItemInfoLen_NvRam_pFile	=	(11) ,	
	//项偏移地址
	Dl645FileItemInfoOffAddr_NvRam_FristPowOn 	=	(0),
	Dl645FileItemInfoOffAddr_NvRam_pFile 	=	(	Dl645FileItemInfoOffAddr_NvRam_FristPowOn 		+	Dl645FileItemInfoLen_NvRam_FristPowOn 	),
	Dl645FileItemInfoOffAddr_NvRam_End 	=	(	Dl645FileItemInfoOffAddr_NvRam_pFile 		+	Dl645FileItemInfoLen_NvRam_pFile 	)
} eDl645FileItemInfo_NvRam_TypeDef;//存储器参数

//===定义文件信息==========================================
#define FRAM_SADDR	AT24C256_START_ADDR

typedef enum
{
	//文件编号
	Dl645FileId_NvRam	=	0	,	
	Dl645FileId_FirmPara	=	1	,	
	Dl645FileId_HighPara	=	2	,		
	Dl645FileId_GeneralPara	=	3	,	
	Dl645FileId_Max	=	4	,	

	//文件长度
	Dl645FileLen_NvRam	=	(sizeof(sDl645NvRamFile_TypeDef) ),
	Dl645FileLen_FirmPara	=	(sizeof(sDl645FirmParaFile_TypeDef) ),
	Dl645FileLen_HighPara	=	(sizeof(sDl645HighParaFile_TypeDef) ),
	Dl645FileLen_GeneralPara	=	(sizeof(sDl645GeneralParaFile_TypeDef) ),
} eDl645FileInfo_TypeDef;
//----FRAM_ADDR 定义----
//参数变量存储地址定义
#define	Dl645FileAddr_FRAM_Start		(	FRAM_SADDR		)	
#define	Dl645FileAddr_NvRam		(	Dl645FileAddr_FRAM_Start		)	
#define	Dl645FileAddr_FirmPara		(	Dl645FileAddr_NvRam	+	Dl645FileLen_NvRam	)//枚举值做运算时需要转换为无符号数或更大类型值
#define	Dl645FileAddr_HighPara		(	Dl645FileAddr_FirmPara	+	Dl645FileLen_FirmPara	)
#define	Dl645FileAddr_GeneralPara		(	Dl645FileAddr_HighPara	+	Dl645FileLen_HighPara	)
//----------------------------------------------------------
typedef	struct {
  u32 OffAddr;
  u32 Len;
} sFileItemInfo_TypeDef;

typedef	struct {
	u32 StartAddr;
	u32 Len;
	u32 MaxItem;
	const sFileItemInfo_TypeDef *pItemInfo;	//文件项信息
} sDl645FileInfo_TypeDef;

//------------------------
extern ErrorStatus fnDl645File_Read(u8 FileId , u32 OffAddr , void *Dst , u32 Len);
extern ErrorStatus fnDl645File_Write(u8 FileId , u32 OffAddr , void *Src , u32 Len);
extern ErrorStatus fnDl645File_Clr(u8 FileId);
extern ErrorStatus fnDl645FileItem_Read(u8 FileId , u8 nItem , void *Dst);
extern ErrorStatus fnDl645FileItem_Write(u8 FileId , u8 nItem , void *Src);
extern ErrorStatus fnDl645FileItem_Clr(u8 FileId , u8 nItem);
extern void fnDl645File_Init(void);
extern ErrorStatus fnDl645FileItem_Clr(u8 FileId , u8 nItem);
extern const sDl645FileInfo_TypeDef Dl645FileInfo[];
#endif // _DL645_FILE_H 



