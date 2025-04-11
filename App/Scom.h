
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
	SCOM_PORT_RS485A = 0,	//��һ��Rs485ͨ�ſ�
	SCOM_PORT_ZBCOM = 1,	// �ز�ͨѶ��
	SCOM_PORT_IR	= 2,	//����ͨ�ſ�	
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

} sFrmDl645B_TypeDef;	//DL645-2007Э��֡�ṹ

#pragma pack()



typedef struct
{
	u16		EFlag;			//ͨѶ״̬
	
    u16 	RxLen;
    u16  	TxLen;
    u32		TimeOutStamp;
    u8 		*pTx;
    
    u8		fBps;				//�����ʱ����־
    u8		NewBps;				//�²�����
    u32 	NewBpsStamp;		//������ʱ��
    
    u8 		RxBuf[MAX_COMPACK_SIZE];
    u8		TxBuf[MAX_COMPACK_SIZE];
}sComPack_TypeDef;







//----------------
extern void fnScomPk_TxEnDis(u8 ComPort , FunctionalState EnOrDis);  //�շ�����DIR
extern void fnScomPk_Init(u8 ComPort);  //ͨѶ����ʼ��
extern void fnScomPk_ChangeBps(u8 ComPort);   //�޸Ĳ�����
extern void fnScomPk_Exec(u8 ComPort);  //ͨѶ��ִ��--
extern u8 *fnPt_Check(u8 *Src , u16 Len , ePtType_TypeDef *PtType);  //֡��ʽ���
extern u16 fnPt_Exec(u8 ComPort);  //Э��ִ��
extern void fnScom_AllInit(void);
//-------------------------------

#endif // _SCOM_H 

//-------------------------------





















