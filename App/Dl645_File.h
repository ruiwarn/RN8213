
#ifndef _DL645_FILE_H
#define _DL645_FILE_H

#ifdef   _DL645_FILE_GLOBALS
	#define  DL645FILE_EXT
#else
	#define  DL645FILE_EXT  extern 
#endif

#pragma pack(1)	

//---�α����ļ�-------
typedef struct {
	//---��ѹ---
	sDF07		Ua;
	//---���ߵ���---
	sDF25		Ia;
	//---���ߵ���---
	sDF25		In;  
	//---˲ʱ�й�����---
	sDF09		PPw;
	//---��������---
	sDF26		Pf;
	//---���---
	sDF05		AngleIa;
	//---����---
	sDF30		Fre;	
	sDF07		Temperature;
	sDF30		ClockBat;
	sDF27		tWorkBat;
} sDl645VarFile_TypeDef;


typedef struct {				//���ڵ�������β��
    u32	    PieceEg;
} sEgPiece_TypeDef;				//sEgPiece_TypeDef;

//---ͨ�ò����ļ�-------
typedef struct {
	u8		Ir1Style;	//�����ͺ���ڲ�����������
	u8		Ir2Style;	//�Ӵ��ͺ���ڲ�����������
	u8		Com1Style;	//ͨ�ſ�1������������
	u8		Com2Style;	//ͨ�ſ�2������������
	u8		Com3Style;	//ͨ�ſ�3������������	
} sComPara_TypeDef;

typedef struct {
	u8		nCycDisp;	//ѭ������
	u8		DispTime;	//��ʾʱ��
	u8		DecimalEg;	//��ʾ����С��λ��
	u8		DecimalPw;	//��ʾ����С��λ��
	
	u8		nKeyDisp;	//��������
	
	u8		CycDispItem[DL645_MAX_CYCDISP][5];
	u8		KeyDispItem[DL645_MAX_KEYDISP][5];
	u8		AllOnDispTime;	// �ϵ�ȫ��ʱ��5-30S����Ĭ��5S
} sDispPara_TypeDef;

typedef struct {
	sComPara_TypeDef	ComPara;
	sDispPara_TypeDef	DispPara;
} sDl645GeneralParaFile_TypeDef;	//

//---�߼������ļ�-------
typedef struct {
	u8 		PExpression;       	//�й���Ϸ�ʽ������
	
	sDF12	ComAddr;			//ͨ�ŵ�ַ
	sDF12	MeterNo;			//���
	sDF10	PConstE;			//�й�����
} sDl645HighParaFile_TypeDef;		//229 Byte

//---У������ļ�-------���ɸ��ݼ���оƬ���ģ�
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
	float   KUrms;		// ��ѹϵ��
	float	KIArms;		// Aͨ������ϵ��
	float   KIBrms;		// Bͨ������ϵ��
	float	KPrms;		// ����ϵ��
	u32		PStarDisp;	// ������ʾֵ�����ʴ��ڴ�ֵʱ��ʾ 0.2%
	u32		IStarDisp;	// ������ʾֵ���������ڴ�ֵʱ��ʾ 0.2%
	u32		ChkSum;
	u16		RTCDota0;		// RTCУ���Ĵ���
}sDl645FirmParaFile_TypeDef;		//58 Byte

