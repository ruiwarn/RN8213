

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
} sDl645BkgrdFlagBit_TypeDef;		//后台状态标志字	

typedef union {
	u32							Flag;
	sDl645BkgrdFlagBit_TypeDef	FlagBit;		
} sDl645BkgrdFlag_TypeDef;
//----------------------------------------------------------
typedef enum{
	BKGRD_ACTIVE_POWON			= 0x00, //--0=正常工作
	BKGRD_ACTIVE_POWDN			= 0x01, //--1=修眠
	BKGRD_ACTIVE_POWUP			= 0x02,//--2=上电恢复
} eDl645Bkgrd_Active_TypeDef;
	
//------------------------------------------------------------
typedef struct {
	struct sDl645PubData_TypeDef {
		u8  	Active;       		//0=正常工作  1=修眠  2=上电恢复过程 
		u16   	PownUpingDelay;		//系统上电需要延时，保证前端正确计算出U、I、P、Pf		       
		
		sDl645BkgrdFlag_TypeDef	fChange;	//后台状态标志字
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
	} PriData;//1秒前的数据
	
	struct sDl645PriPara_TypeDef {
		u16		Crc;           
	} PriPara;
	
} sDl645Bkgrd_TypeDef;

typedef  struct
{
	struct sDl645PubData_Type
	{             
		u8	ActiveState;//休眠时系统是否被唤醒标志
		u8	ActiveInit;  //唤醒时是否被初始化
		u16	ActiveStamp;//休眠唤醒时戳
		u16	InactiveStamp;
		u32  BatteryTimer;//休眠时，电池时间累计	
		u16	 BatteryTimerCrc;
		u8   PowDnSec;	  // 低于临界电压时间判断掉电
		u8	PowerErrFlag;	// 电源异常标志
	}PubData;
	struct sDl645Flag_Type
	{
		u8	ExeFlag;
		u8	SleepFlag;
		u32 EventFlag;
		u8	fChange;		// 01 按键按下  02 秒进位
	}InactiveFlag;
}sD645Inactive_TypeDef;

#define PownUpingDelayTime       10              //上电恢复过程延时---10 
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











