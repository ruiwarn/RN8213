#define  _LcdDrive_GLOBALS
#include <Drive_Lib.h>

/*************************液晶低层显示部分*********************************/
void  fnLcd_Init(void)
{
	LCD->CTRL = 0x0;
	#if(LcdRefFlag == 1)
	SYSCTL->SYS_PS = 0x75;
	*(uint32_t *)(0x4003405c) |= 0xa5790000;
	*(uint32_t *)(0x4003405c) &= 0xffffffdf;
	SYSCTL->SYS_PS = 0x0;
	#endif
}

/*全屏*/
void fnLcd_AllOn(void)
{
   	u8 i;
	for(i=0;i<24;i++)
    {LCDMEM[i]=0xff;LCD->BUF[i] = 0xff;}
}
/*清屏*/
void fnLcd_ClrScr(void)
{
  	u8 i;
  	for(i=0;i<24;i++) LCDMEM[i]=0x00;
  	return;
}
/******************************************
                    八段显示0~9，A~F字符      
1、点亮或灭掉指定段，B=0时为熄灭,B=1时为点亮
2、nSegment为各段的定义位置，详见包含文件Basic.h
*******************************************/
void fnLcd_SetSegment(u8 nSegment,u8 B)
{
  	
  	u8 dot,i;
	if(nSegment>=192)  return;  // 8com
  	dot=0x01<<(nSegment%8);
  	i = nSegment/8;
  	if (B)
  	{
  		LCDMEM[i] |=dot;
  	}
  	else
  	{
  		LCDMEM[i] &=(~dot);
  	}
	return;
}


const unsigned char DigitNum1[19]={
//	0	 1   2    3    4    5    6    7    8    9
0xf5,0x05,0xd3,0x97,0x27,0xb6,0xf6,0x15,0xf7,0xb7,
// A	b	 C   d    E    F          -   r	
0x77,0xe6,0xc2,0xc7,0xf2,0x72,0x00,0x02,0x42
};

const unsigned char MemAddress[9]={13,21,19,17,15,13,4,2,0};

void fnLcd_SetDigit(u8 nDigit,u8 B)
{
	nDigit--;
	if (nDigit>9)return;  //数值范围1---9
	if(nDigit == 0x08)
	{
		nDigit = 0x00;
	}
	else   nDigit++;
	/*8com液晶程序*/
	if(nDigit == 0)
	{
		LCDMEM[MemAddress[nDigit] ] = ((DigitNum1[B] & 0x0f)|(LCDMEM[MemAddress[nDigit]] & 0x08)) + (LCDMEM[MemAddress[nDigit]] & 0xf0);
		LCDMEM[MemAddress[nDigit] + 1 ] = ((DigitNum1[B]>>4) & 0x0f) + (LCDMEM[MemAddress[nDigit] + 1] & 0xf0);
	}
	else if(nDigit == 6)
	{
		LCDMEM[MemAddress[nDigit] ] = (((DigitNum1[B]<<4) & 0xf0)|(LCDMEM[MemAddress[nDigit]] & 0x80)) + (LCDMEM[MemAddress[nDigit]] & 0x0f);
		LCDMEM[MemAddress[nDigit] + 8 ] = (DigitNum1[B] & 0xf0) + (LCDMEM[MemAddress[nDigit] + 8] & 0x0f);
	}
	else
	{
		LCDMEM[MemAddress[nDigit] ] = (((DigitNum1[B]<<4) & 0xf0)|(LCDMEM[MemAddress[nDigit]] & 0x80)) + (LCDMEM[MemAddress[nDigit]] & 0x0f);
		LCDMEM[MemAddress[nDigit] + 1 ] = (DigitNum1[B] & 0xf0) + (LCDMEM[MemAddress[nDigit] + 1] & 0x0f);
	}
	return;
}

void fnLcd_RefreshLcdBuf(void)
{
	u8 i;
  	for(i=0;i<24;i++)  LCD->BUF[i] = LCDMEM[i];
  	return;
}
//---------------------------------------------------
