//---�洢�������ļ�-------
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
//===�����ļ�����Ϣ========================================
typedef enum{
	//����
	Dl645FileItemInfoNum_GeneralPara_ComPara 	=	0	,
	Dl645FileItemInfoNum_GeneralPara_DispPara 	=	1	,
	Dl645FileItemInfoNum_GeneralPara_Max 	    =	2	,	
	//���	
	Dl645FileItemInfoLen_GeneralPara_ComPara 	    =	(sizeof(sComPara_TypeDef) )	,
	Dl645FileItemInfoLen_GeneralPara_DispPara 	    =	(sizeof(sDispPara_TypeDef) )	,
	//��ƫ�Ƶ�ַ
	Dl645FileItemInfoOffAddr_GeneralPara_ComPara 	=	(	0 	),
	Dl645FileItemInfoOffAddr_GeneralPara_DispPara 	=	(	Dl645FileItemInfoOffAddr_GeneralPara_ComPara 	+	Dl645FileItemInfoLen_GeneralPara_ComPara 	),
	Dl645FileItemInfoOffAddr_GeneralPara_End	=	(	Dl645FileItemInfoOffAddr_GeneralPara_DispPara 	+	Dl645FileItemInfoLen_GeneralPara_DispPara 	)
} eDl645FileItemInfo_GeneralPara_TypeDef;//ͨ�ò����ļ���У�������ȣ�ͨѶ�����ʣ��й����ʵ�ѹ�����ޣ�����ʱ��ȣ�
typedef enum{
	//����
	Dl645FileItemInfoNum_HighPara_PExpression  	=	0	,

	Dl645FileItemInfoNum_HighPara_ComAddr	=	1	,
	Dl645FileItemInfoNum_HighPara_MeterNo	=	2	,
	Dl645FileItemInfoNum_HighPara_PConstE	=	3	,
	Dl645FileItemInfoNum_HighPara_Max 	=	4	,
	
	//���	
	Dl645FileItemInfoLen_HighPara_PExpression  	=	(	1	),

	Dl645FileItemInfoLen_HighPara_ComAddr	=	(	sizeof(	sDF12	)	),
	Dl645FileItemInfoLen_HighPara_MeterNo	=	(	sizeof(	sDF12	)	),
	Dl645FileItemInfoLen_HighPara_PConstE	=		sizeof(	sDF10	),
	//��ƫ�Ƶ�ַ
	Dl645FileItemInfoOffAddr_HighPara_PExpression  	=	(	0			),
	Dl645FileItemInfoOffAddr_HighPara_ComAddr	=	(	Dl645FileItemInfoOffAddr_HighPara_PExpression  	+	Dl645FileItemInfoLen_HighPara_PExpression	),
	Dl645FileItemInfoOffAddr_HighPara_MeterNo	=	(	Dl645FileItemInfoOffAddr_HighPara_ComAddr	+	Dl645FileItemInfoLen_HighPara_ComAddr	),
	Dl645FileItemInfoOffAddr_HighPara_PConstE	=	(	Dl645FileItemInfoOffAddr_HighPara_MeterNo	+	Dl645FileItemInfoLen_HighPara_MeterNo	),
	Dl645FileItemInfoOffAddr_HighPara_End 	=	(	Dl645FileItemInfoOffAddr_HighPara_PConstE	+	Dl645FileItemInfoLen_HighPara_PConstE	)
} eDl645FileItemInfo_HighPara_TypeDef;//�߼������ļ�����ų����ʲ���ţ��й��ȼ��Ȳ�����
typedef enum{
	//����
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
		
	//���	
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
	
	//��ƫ�Ƶ�ַ
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
} eDl645FileItemInfo_FirmPara_TypeDef;//У��������ɸ��ݼ���оƬ���ģ�


typedef enum{
	//����
	Dl645FileItemInfoNum_NvRam_FristPowOn 	=	0	,
	Dl645FileItemInfoNum_NvRam_pFile 	=	1	,	
	Dl645FileItemInfoNum_NvRam_Max 	=	2	,	
	//���	
	Dl645FileItemInfoLen_NvRam_FristPowOn	=	(4) ,
	Dl645FileItemInfoLen_NvRam_pFile	=	(11) ,	
	//��ƫ�Ƶ�ַ
	Dl645FileItemInfoOffAddr_NvRam_FristPowOn 	=	(0),
	Dl645FileItemInfoOffAddr_NvRam_pFile 	=	(	Dl645FileItemInfoOffAddr_NvRam_FristPowOn 		+	Dl645FileItemInfoLen_NvRam_FristPowOn 	),
	Dl645FileItemInfoOffAddr_NvRam_End 	=	(	Dl645FileItemInfoOffAddr_NvRam_pFile 		+	Dl645FileItemInfoLen_NvRam_pFile 	)
} eDl645FileItemInfo_NvRam_TypeDef;//�洢������

//===�����ļ���Ϣ==========================================
#define FRAM_SADDR	AT24C256_START_ADDR

typedef enum
{
	//�ļ����
	Dl645FileId_NvRam	=	0	,	
	Dl645FileId_FirmPara	=	1	,	
	Dl645FileId_HighPara	=	2	,		
	Dl645FileId_GeneralPara	=	3	,	
	Dl645FileId_Max	=	4	,	

	//�ļ�����
	Dl645FileLen_NvRam	=	(sizeof(sDl645NvRamFile_TypeDef) ),
	Dl645FileLen_FirmPara	=	(sizeof(sDl645FirmParaFile_TypeDef) ),
	Dl645FileLen_HighPara	=	(sizeof(sDl645HighParaFile_TypeDef) ),
	Dl645FileLen_GeneralPara	=	(sizeof(sDl645GeneralParaFile_TypeDef) ),
} eDl645FileInfo_TypeDef;
//----FRAM_ADDR ����----
//���������洢��ַ����
#define	Dl645FileAddr_FRAM_Start		(	FRAM_SADDR		)	
#define	Dl645FileAddr_NvRam		(	Dl645FileAddr_FRAM_Start		)	
#define	Dl645FileAddr_FirmPara		(	Dl645FileAddr_NvRam	+	Dl645FileLen_NvRam	)//ö��ֵ������ʱ��Ҫת��Ϊ�޷��������������ֵ
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
	const sFileItemInfo_TypeDef *pItemInfo;	//�ļ�����Ϣ
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



