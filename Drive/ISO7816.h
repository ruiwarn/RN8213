#ifndef  _ISO7816_H
#define  _ISO7816_H

#ifdef   DRIVE_ISO7816_GLOBALS
	#define  ISO7816_EXT
#else
	#define  ISO7816_EXT	extern 
#endif

#define VERIFY_ERROR 0x01
#define RXDING 0x00
#define TXDING 0x01

//;CPU卡类型命令码集合
//;======================================================================
#define		UserCard			0x01	//;用户卡命令码

#define		UserCardLen			39	//;用户卡长度
#define		InitCardType		0x01	//;初始化卡类型码
#define		UserCardType		0x02	//;普通用户卡类型码
#define		RecuitCardType		0x03	//;补卡类型码


#define		PresetCard			0x06	//;预制卡命令码
#define		PresetCardLen		26	//;预制卡长度

typedef struct
{
    u8	EFlag;			// 卡插入处理标志，有此标志时进行卡处理，卡操作完成后退出
}sISO7816ComBuf_TypeDef;

typedef enum
{
	ESAM		= 0x00,
	CARD		= 0x01
}eICEsamSelFlag_TypeDef;

ISO7816_EXT	sISO7816ComBuf_TypeDef I7816ComExt;

extern	ErrorStatus fnCardSendByte(u8 CH, u8 Data);
extern	ErrorStatus fnCardGetByte(u8 CH,  u8 *Data );
extern	void fnISO7816_Init(void);
#endif


