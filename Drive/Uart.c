

#define  DRIVE_UART_GLOBALS
#include <Drive_Lib.h>

#include <Scom.h> 
extern sComPack_TypeDef 	ComPack[SCOM_PORT_MAX];	//��Dl645_Globals.h�ж��塣
				
//===(����)================================
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
	//UART0����RS485��1ͨѶ.
	UART1->BAUD = ClkDiv;
	UART1->CTRL = (1 << 0)	|   /* uart enable */
                  (3 << 5)	|	/* data bit: 8 */
                  (2 << 7);	/* even  */
	//GPIO->PCA0 |=0x04000000;		//GPIOѡ��UART0
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
	//UART0����RS485��1ͨѶ.
	UART0->BAUD = ClkDiv;
	UART0->CTRL = (1 << 0)	|   /* uart enable */
                  (3 << 5)	|	/* data bit: 8 */
                  (2 << 7);	/* even  */
	//GPIO->PCA0 |=0x02000000;		//GPIOѡ��UART0
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
	//UART3�����ز�ͨѶ.
	UART3->BAUD = ClkDiv;
	UART3->CTRL = (1 << 0)	|   /* uart enable */
                  (3 << 5)	|	/* data bit: 8 */
                  (2 << 7);	/* even  */
	//GPIO->PCA0 |=0x10000000;		//GPIOѡ��UART3
	PinMode_ZBRX(GPIO_MODE_IN);
	PinMode_ZBTX(GPIO_MODE_OUT);
	return;
}

//---------------------------------------------------
//�����˿ڹ�������ʽ���ơ�
void USART_ITConfig(u8 ComPort, FunctionalMODE USART_IT, FunctionalState NewState)
{
	u8 Compose;

	Compose=(ComPort<<2)|(USART_IT<<1)|(NewState);
	switch(Compose)
	{
  	case (SCOM_PORT_IR<<2)|(USART_IT_RX<<1)|DISABLE:	//����ͨ�ſ�|����|�ر�
		UART1->CTRL &=0xfff3; 	//���ν����ж�,���ν��մ����ж�
		UART1->STA = 0x3d;		//������жϱ�־�����մ����־;  	//��ֹ�жϷ���
		
		break;
  	case (SCOM_PORT_IR<<2)|(USART_IT_RX<<1)|ENABLE:	//����ͨ�ſ�|����|��
		UART1->CTRL |=0x0c; 	//��������ж�,������մ����ж�
		UART1->STA = 0x3d;		//������жϱ�־�����մ����־
		INTC->STA |= 0x02;			/* INTP1 IF clear */
		INTC->MASK |= 0x02;					/* INTP1 disable */
		IRComExt.Status=IRComRxfinish;
		Remote.Flag=0;
		break;
  	case (SCOM_PORT_IR<<2)|(USART_IT_TX<<1)|DISABLE:	//����ͨ�ſ�|����|�ر�
		UART1->CTRL &=0xfbfd;	//���η����ж�
		UART1->STA = 0x02;		//�巢���жϱ�־
		break;
  	case (SCOM_PORT_IR<<2)|(USART_IT_TX<<1)|ENABLE:	//����ͨ�ſ�|����|��
		UART1->CTRL |=0x0402;		//���η����ж�
		UART1->STA = 0x02;		//�巢���жϱ�־
		UART1->TXD=*(ComPack[SCOM_PORT_IR].pTx++);
	  	ComPack[SCOM_PORT_IR].TxLen--;
		
		break;
//-------------------------
	case (SCOM_PORT_RS485A<<2)|(USART_IT_RX<<1)|DISABLE:	//��һ��Rs485ͨ�ſ�|����|�ر�
		UART0->CTRL &=0xfff3; 	//���ν����ж�,���ν��մ����ж�
		UART0->STA = 0x3d;		//������жϱ�־�����մ����־
		break;
	case (SCOM_PORT_RS485A<<2)|(USART_IT_RX<<1)|ENABLE:	//��һ��Rs485ͨ�ſ�|����|��
		UART0->CTRL |=0x0c; 	//��������ж�,������մ����ж�
		UART0->STA = 0x3d;		//������жϱ�־�����մ����־
		
//		// DAM TEST
//		DMA->IE = 0;      //��ֹ�ж�
//  		DMA->C0CTRL = 0;
//  		DMA->C0SRC = 0x40008010;		// uart2 buf
//  		DMA->C0DST = 0x10000118;		// uart buf
//  		DMA->C0CTRL = 0x40000341;
		
		break;
	case (SCOM_PORT_RS485A<<2)|(USART_IT_TX<<1)|DISABLE:	//��һ��Rs485ͨ�ſ�|����|�ر�
		UART0->CTRL &=0xfffd;		//���η����ж�
		UART0->STA = 0x02;		//�巢���жϱ�־
		break;
	case (SCOM_PORT_RS485A<<2)|(USART_IT_TX<<1)|ENABLE:	//��һ��Rs485ͨ�ſ�|����|��
		UART0->CTRL |=0x02;		//���η����ж�
		UART0->STA = 0x02;		//�巢���жϱ�־

	  	UART0->TXD=*(ComPack[SCOM_PORT_RS485A].pTx++);
	  	ComPack[SCOM_PORT_RS485A].TxLen--;
		break;
		
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_RX<<1)|DISABLE:	//�ز�ͨ�ſ�|����|�ر�
		UART3->CTRL &=0xfff3; 	//���ν����ж�,���ν��մ����ж�
		UART3->STA = 0x3d;		//������жϱ�־�����մ����־
		break;
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_RX<<1)|ENABLE:	//�ز�ͨ�ſ�|����|��
		UART3->CTRL |=0x0c; 	//��������ж�,������մ����ж�
		UART3->STA = 0x3d;		//������жϱ�־�����մ����־
		break;
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_TX<<1)|DISABLE:	//�ز�ͨ�ſ�|����|�ر�
		UART3->CTRL &=0xfffd;		//���η����ж�
		UART3->STA = 0x02;		//�巢���жϱ�־
		break;
	case (SCOM_PORT_ZBCOM<<2)|(USART_IT_TX<<1)|ENABLE:	//�ز�ͨ�ſ�|����|��
		UART3->CTRL |=0x02;		//���η����ж�
		UART3->STA = 0x02;		//�巢���жϱ�־

	  	UART3->TXD=*(ComPack[SCOM_PORT_ZBCOM].pTx++);
	  	ComPack[SCOM_PORT_ZBCOM].TxLen--;
		break;
	default: return;
	}
	return;
}


