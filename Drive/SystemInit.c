#define  _SystemInit_GLOBALS
#include <Drive_Lib.h>
#include <Scom.h> 

void fnRTCDrive_Delay(void)
{
	u8 i;
	for(i=200;i>0;i--)
	{
		__NOP();
		__NOP();
		__NOP();
		__NOP();
	}
}

void SystemDelay(u16 t)
{
	u16 i;
	while(t--)
	{
		for (i = 0;i < 400;i++)
		  ;
		WDT->EN = 0xbb;
	}
}

void fnWDT_Restart( void )
{
	WDT->EN = 0xbb;		/* restart watch dog timer */
}


void fnRCC_Configuration(void)	
{
    fnDl645MainClock_Init();
}

void fnGPIO_Configuration(void)
{
	#if(METERTYPE)
	GPIO->PA |=0x00000007;
	GPIO->PMA = 0xfb75fff8;		     // p0.0 Rst(ZB)  p0.1 SET p0.2 evenout p0.3 LVD p0.4 JDQ_TEST 
	GPIO->PUA = 0xFb00Fce0;
	GPIO->PIMA = 0;				    // P1.0 TURN P1.1 UP_OVER
	GPIO->PCA0 = 0x26000020;	    // P2.0 RXD P2.1 TXD P2.2 IR_RXD P2.3 IR_TXD P2.4 STA P2.5 SWDIO P2.6 RXD3 P2.7 TXD3   
	GPIO->PCA1 = 0x00000020;	    // P3.2 SP(RTC) 
	GPIO->PIEA = 0xffeffce7;
	
	
	GPIO->PB |=0x00003000;
	GPIO->PB &=0xffffa70f;
	GPIO->PMB = 0x0fe0860A;		    // p4.0 ES_CLK P4.1 SAMIO P4.2 ICIO_OUT P4.3 ICIO_IN P4.4 SPK P4.5 BJ_led p4.6JDQ1A P4.7 JDQ1B 
	GPIO->PUB = 0x0fe00200;			// P5.0 CF P5.2 GH  P5.3 TZ_LED P5.4 SamVcc P5.5 SAMRST p5.6 YJBG P5.7 PRG
	GPIO->PCB = 0xF01F010F;	        // P6.0 - P6.4 SEG
		   					   		// P7.4- P7.7 SEG 
	GPIO->PIEB = 0xFFFF7fFF;
	
	GPIO->PMC = 0xffffff00;		    	// P8.0 - P8.7 SEG
	GPIO->PUC = 0xffffff00;				// P9.0 - P9.7
	GPIO->PCC = 0x000000FF;	        	// P10
		   					   			// P11
	GPIO->PIEC = 0xFFFFFFFF;	
	GPIO->PCE = 0x0000000f;
#else
	GPIO->PMA = 0xfb75ffff;		     // p0.0 Rst(ZB)  p0.1 SET p0.2 evenout p0.3 LVD p0.4 JDQ_TEST 
	GPIO->PUA = 0xfb00fce0;
	GPIO->PIMA = 0;				    // P1.0 TURN P1.1 UP_OVER
	GPIO->PCA0 = 0x26000020;	    // P2.0 RXD P2.1 TXD P2.2 IR_RXD P2.3 IR_TXD P2.4 STA P2.5 SWDIO P2.6 RXD3 P2.7 TXD3   
	GPIO->PCA1 = 0x00000020;	    // P3.2 SP(RTC) 
	GPIO->PIEA = 0xfffffce7;
	
	
	GPIO->PB |=0x00003000;
	GPIO->PB &=0xffffa71f;
	GPIO->PMB = 0x0fe0863e;		    // p4.0 ES_CLK P4.1 SAMIO P4.2 ICIO_OUT P4.3 ICIO_IN P4.4 SPK P4.5 BJ_led p4.6JDQ1A P4.7 JDQ1B 
	GPIO->PUB = 0x00000000;			// P5.0 CF P5.2 GH  P5.3 TZ_LED P5.4 SamVcc P5.5 SAMRST p5.6 YJBG P5.7 PRG
	GPIO->PCB = 0xF01F0103;	        // P6.0 - P6.4 SEG
		   					   		// P7.4- P7.7 SEG 
	GPIO->PIEB = 0xffffffff;
	
	GPIO->PMC = 0xffffff00;		    	// P8.0 - P8.7 SEG
	GPIO->PUC = 0xffff0000;				// P9.0 - P9.7
	GPIO->PCC = 0x000000FF;	        	// P10
		   					   			// P11
	GPIO->PIEC = 0xFFFFFFFF;	
	GPIO->PCE = 0x0000000f;
#endif

}

