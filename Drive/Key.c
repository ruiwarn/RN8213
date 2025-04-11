#define  _KEY_GLOBALS
#include <Drive_Lib.h>


void fnKey_Init(void)
{	
	memset(&Key,0,sizeof(sKey_TypeDef) ); 
}

//函数功能：按键扫描
//入口参数：无
//出口参数：无(按键按下和释放分别放于Key.StampPress和Key.StampRelease，如有按键动作，对应位为1)
void fnKey_Scan(void)
{
	Key.KeyProg_Scan  =(Key.KeyProg_Scan << 1)+ PinRead_KeyPrg();
	Key.KeyTurn_Scan  =(Key.KeyTurn_Scan << 1)+ PinRead_KeyTurn();      
	Key.KeyShell_Scan  =(Key.KeyShell_Scan << 1)+ PinRead_KeyShell();
	if (Key.KeyProg_Scan ==0x00 )    //编程按钮按下
	{
		if(Key.Release&KEY_VAL_PRG)
		{
   			Key.Press |= 0X01; 
   			Key.Release &= 0XFE; 			   
		}
	}
	if (Key.KeyProg_Scan ==0xFF )   //编程按钮常态释放态 
	{
		Key.Release |= 0X01; 
	}
	if (Key.KeyTurn_Scan ==0x00 )    //键显按钮按下
	{
 		Key.KeyTurn_Time++;
 		if(Key.Release &KEY_VAL_TURN)
 		{
    		Key.Press |= 0X02; 
    		Key.Release &= 0XFD; 			   
 		}		   
	}
	if (Key.KeyTurn_Scan ==0xFF )    //键显按钮常态释放态
	{
		Key.KeyTurn_Time=0;
		Key.Release |= 0X02;				    
	}
	if (Key.KeyTurn_Time >=3000 )  
	{
		Key.KeyTurn_Time=0;                       
		Key.Press |= 0X08;                        
	}
	if (Key.KeyShell_Scan ==0x00 )    //开上表盖按钮扫描
	{		
		Key.Press |= 0X04;              //状态跟随		        
		if(Key.Release &KEY_VAL_SHELL)
		{ 
  			Key.Press |= 0X10;              //记录开盖记录用

  			Key.Release &= 0XFB; 	
 		}	  
	}
	if (Key.KeyShell_Scan ==0xff )     //表盖常态合上盖状态
	{
		Key.Release |= 0X04;           //状态跟随 	
		if(Key.Press &KEY_VAL_SHELL)			  
 		{
  			Key.Release |= 0X10;	        //记录合盖记录用        		      

  			Key.Press &= 0XFB;  
 		}
	}        	
}


























