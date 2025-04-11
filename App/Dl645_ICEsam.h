#ifndef  _DL645_ICEsam_H
#define  _DL645_ICEsam_H


#define MAX_ICEsamCom_SIZE 160

typedef struct
{
	u16		EFlag;			//通讯状态
    u8 		Len;
    u8		Ic_Comm_RxPtr;	// 接受数据指针长度
    u8		EsamRand[8];	//ESAM 8字节随机数
    u8		CardRand[8];	//CARD 8字节随机数
    u8		CardRst8B[8];	//CARD复位信息中后8字节
    u8		EsamRst8B[8];	// ESAM复位信息中后8字节
    u8		PcRand[8];		//PC远程下发的 随机数1	8字节
    u8		PcRst8B[8];		//PC远程下发的 分散因子 8字节
    u32		TimeOutStamp;
    u16		RemoteTime;		// 远程认证时间
    u16		RemoteDecTime;	// 远程认证时间进行扣减
    u8 		Ic_Comm_TxBuf[MAX_ICEsamCom_SIZE];
    u8 		Ic_Comm_RxBuf[MAX_ICEsamCom_SIZE];
}sICEsamPack_TypeDef;

typedef struct
{
	u8	CardPricePara1Buf[270]; // 第1套费率文件
	u8	CardPricePara2Buf[270]; // 第1套费率文件
	u8	CardParaBuf[18];		// 报警金额1，报警金额2，电压、电流变比
	u8	CardPriceSwitchTime[9];	// 两套分时费率切换时间
	u8	CardMoneyBag[16];		// 钱包文件 0-7 钱包金额  8-15 购电次数
	u8	CardUserNo[11];			// 客户编号
	u8	CardUpdataFlag[5];		// 参数更新标志位
}sICEsamReadBuf_TypeDef;		// IC卡读写操作数据暂存BUF

typedef enum
{
	ICEsam_UpdateFlag_Rata1      = 0x01,
	ICEsam_UpdateFlag_Rata2      = 0x02,
	ICEsam_UpdateFlag_Ladder1    = 0x04,
	ICEsam_UpdateFlag_Ladder2    = 0x08,

	ICEsam_UpdateFlag_Other      = 0x80,
	
	ICEsam_UpdateFlag_None       = 0x00,
}eICEsam_UpdateFlag_TypeDef;			// 参数更新标志位

typedef enum
{
	CARD_PROC_OK		= 0x00,
	IC_PowerDn_ERR		= 0x01,		// 表计电压过低
	IC_ESAM_Operate_ERR	= 0x02,		// 操作esam错误
	IC_ESAM_RST_ERR		= 0x03,		// esam复位错误
	IC_IC_RST_ERR		= 0x04,		// 卡片复位错误
	IC_Auth_Err			= 0x05,		// 身份认证错误	
	IC_OtherAuth_Err	= 0x06,		// 外部认证错误	
	IC_InvalidCard_Err  = 0x07,		// 未发行卡片
	IC_CardType_Err		= 0x08,     // 卡类型错误
	IC_CardAuth_Err		= 0x09,		// 卡片操作未授权(密钥状态不为公钥时插参数预制卡)
	IC_MAC_Err			= 10,		// MAC效验错误
	IC_MeterNo_Err		= 11,		// 表号错误
	IC_UserNo_Err		= 12,		// 客户编号错误
	IC_CardNo_Err		= 13,		// 卡序列号错误
	IC_Frame_Err		= 14,		// 卡文件格式不合法
	IC_BuyCard_Err		= 15,		// 购电卡插入未开户表
	IC_RecuitCar_Err	= 16,		// 补卡插入未开户表
	IC_RechargCount_Err = 17,		// 充值次数错误
	IC_ReturnFile_Err 	= 18,		// 返写文件不为空
	IC_OtherRW_Err		= 19,		// 操作卡片通讯错误
	IC_Pull_Err			= 20,		// 卡提前拔出
	IC_Regrate_Err      = 21,		// 购电操囤积
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