//---UART------------------------------------------------------
//函数功能：串口初始化
//入口参数：UsartPort串口类型,Cfg串口参数
//出口参数：无
//======================================
//负控协议 透明转发通讯控制字定义
//bit7~bit5:Baud(bps):0-7依次标示:300,600,1200,2400,4800,7200,9600,19200
//bit4:0/1:1/2停止位
//bit3:0/1:无/有校验
//bit2:0/1:偶/奇校验
//bit1,bit0:0-3:5-8位数
//======================================
const u32 BpsTable[] = {300 , 600 , 1200 , 2400 , 4800 , 7200 , 9600 , 19200};
void fnUsart_Init(u8 ComPort , u8 Cfg)  //所有的通讯口初始化
{	
	if(ComPort >= SCOM_PORT_MAX) return;
	
	switch(ComPort)
	{
  	case SCOM_PORT_IR:	//红外通信口
		fnUSART_IRCom_Init(Cfg);
		break;
  	case SCOM_PORT_RS485A:	//第一个Rs485通信口
  		fnUSART_RS485_Init(Cfg);
		break;
	case SCOM_PORT_ZBCOM:
		fnUSART_ZBCOM_Init(Cfg);
		break;
	default: return;
	}

	USART_ITConfig(ComPort , USART_IT_RX , ENABLE);         //接收中断使能
	return;
}
//---------------------------------------------------------
void INTP_Init( void )
{
	INTC->MODE = 0x04; 			// INTP1 fail SEL
	INTC->CTRL = 0x02;
	INTC->MASK = 0x02;
	INTC->STA = 0xff;
}

//定时器初始化 用做1MS中断
void fnINTTC0_Init( void )
{
    TC0->CCFG = 0;
    TC0->PS = 255;
    TC0->DN = 13;
    TC0->CTRL = 0x03;		// star TC0
    TC0->IE = 0x01;			// IRQ enable
}

void fnLVD_Init(void)
{
	MADC->LVD_CTRL = 0x00000180;			 // enable LVD
	MADC->LVD_STAT = 0x01ff;
}

void fnINTTCAll_Init( void )
{
	fnINTTC0_Init();
}

void fnINTRTC_Init( void )
{
	RTC->CTRL |= 0x00000100;		//RTC Write enable
	fnRTCDrive_Delay();
	RTC->CTRL = 0x00000159;
	fnRTCDrive_Delay();
	RTC->CNT1 = 0x00;
	fnRTCDrive_Delay();
	RTC->IE = 0x02;
	fnRTCDrive_Delay();
}

//函数功能：计算时戳(每个定时器节拍，SysStamp累加一次)
//入口参数：Ago需要比较的节拍
//出口参数：返回SysStamp与Ago间的节拍数
u16 fnStamp_Through(u16 Ago)
{
	extern u16	SysStamp;
	if (SysStamp >= Ago)
		{return (SysStamp - Ago);}
	else
		{return ( (0xffff - Ago) + SysStamp);}
}

void SystemInit(void)
{

}
//---------------------------------------------------------
//函数功能：硬件系统初始化
//入口参数：无
//出口参数：无
void fnTarget_Init(void)
{
	fnRCC_Configuration();
	fnGPIO_Configuration();
	fnLVD_Init();
	fnINTTCAll_Init();
	fnINTRTC_Init(); 
	INTP_Init();
	fnISO7816_Init();
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART3_IRQn);
	NVIC_EnableIRQ(TC0_IRQn);
	NVIC_DisableIRQ(KBI_IRQn);
	NVIC_EnableIRQ(CMP_IRQn);
	NVIC_DisableIRQ(RTC_IRQn);
	NVIC_EnableIRQ(VCH_IRQn);
	#if(METERTYPE == 1)
	NVIC_EnableIRQ(ISO78161_IRQn);   	// 电源稳定后在进行卡插入的检测
	#endif
}
