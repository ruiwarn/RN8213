
#ifndef _UART_H
#define _UART_H

#ifdef   DRIVE_UART_GLOBALS
	#define  UART_EXT
#else
	#define  UART_EXT	extern 
#endif

//#include <includes.h>

//��չ����1 ����(ʹ��TIM2 EXTI1 TIM3_CH3)
//��չ����2 RS485����(ʹ��TIM4 EXTI9_5)
//��չ����3 RS485����(ʹ��TIM5 EXTI15_10)
//-------------------
//==========================================
typedef struct
{
	u16 SlipTime;
	u8 	Mode;	
	u8	TimEn : 1;	//0��ʱ��δ����1�Ѵ�
	u8	PWM : 1;	//0PWMδ������1������
		
	u8 RxStatus;
	u8 RxBitCnt;
	u8 RxShift;
	u16 RxSlipTime;

	u8 TxStatus;
	u8 TxBitCnt;
	u8 TxShift;
	u16 TxSlipTime;
} sUsartExt_TypeDef;

typedef struct
{
	u8	RxDATA[4];			//�ݴ����OK�����������,ң������������
  	u8	RxBitCounter;  		//���ؽ��ռ�����
	u8	RxCounter;			//�����ֽڼ���
	u32	IRStamp;			//����ʱ��
	u8  Over;				//���յ�ң�����������1����ʾ��ɺ����
	u8  Flag;				//����ͨѶ��ң������־
	u8  RXFlag;			    //�յ�ң������������ȷ��־������ʾ�������е�������
	u8  mode;				//ģʽ=1 �������ԣ������ּ����ǹ涨��ʾ�������=0 ���ݱ�ʶ��������ʾ
	u8  DigitPosition;		// ����̼����ñ�ʶ����
	u8  UpDownFlag;			// ����ʱ��������
	u8  MeyKey;				// ������ʾ��ֵ
	u16	TimeOutStamp;		// ��ʱʱ�����
} sIRRemote_TypeDef;

typedef struct
{
	u8 TxRxDATA;			//�ݴ����OK�����������
    u8 Status;				//״̬: 0�غ� 1������ 2������� 3������ 4�������
    u8 TxRxCounter;  		//���ط��ͽ��ռ�����
  	u8 OddEvenCheck;		//Ч��λ���
  	u8 RxFlag;				//���ձ�־
}sIRComExt_TypeDef;

typedef enum
{
	IRComIdle			= 0x00,
	IRComRxing			= 0x01,
	IRComRxfinish		= 0x02,
	IRComTxing			= 0x03,
	IRComTxfinish		= 0x04,
}eIRComFlag_TypeDef;

UART_EXT	sUsartExt_TypeDef	UsartExt1;
UART_EXT	sIRRemote_TypeDef	Remote;
UART_EXT	sIRComExt_TypeDef	IRComExt;

//��չ����1 ����(ʹ��INTTM51 T0H1 INTP2)
//---------------------------------------------------
//#define START_38K { TMHE1=1; } //38K��ʱ����ʼ����;
//#define STOP_38K  { TMHE1=0; } //38K��ʱ��ֹͣ����;
//---------------------------------------------------
//#define TimerStart_UsartExt1Tx(x) ( x ? (TMHE1=1) : (TMHE1=0) )//1:��38k��ʱ��0:�ر�38k��ʱ��;

/********************TimerA��չUART�Ĵ���******************************/
#define TimeraIdle          0
#define TimerIRRxing        1
#define TimerIRRxfinish    2
#define TimerIRTxing        3
#define TimerIRTxfinish   4
#define TxCharBits           11             //����ʱ���跢��11����������-----1����ʼλ��8������λ��1��ŻУ��λ��1��ֹͣλ

#define OddCheckOk  0x80


typedef enum {USART_IT_RX= 0, USART_IT_TX = !USART_IT_RX} FunctionalMODE;


typedef enum
{
	Parity_No 	= 0,
	Parity_Odd 	= 1,
	Parity_Even = 2
}Parity_enum;

//===���ڶ���================================

typedef enum
{
	USART_EVEN_PARITY 	= 0x08,	//У��λ
	USART_ODD_PARITY  	= 0x0C,
	USART_NO_PARITY  	= 0x00, 
	
	USART_1STOP_SBIT   	= 0x00,	//ֹͣλ
	USART_2STOP_SBIT   	= 0x10,
	
	USART_5BIT_DAT  	= 0x00,	//����λ
	USART_6BIT_DAT		= 0x01,
	USART_7BIT_DAT  	= 0x02,
	USART_8BIT_DAT		= 0x03,
	
	USART_BPS_300 		= 0x00,	//������
	USART_BPS_600 		= 0x20,
	USART_BPS_1200 		= 0x40,
	USART_BPS_2400 		= 0x60,
	USART_BPS_4800 		= 0x80,
	USART_BPS_7200 		= 0xa0,
	USART_BPS_9600 		= 0xc0,	
	USART_BPS_19200 	= 0xe0				
} eUsartPara_TypeDef;



extern void fnUSART_IRCom_Init(u8 Cfg);
extern void fnUSART_RS485_Init(u8 Cfg);
extern void fnUSART_ZBCOM_Init(u8 Cfg);
extern void USART_ITConfig(u8 ComPort, FunctionalMODE USART_IT, FunctionalState NewState);


#endif	//_UART_H









