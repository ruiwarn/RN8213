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

//;CPU�����������뼯��
//;======================================================================
#define		UserCard			0x01	//;�û���������

#define		UserCardLen			39	//;�û�������
#define		InitCardType		0x01	//;��ʼ����������
#define		UserCardType		0x02	//;��ͨ�û���������
#define		RecuitCardType		0x03	//;����������


#define		PresetCard			0x06	//;Ԥ�ƿ�������
#define		PresetCardLen		26	//;Ԥ�ƿ�����

typedef struct
{
    u8	EFlag;			// �����봦���־���д˱�־ʱ���п�������������ɺ��˳�
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


