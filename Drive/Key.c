#define  _KEY_GLOBALS
#include <Drive_Lib.h>


void fnKey_Init(void)
{	
	memset(&Key,0,sizeof(sKey_TypeDef) ); 
}

//�������ܣ�����ɨ��
//��ڲ�������
//���ڲ�������(�������º��ͷŷֱ����Key.StampPress��Key.StampRelease�����а�����������ӦλΪ1)
void fnKey_Scan(void)
{
	Key.KeyProg_Scan  =(Key.KeyProg_Scan << 1)+ PinRead_KeyPrg();
	Key.KeyTurn_Scan  =(Key.KeyTurn_Scan << 1)+ PinRead_KeyTurn();      
	Key.KeyShell_Scan  =(Key.KeyShell_Scan << 1)+ PinRead_KeyShell();
	if (Key.KeyProg_Scan ==0x00 )    //��̰�ť����
	{
		if(Key.Release&KEY_VAL_PRG)
		{
   			Key.Press |= 0X01; 
   			Key.Release &= 0XFE; 			   
		}
	}
	if (Key.KeyProg_Scan ==0xFF )   //��̰�ť��̬�ͷ�̬ 
	{
		Key.Release |= 0X01; 
	}
	if (Key.KeyTurn_Scan ==0x00 )    //���԰�ť����
	{
 		Key.KeyTurn_Time++;
 		if(Key.Release &KEY_VAL_TURN)
 		{
    		Key.Press |= 0X02; 
    		Key.Release &= 0XFD; 			   
 		}		   
	}
	if (Key.KeyTurn_Scan ==0xFF )    //���԰�ť��̬�ͷ�̬
	{
		Key.KeyTurn_Time=0;
		Key.Release |= 0X02;				    
	}
	if (Key.KeyTurn_Time >=3000 )  
	{
		Key.KeyTurn_Time=0;                       
		Key.Press |= 0X08;                        
	}
	if (Key.KeyShell_Scan ==0x00 )    //���ϱ�ǰ�ťɨ��
	{		
		Key.Press |= 0X04;              //״̬����		        
		if(Key.Release &KEY_VAL_SHELL)
		{ 
  			Key.Press |= 0X10;              //��¼���Ǽ�¼��

  			Key.Release &= 0XFB; 	
 		}	  
	}
	if (Key.KeyShell_Scan ==0xff )     //��ǳ�̬���ϸ�״̬
	{
		Key.Release |= 0X04;           //״̬���� 	
		if(Key.Press &KEY_VAL_SHELL)			  
 		{
  			Key.Release |= 0X10;	        //��¼�ϸǼ�¼��        		      

  			Key.Press &= 0XFB;  
 		}
	}        	
}


























