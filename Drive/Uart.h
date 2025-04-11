
#ifndef _UART_H
#define _UART_H

#ifdef   DRIVE_UART_GLOBALS
	#define  UART_EXT
#else
	#define  UART_EXT	extern 
#endif

//#include <includes.h>

//扩展串口1 红外(使用TIM2 EXTI1 TIM3_CH3)
//扩展串口2 RS485被抄(使用TIM4 EXTI9_5)
//扩展串口3 RS485抄表(使用TIM5 EXTI15_10)
//-------------------
//==========================================
typedef struct
{
	u16 SlipTime;
	u8 	Mode;	
	u8	TimEn : 1;	//0定时器未开，1已打开
	u8	PWM : 1;	//0PWM未启动，1已启动
		
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
	u8	RxDATA[4];			//暂存接收OK或待发送数据,遥控器编码数据
  	u8	RxBitCounter;  		//比特接收计数器
	u8	RxCounter;			//接收字节计数
	u32	IRStamp;			//接收时戳
	u8  Over;				//接收到遥控器编码后置1，显示完成后清除
	u8  Flag;				//红外通讯，遥控器标志
	u8  RXFlag;			    //收到遥控器码数据正确标志，在显示主程序中点亮背光
	u8  mode;				//模式=1 进入轮显，按数字键则是规定显示的数据项，=0 根据标识符进行显示
	u8  DigitPosition;		// 按编程键设置标识编码
	u8  UpDownFlag;			// 设置时闪锁编码
	u8  MeyKey;				// 红外显示键值
	u16	TimeOutStamp;		// 延时时间清除
} sIRRemote_TypeDef;

typedef struct
{
	u8 TxRxDATA;			//暂存接收OK或待发送数据
    u8 Status;				//状态: 0守侯 1接收中 2接收完成 3发送中 4发送完成
    u8 TxRxCounter;  		//比特发送接收计数器
  	u8 OddEvenCheck;		//效验位检查
  	u8 RxFlag;				//接收标志
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

//扩展串口1 红外(使用INTTM51 T0H1 INTP2)
//---------------------------------------------------
//#define START_38K { TMHE1=1; } //38K定时器开始计数;
//#define STOP_38K  { TMHE1=0; } //38K定时器停止计数;
//---------------------------------------------------
//#define TimerStart_UsartExt1Tx(x) ( x ? (TMHE1=1) : (TMHE1=0) )//1:打开38k定时器0:关闭38k定时器;

/********************TimerA扩展UART寄存器******************************/
#define TimeraIdle          0
#define TimerIRRxing        1
#define TimerIRRxfinish    2
#define TimerIRTxing        3
#define TimerIRTxfinish   4
#define TxCharBits           11             //发送时，需发送11个比特数据-----1个起始位，8个数据位，1个呕校验位，1个停止位

#define OddCheckOk  0x80


typedef enum {USART_IT_RX= 0, USART_IT_TX = !USART_IT_RX} FunctionalMODE;


typedef enum
{
	Parity_No 	= 0,
	Parity_Odd 	= 1,
	Parity_Even = 2
}Parity_enum;

//===串口定义================================

typedef enum
{
	USART_EVEN_PARITY 	= 0x08,	//校验位
	USART_ODD_PARITY  	= 0x0C,
	USART_NO_PARITY  	= 0x00, 
	
	USART_1STOP_SBIT   	= 0x00,	//停止位
	USART_2STOP_SBIT   	= 0x10,
	
	USART_5BIT_DAT  	= 0x00,	//数据位
	USART_6BIT_DAT		= 0x01,
	USART_7BIT_DAT  	= 0x02,
	USART_8BIT_DAT		= 0x03,
	
	USART_BPS_300 		= 0x00,	//波特率
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









