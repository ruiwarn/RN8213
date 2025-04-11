
#define  _DL645CLOCK_GLOBALS

#include <Drive_Lib.h>
extern	 void SystemDelay(u16 t);

//*********主时钟切换到外部高速晶振模式***********
void fnDl645MainClock_Init( void )
{
   	SYSCTL->SYS_PS = 0x82;
   	SysModeChg(2,1);
    SYSCTL->SYS_PD = 0x0018;
    SYSCTL->MOD0_EN = 0x3b54;
    SYSCTL->MOD1_EN = 0x0fe0;
    SYSCTL->INTC_EN = 0x01ff;
    SYSCTL->KBI_EN = 0x0000;
    SYSCTL->SYS_RST = 0x0000;   
	SystemDelay(100);
	SYSCTL->SYS_PS = 0x00;
  	return;
}

//*******主时钟切换到32768HZ低功耗模式********
void fnDl645SubClock_Init( void )
{
	SysModeChg(0,0);
}

void fnDl645RCClock_Init( void )
{
	SysModeChg(1,0);
}

//*******获取当前系统时钟频率********
u32 fnDl645MainClock_Get( void )
{
	u32  freq = 1618496;
	if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) == 0x3)
		freq = 32768;
    else if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) == 0x2)
		freq = 1843200;
    else if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) == 0x1)
    	freq = 3686400;
    else if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) == 0x0)
    	freq = 7372800;
    else if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) == 0x4)
    	freq = 14745600;
    else if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) == 0x5)
    	freq = 29491200;
  	if(SYSCTL->OSC_CTRL1 & 0x00000020)	freq = 1843200-224704;
  	return freq;
}
