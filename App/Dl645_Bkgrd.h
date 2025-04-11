

#ifndef _DL645_BKGRD_H
#define _DL645_BKGRD_H

#ifdef   _DL645_BKGRD_GLOBALS
	#define  DL645BKGRD_EXT
#else
	#define  DL645BKGRD_EXT  extern 
#endif

#define 	DL645BKGRD_FCHANGE_SECOND    	( (u32)1 << 0)
#define 	DL645BKGRD_FCHANGE_MINUTE   	( (u32)1 << 1)
#define 	DL645BKGRD_FCHANGE_HOUR      	( (u32)1 << 2)
#define 	DL645BKGRD_FCHANGE_DAY      	( (u32)1 << 3)
#define 	DL645BKGRD_FCHANGE_MONTH    	( (u32)1 << 4)
#define 	DL645BKGRD_FCHANGE_RMDAY1   	( (u32)1 << 5)
#define 	DL645BKGRD_FCHANGE_RMDAY2   	( (u32)1 << 6)
#define 	DL645BKGRD_FCHANGE_RMDAY3   	( (u32)1 << 7)
#define 	DL645BKGRD_FCHANGE_TIMEFRZ   	( (u32)1 << 8)
#define 	DL645BKGRD_FCHANGE_WINKFRZ   	( (u32)1 << 9)
#define 	DL645BKGRD_FCHANGE_DAYFRZ   	( (u32)1 << 10)
#define 	DL645BKGRD_FCHANGE_TZONEFRZ  	( (u32)1 << 11)
#define 	DL645BKGRD_FCHANGE_POTFRZ   	( (u32)1 << 12)
#define 	DL645BKGRD_FCHANGE_RATEPRICESW  ( (u32)1 << 13)
#define 	DL645BKGRD_FCHANGE_LADDERESW  	( (u32)1 << 14)
#define 	DL645BKGRD_FCHANGE_HOURFRZ  	( (u32)1 << 15)
#define 	DL645BKGRD_FCHANGE_FL			( (u32)1 << 16)
#define 	DL645BKGRD_FCHANGE_INIT   		( (u32)1 << 17)
#define		DL645BKGRD_FCHANGE_RMYEAR1   	( (u32)1 << 18)
#define		DL645BKGRD_FCHANGE_RMYEAR2   	( (u32)1 << 19)
#define		DL645BKGRD_FCHANGE_RMYEAR3   	( (u32)1 << 20)
#define		DL645BKGRD_FCHANGE_RMYEAR4   	( (u32)1 << 21)
#define		DL645BKGRD_FCHANGE_XiugaiClock   ( (u32)1 << 22)
#define		DL645BKGRD_FCHANGE_T500MS    	 ( (u32)1 << 23)
#define 	DL645BKGRD_FCHANGE_POT			( (u32)1 << 24)
//----------------------------------------------------------
#define End_State           0x0
#define Start_State         0x1
#define Running_State    	0x2

typedef struct {
	u8	Second : 1;
	u8	Minute : 1;
	u8 	Hour : 1;
	u8 	Day : 1;
	u8 	Month : 1;
	u8 	RmDay1 : 1;
	u8 	RmDay2 : 1;
	u8 	RmDay3 : 1;
	u8 	TimeFrz : 1;
	u8 	WinkFrz : 1;
	u8 	DayFrz : 1;
	u8 	TZoneFrz : 1;
	u8 	POTFrz : 1;
	u8 	RatePriceSw : 1;
	u8 	LaddereSw : 1;
	u8 	HourFrz : 1;
	u8 	FL : 1;
	u8	INIT : 1;
	u8	RmYear1 : 1;
	u8	RmYear2 : 1;
	u8	RmYear3 : 1;
	u8	RmYear4 : 1;
	u8 	XiugaiClock: 1;
	u8 	t500ms: 1;
	u8	POT : 1;
} sDl645BkgrdFlagBit_TypeDef;		//��̨״̬��־��	

typedef union {
	u32							Flag;
	sDl645BkgrdFlagBit_TypeDef	FlagBit;		
} sDl645BkgrdFlag_TypeDef;
//----------------------------------------------------------
typedef enum{
	BKGRD_ACTIVE_POWON			= 0x00, //--0=��������
	BKGRD_ACTIVE_POWDN			= 0x01, //--1=����
	BKGRD_ACTIVE_POWUP			= 0x02,//--2=�ϵ�ָ�
} eDl645Bkgrd_Active_TypeDef;
	
//------------------------------------------------------------
typedef struct {
	struct sDl645PubData_TypeDef {
		u8  	Active;       		//0=��������  1=����  2=�ϵ�ָ����� 
		u16   	PownUpingDelay;		//ϵͳ�ϵ���Ҫ��ʱ����֤ǰ����ȷ�����U��I��P��Pf		       
		
		sDl645BkgrdFlag_TypeDef	fChange;	//��̨״̬��־��
		u8      CommStatus; 
		u8 		Second;     			
		u16 	ClockPiece;  
		u16     SecondSysStamp;
	} PubData;

	struct sDl645PriData_TypeDef {
		u8 		Second;			
		u8 		Minute;			
		u8 		Hour;					
		u8 		Day;			
		u8 		Month;			
		u8 		Fl;
		u8   	nPOT;
	} PriData;//1��ǰ������
	
	struct sDl645PriPara_TypeDef {
		u16		Crc;           
	} PriPara;
	
} sDl645Bkgrd_TypeDef;

typedef  struct
{
	struct sDl645PubData_Type
	{             
		u8	ActiveState;//����ʱϵͳ�Ƿ񱻻��ѱ�־
		u8	ActiveInit;  //����ʱ�Ƿ񱻳�ʼ��
		u16	ActiveStamp;//���߻���ʱ��
		u16	InactiveStamp;
		u32  BatteryTimer;//����ʱ�����ʱ���ۼ�	
		u16	 BatteryTimerCrc;
		u8   PowDnSec;	  // �����ٽ��ѹʱ���жϵ���
		u8	PowerErrFlag;	// ��Դ�쳣��־
	}PubData;
	struct sDl645Flag_Type
	{
		u8	ExeFlag;
		u8	SleepFlag;
		u32 EventFlag;
		u8	fChange;		// 01 ��������  02 ���λ
	}InactiveFlag;
}sD645Inactive_TypeDef;

#define PownUpingDelayTime       10              //�ϵ�ָ�������ʱ---10 
//-------------------------------------------------------------
extern void fnDl645Bkgrd_Init(void); 
extern void fnDl645Bkgrd_Exec(void);
extern void fnDl645Bkgrd_Inactive(void);
extern void fnDl645Bkgrd_JudgeAlarm(void);
extern void fnDl645Bkgrd_Sleep(void);
extern void fnDl645Bkgrd_SleepWakeupDisp(void);
extern void fnDl645Bkgrd_SleepRefreshDisp(void);
extern u8 fnDl645Bkgrd_LVDCheck(void);
extern void fnDl645Bkgrd_SleepSet(void);
extern void fnDl645Bkgrd_AfterPowerDn(void);
extern ErrorStatus fnDl645Bkgrd_SetCipherKey(void);
extern	void fnDl645Bkgrd_GetFl(void);
//-------------------------------------------------------------

//-------------------------------------------------------------


#endif // __DL645_BKGRD_H 











