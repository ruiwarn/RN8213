

#define  DRIVE_UART_GLOBALS
#include <Drive_Lib.h>

#include <Scom.h> 
extern sComPack_TypeDef 	ComPack[SCOM_PORT_MAX];	//在Dl645_Globals.h中定义。
				
//===(红外)================================
void fnUSART_IRCom_Init(u8 Cfg)
{  
	u32	ClkDiv;
	IRComExt.RxFlag=0;
	memset(&IRComExt,0,sizeof(sIRComExt_TypeDef));
	ClkDiv = fnDl645MainClock_Get();
	switch(Cfg & 0xE0)
	{
	  	case USART_BPS_600:		//600
			ClkDiv=ClkDiv/(600*16)-1;
			break;
	  	case USART_BPS_1200:	//1200
			ClkDiv=ClkDiv/(1200*16)-1;
			break;
		case USART_BPS_2400:	//2400
			ClkDiv=ClkDiv/(2400*16)-1;
			break;
		case USART_BPS_4800:	//4800
			ClkDiv=ClkDiv/(4800*16)-1;
			break;
		case USART_BPS_9600:	//9600
			ClkDiv=ClkDiv/(9600*16)-1;
			break;
		default:	
			ClkDiv=ClkDiv/(2400*16)-1;		//2400
			break;
	}
	//UART0用于RS485口1通讯.
	UART1->BAUD = ClkDiv;
	UART1->CTRL = (1 << 0)	|   /* uart enable */
                  (3 << 5)	|	/* data bit: 8 */
                  (2 << 7);	/* even  */
	//GPIO->PCA0 |=0x04000000;		//GPIO选择UART0
	PinMode_IRRX(GPIO_MODE_IN);
	PinMode_IRTX(GPIO_MODE_OUT);
	//
	return;
}
//---------------------------------------------------


void fnUSART_RS485_Init(u8 Cfg)
{
	u32	ClkDiv;
	ClkDiv = fnDl645MainClock_Get();
	switch(Cfg & 0xE0)
	{
	  	case USART_BPS_600:		//600
			ClkDiv=ClkDiv/(600*16)-1;
			break;
	  	case USART_BPS_1200:	//1200
			ClkDiv=ClkDiv/(1200*16)-1;
			break;
		case USART_BPS_2400:	//2400
			ClkDiv=ClkDiv/(2400*16)-1;
			break;
		case USART_BPS_4800:	//4800
			ClkDiv=ClkDiv/(4800*16)-1;
			break;
		case USART_BPS_9600:	//9600
			ClkDiv=ClkDiv/(9600*16)-1;
			break;
		default:	
			ClkDiv=ClkDiv/(2400*16)-1;			//2400
			break;
	}
	//UART0用于RS485口1通讯.
	UART0->BAUD = ClkDiv;
	UART0->CTRL = (1 << 0)	|   /* uart enable */
                  (3 << 5)	|	/* data bit: 8 */
                  (2 << 7);	/* even  */
	//GPIO->PCA0 |=0x02000000;		//GPIO选择UART0
	PinMode_RS485RX(GPIO_MODE_IN);
	PinMode_RS485TX(GPIO_MODE_OUT);
	return;
}

void fnUSART_ZBCOM_Init(u8 Cfg)
{
	u32	ClkDiv;
	ClkDiv = fnDl645MainClock_Get();
	switch(Cfg & 0xE0)
	{
	  	case USART_BPS_600:		//600
			ClkDiv=ClkDiv/(600*16)-1;
			break;
	  	case USART_BPS_1200:	//1200
			ClkDiv=ClkDiv/(1200*16)-1;
			break;
		case USART_BPS_2400:	//2400
			ClkDiv=ClkDiv/(2400*16)-1;
			break;
		case USART_BPS_4800:	//4800
			ClkDiv=ClkDiv/(4800*16)-1;
			break;
		case USART_BPS_9600:	//9600
			ClkDiv=ClkDiv/(9600*16)-1;
			break;
		default:	
			ClkDiv=ClkDiv/(2400*16)-1;			//2400
			break;
	}
	//UART3用于载波通讯.
	UART3->BAUD = ClkDiv;
	UART3->CTRL = (1 << 0)	|   /* uart enable */
                  (3 << 5)	|	/* data bit: 8 */
                  (2 << 7);	/* even  */
	//GPIO->PCA0 |=0x10000000;		//GPIO选择UART3
	PinMode_ZBRX(GPIO_MODE_IN);
	PinMode_ZBTX(GPIO_MODE_OUT);
	return;
}

