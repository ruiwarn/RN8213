#ifndef _DL645_FRONT_H
#define _DL645_FRONT_H

#ifdef   _DL645_FRONT_GLOBALS
	#define  DL645FRONT_EXT
#else
	#define  DL645FRONT_EXT  extern 
#endif

typedef enum{
	POSITIVE		= 0x00,    //正向
	INVERSION		= 0x01    //反向
} eDl645Front_Direction_TypeDef;//单相表只要枚举正反向，根据正反向来处理电能

typedef enum{
	FRONT_FLAG_INIT			= 0x01,	
	FRONT_FLAG_RN8209RST	= 0x02	
} eDl645Front_Flag_TypeDef;


typedef struct
{
    u8		ChkErrCnt;
    u32 	Pw[2];   		    //pa,pb   
    u32 	UI[3];          // Ia=UI[0] Inal U         
    u16 	Frequency;   	//电网频率，单位：                            	
    u32		Pulse;		    //前台脉冲
    u16     Pstart;
    //---电能脉冲---	
    u32		Pulse_Eg;	    //脉冲个数
    u32 	PDirect;		//功率方向
    u32 	ChkSum1;		//读出EMU的校表参数校验
} sDl645FrontTmp_TypeDef;

typedef struct
{	
	struct sDl645FrontPubData_TypeDef  {
		u16		U;			    //---电压---NNN.N
		u32	    Ia;			    //---电流NNNN.NNNN(电流值要求3整3小，整定值要求2整4小，最高位表示方向)---		
		u32	    In;             //---零线电流
		sDF09	Pw;			    //---瞬时有功p
		u16		Pf;			    //---功率因数N.NNN---	最高位表示方向{Pf Pfa Pfb Pfc}	
		u16		Angle;		    //---相角NNN.N---		
		u16		Frequency;		//---频率NN.NN
		u32		PPwave;			//---一分钟平均功率NN.NNNN
		u8      Chnsel;
		u16		Temperature;	//---NNN.N  温度
		u16		ClockBat;		//---NN.NN  电池电压
		u32		tWorkBat;		//---NNNN  时钟工作时间（分钟）
		u8		PDirect;		//---原功率方向
		
	    u16    	CfIn; 			//脉冲输入电平判断
	    u8    	CfTime;			//
	    u8    	Step;
	    u16   	FrontStamp;
	} PubData;
	
	struct sDl645FrontPriData_TypeDef  {		
		u8		Flag;			//---工作异常标志---
	} PriData;	
	
	struct sDl645FrontPriPara_TypeDef  {		
		u32	 PConstE;			//有功常数
		u16	 Crc;
	} PriPara;		
} sDl645Front_TypeDef;	

extern	void fnDl645Front_Init(void);				//上电初始化校表参数，写入EMU
extern	void fnDl645Front_Exec(void);               //EMU处理函数：输出脉冲数、瞬变量、电池电压
extern  void fnFront_TemperIAGain(void);
extern	void fnFront_CurMaxPGain(void);
#endif
