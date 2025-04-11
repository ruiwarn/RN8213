#ifndef  _DL645_ICEsam_H
#define  _DL645_ICEsam_H


#define MAX_ICEsamCom_SIZE 160

typedef struct
{
	u16		EFlag;			//ͨѶ״̬
    u8 		Len;
    u8		Ic_Comm_RxPtr;	// ��������ָ�볤��
    u8		EsamRand[8];	//ESAM 8�ֽ������
    u8		CardRand[8];	//CARD 8�ֽ������
    u8		CardRst8B[8];	//CARD��λ��Ϣ�к�8�ֽ�
    u8		EsamRst8B[8];	// ESAM��λ��Ϣ�к�8�ֽ�
    u8		PcRand[8];		//PCԶ���·��� �����1	8�ֽ�
    u8		PcRst8B[8];		//PCԶ���·��� ��ɢ���� 8�ֽ�
    u32		TimeOutStamp;
    u16		RemoteTime;		// Զ����֤ʱ��
    u16		RemoteDecTime;	// Զ����֤ʱ����пۼ�
    u8 		Ic_Comm_TxBuf[MAX_ICEsamCom_SIZE];
    u8 		Ic_Comm_RxBuf[MAX_ICEsamCom_SIZE];
}sICEsamPack_TypeDef;

typedef struct
{
	u8	CardPricePara1Buf[270]; // ��1�׷����ļ�
	u8	CardPricePara2Buf[270]; // ��1�׷����ļ�
	u8	CardParaBuf[18];		// �������1���������2����ѹ���������
	u8	CardPriceSwitchTime[9];	// ���׷�ʱ�����л�ʱ��
	u8	CardMoneyBag[16];		// Ǯ���ļ� 0-7 Ǯ�����  8-15 �������
	u8	CardUserNo[11];			// �ͻ����
	u8	CardUpdataFlag[5];		// �������±�־λ
}sICEsamReadBuf_TypeDef;		// IC����д���������ݴ�BUF

typedef enum
{
	ICEsam_UpdateFlag_Rata1      = 0x01,
	ICEsam_UpdateFlag_Rata2      = 0x02,
	ICEsam_UpdateFlag_Ladder1    = 0x04,
	ICEsam_UpdateFlag_Ladder2    = 0x08,

	ICEsam_UpdateFlag_Other      = 0x80,
	
	ICEsam_UpdateFlag_None       = 0x00,
}eICEsam_UpdateFlag_TypeDef;			// �������±�־λ

typedef enum
{
	CARD_PROC_OK		= 0x00,
	IC_PowerDn_ERR		= 0x01,		// ��Ƶ�ѹ����
	IC_ESAM_Operate_ERR	= 0x02,		// ����esam����
	IC_ESAM_RST_ERR		= 0x03,		// esam��λ����
	IC_IC_RST_ERR		= 0x04,		// ��Ƭ��λ����
	IC_Auth_Err			= 0x05,		// �����֤����	
	IC_OtherAuth_Err	= 0x06,		// �ⲿ��֤����	
	IC_InvalidCard_Err  = 0x07,		// δ���п�Ƭ
	IC_CardType_Err		= 0x08,     // �����ʹ���
	IC_CardAuth_Err		= 0x09,		// ��Ƭ����δ��Ȩ(��Կ״̬��Ϊ��Կʱ�����Ԥ�ƿ�)
	IC_MAC_Err			= 10,		// MACЧ�����
	IC_MeterNo_Err		= 11,		// ��Ŵ���
	IC_UserNo_Err		= 12,		// �ͻ���Ŵ���
	IC_CardNo_Err		= 13,		// �����кŴ���
	IC_Frame_Err		= 14,		// ���ļ���ʽ���Ϸ�
	IC_BuyCard_Err		= 15,		// ���翨����δ������
	IC_RecuitCar_Err	= 16,		// ��������δ������
	IC_RechargCount_Err = 17,		// ��ֵ��������
	IC_ReturnFile_Err 	= 18,		// ��д�ļ���Ϊ��
	IC_OtherRW_Err		= 19,		// ������ƬͨѶ����
	IC_Pull_Err			= 20,		// ����ǰ�γ�
	IC_Regrate_Err      = 21,		// ����ٶڻ�
}eICEsam_ErrInfo_TypeDef;

extern	ErrorStatus fnICEsam_CardColdReset(u8 CH);
extern	ErrorStatus fnICEsam_ReadRandom( u8 CH, u8 Len );
extern	ErrorStatus fnICEsam_EsamReset( void );
extern	ErrorStatus	fnICEsam_Authorization(u8 *Buf);
extern	ErrorStatus fnICEsam_UpdateRunFile_Clock( void );
extern	ErrorStatus fnICEsam_ReadBin2BinWithMac( u8 Ins, u8 P1, u8 P2, u8 Len );
extern	ErrorStatus fnICEsam_ReadBinFile( u8 CH, u8 P1, u8 P2, u8 Len );
extern	ErrorStatus fnICEsam_UpdatePSW( u8 P1, u8 P2, u8 *Buf );
extern	ErrorStatus fnICEsam_WriteBinFile( u8 CH, u8 P1, u8 P2, u8 Len );
extern	ErrorStatus fnICEsam_Update_ESAM_Bin_Mac( u8 P1, u8 P2, u8 *Buf, u8 Len );
extern	ErrorStatus fnICEsam_RemoteControl( u8 *Buf, u8 Len , u8 P2);
extern	ErrorStatus fnICEsam_ReadRecFile( u8 CH, u8 P1, u8 P2, u8 Len );
extern	ErrorStatus fnICEsam_JugeRegrate(u32 BuyMoney);
extern	ErrorStatus fnIcEsam_AddMoney(u8 CH ,u8 *Buf );
extern	void fnICEsam_Card_Proc( void );
extern	void fndelayETU(u8 i);
extern	ErrorStatus fnICEsam_MoneyProc(void);
extern	ErrorStatus fnICEsam_Auth_In_Out( u8 INS, u8 CH, u8 P2, u8 *BufA );
extern	ErrorStatus fnICEsam_Read_Kx( u8 CH, u8 Len );
extern	ErrorStatus fnICEsam_MoneyBagInit(void);
extern	ErrorStatus fnICEsam_VerifyUpdatePSW(u8 *Buf , u8 Len);
extern	ErrorStatus fnICEsam_SelectFile( u8 CH, u8 F1, u8 F2 );
extern	ErrorStatus fnICEsam_HW_RemoteAuth( u8 *Buf );
extern	ErrorStatus fnICEsam_WriteBinWithMac( u8 CH, u8 P1, u8 P2, u8 Len );
extern	ErrorStatus fnICEsam_RemoteMoneyBagInit(void);
extern	ErrorStatus fnICEsam_UpdateKey( u8 P1);
extern	ErrorStatus fnICEsam_ReturnMoney( u8 *Buf );
extern	void fnICEsam_RxTxCommhead( u8 inc, u8 com, u8 P1, u8 P2, u8 len);
extern	ErrorStatus fnICEsam_CardTxRxFrame( u8 CH, u8 Lc, u8 Le );
extern	ErrorStatus fnICEsam_ReadMoneyBagWithMac( u8 Ins, u8 P1, u8 P2, u8 Len );
extern	ErrorStatus fnICEsam_Update_BackMoneyFile( u8 P1, u8 P2, u8 *Buf, u8 Len );
extern	ErrorStatus fnICEsam_UpdateRunFile(void);
#endif	//_ESAM_H