//---------------------------------------------------
//各个端口管理工作方式控制。
void USART_ITConfig(u8 ComPort, FunctionalMODE USART_IT, FunctionalState NewState)
{
	u8 Compose;

	Compose=(ComPort<<2)|(USART_IT<<1)|(NewState);
	switch(Compose)
	{
  	case (SCOM_PORT_IR<<2)|(USART_IT_RX<<1)|DISABLE:	//红外通信口|接收|关闭
		UART1->CTRL &=0xfff3; 	//屏蔽接收中断,屏蔽接收错误中断
		UART1->STA = 0x3d;		//清接收中断标志及接收错误标志;  	//禁止中断服务
		
		break;
  	case (SCOM_PORT_IR<<2)|(USART_IT_RX<<1)|ENABLE:	//红外通信口|接收|打开
		UART1->CTRL |=0x0c; 	//允许接收中断,允许接收错误中断
		UART1->STA = 0x3d;		//清接收中断标志及接收错误标志
		INTC->STA |= 0x02;			/* INTP1 IF clear */
		INTC->MASK |= 0x02;					/* INTP1 disable */
		IRComExt.Status=IRComRxfinish;
		Remote.Flag=0;
		break;
  	case (SCOM_PORT_IR<<2)|(USART_IT_TX<<1)|DISABLE:	//红外通信口|发送|关闭
		UART1->CTRL &=0xfbfd;	//屏蔽发送中断
		UART1->STA = 0x02;		//清发送中断标志
		break;
  	case (SCOM_PORT_IR<<2)|(USART_IT_TX<<1)|ENABLE:	//红外通信口|发送|打开
		UART1->CTRL |=0x0402;		//屏蔽发送中断
		UART1->STA = 0x02;		//清发送中断标志
		UART1->TXD=*(ComPack[SCOM_PORT_IR].pTx++);
	  	ComPack[SCOM_PORT_IR].TxLen--;
		
		break;
//-------------------------
	case (SCOM_PORT_RS485A<<2)|(USART_IT_RX<<1)|DISABLE:	//第一个Rs485通信口|接收|关闭
		UART0->CTRL &=0xfff3; 	//屏蔽接收中断,屏蔽接收错误中断
		UART0->STA = 0x3d;		//清接收中断标志及接收错误标志
		break;
	case (SCOM_PORT_RS485A<<2)|(USART_IT_RX<<1)|ENABLE:	//第一个Rs485通信口|接收|打开
		UART0->CTRL |=0x0c; 	//允许接收中断,允许接收错误中断
		UART0->STA = 0x3d;		//清接收中断标志及接收错误标志
		
//		// DAM TEST
//		DMA->IE = 0;      //禁止中断
//  		DMA->C0CTRL = 0;
//  		DMA->C0SRC = 0x40008010;		// uart2 buf
//  		DMA->C0DST = 0x10000118;		// uart buf
//  		DMA->C0CTRL = 0x40000341;
		
		break;
	case (SCOM_PORT_RS485A<<2)|(USART_IT_TX<<1)|DISABLE:	//第一个Rs485通信口|发送|关闭
		UART0->CTRL &=0xfffd;		//屏蔽发送中断
		UART0->STA = 0x02;		//清发送中断标志
		break;
	case (SCOM_PORT_RS485A<<2)|(USART_IT_TX<<1)|ENABLE:	//第一个Rs485通信口|发送|打开
		UART0->CTRL |=0x02;		//屏蔽发送中断
		UART0->STA = 0x02;		//清发送中断标志

	  	UART0->TXD=*(ComPack[SCOM_PORT_RS485A].pTx++);
	  	ComPack[SCOM_PORT_RS485A].TxLen--;
		break;
		
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_RX<<1)|DISABLE:	//载波通信口|接收|关闭
		UART3->CTRL &=0xfff3; 	//屏蔽接收中断,屏蔽接收错误中断
		UART3->STA = 0x3d;		//清接收中断标志及接收错误标志
		break;
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_RX<<1)|ENABLE:	//载波通信口|接收|打开
		UART3->CTRL |=0x0c; 	//允许接收中断,允许接收错误中断
		UART3->STA = 0x3d;		//清接收中断标志及接收错误标志
		break;
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_TX<<1)|DISABLE:	//载波通信口|发送|关闭
		UART3->CTRL &=0xfffd;		//屏蔽发送中断
		UART3->STA = 0x02;		//清发送中断标志
		break;
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_TX<<1)|ENABLE:	//载波通信口|发送|打开
		UART3->CTRL |=0x02;		//屏蔽发送中断
		UART3->STA = 0x02;		//清发送中断标志

	  	UART3->TXD=*(ComPack[SCOM_PORT_ZBCOM].pTx++);
	  	ComPack[SCOM_PORT_ZBCOM].TxLen--;
		break;
	default: return;
	}
	return;
}


