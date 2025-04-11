
#ifndef _SCOM_H
#define _SCOM_H

#ifdef   _SCOM_GLOBALS
	#define  SCOM_EXT
#else
	#define  SCOM_EXT  extern 
#endif


#define MAX_COMPACK_SIZE	256

typedef enum
{
	SCOMPK_EFLAG_IDLE , 	
	SCOMPK_EFLAG_RXHEAD1,
	SCOMPK_EFLAG_RXHEAD2,
	SCOMPK_EFLAG_RXDATALEN,
	SCOMPK_EFLAG_RXEND, 
	SCOMPK_EFLAG_RXFINISH , 
	SCOMPK_EFLAG_ANALYSE , 
	SCOMPK_EFLAG_TXSTART , 
	SCOMPK_EFLAG_TXBUSY , 
	SCOMPK_EFLAG_TXFINISH , 
	SCOMPK_EFLAG_INIT 		
} eScomFlag_TypeDef;

typedef enum
{
	PTTYEP_DL645A	= 1,
	PTTYEP_DL645B	= 2,
	PTTYEP_PT698	= 3,
	PTTYEP_NONE		= 0xFF	
} ePtType_TypeDef;


typedef enum
{
	SCOM_PORT_RS485A = 0,	//第一个Rs485通信口
	SCOM_PORT_ZBCOM = 1,	// 载波通讯口
	SCOM_PORT_IR	= 2,	//红外通信口	
	SCOM_PORT_MAX	= 3		
} eScomPort_TypeDef;

#pragma pack(1)

typedef struct
{
	u8 	Head1;
	u8 	Addr[6];
	u8 	Head2;
	
//	u8 Ctl;
	u8	AFN : 5;
	u8	Persist : 1;	
	u8	Ack : 1;
	u8	Dir : 1;	
	
	u8 Len;
	
	struct	{
		u8	DI0;
		u8	DI1;
		u8	DI2;
		u8	DI3;		
		u8	Dat[MAX_COMPACK_SIZE - 16];
	} UDat;
			
	u8	Cs;
	u8	Tail;

} sFrmDl645B_TypeDef;	//DL645-2007协议帧结构

#pragma pack()



typedef struct
{
	u16		EFlag;			//通讯状态
	
    u16 	RxLen;
    u16  	TxLen;
    u32		TimeOutStamp;
    u8 		*pTx;
    
    u8		fBps;				//波特率变更标志
    u8		NewBps;				//新波特率
    u32 	NewBpsStamp;		//新速率时标
    
    u8 		RxBuf[MAX_COMPACK_SIZE];
    u8		TxBuf[MAX_COMPACK_SIZE];
}sComPack_TypeDef;







//----------------
extern void fnScomPk_TxEnDis(u8 ComPort , FunctionalState EnOrDis);  //收发控制DIR
extern void fnScomPk_Init(u8 ComPort);  //通讯包初始化
extern void fnScomPk_ChangeBps(u8 ComPort);   //修改波特率
extern void fnScomPk_Exec(u8 ComPort);  //通讯包执行--
extern u8 *fnPt_Check(u8 *Src , u16 Len , ePtType_TypeDef *PtType);  //帧格式检测
extern u16 fnPt_Exec(u8 ComPort);  //协议执行
extern void fnScom_AllInit(void);
//-------------------------------

#endif // _SCOM_H 

//-------------------------------





















