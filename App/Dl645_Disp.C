// 07规约国网单相表

#define  DL645_Disp_GLOBALS
#include <App.h>

/**************************
				变量定义
***************************/

/*****************常量定义******************/
void fnDl645Disp_Init(void)
{
	u8	i;
	fnLcd_Init();
	LCD->CTRL = 0x0;
    #if  !DL645SOFT_DEBUG
    for(i=0;i<200;i++)
	{
		if(!(LCD->STA & 0x40)) break;
		SystemDelay(1);
	}
   	#endif
    if(Dl645Bkgrd.PubData.Active==BKGRD_ACTIVE_POWDN) 
    {
    	SYSCTL->SYS_PS = 0x75;
		*(uint32_t *)(0x4003405c) |= 0xa5790020;
		SYSCTL->SYS_PS = 0x0;
    	LCD->CTRL = 0xbfb;
    }
    else LCD->CTRL = 0x3fb;
    LCD->CLKDIV = 0x1f;
    LCD->BLINK = 0x000001;
	fnDl645File_Read(Dl645FileId_GeneralPara, Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + 10*DL645_MAX_CYCDISP , &i , 1 );
	i = fnBcdToHex_u8(i);
	fnLcd_ClrScr();
	memset(&Dl645Session,0,sizeof(sDl645B_Session_TYPE)); //上电复位后，默认为自动循环显示 
    memset(Dl645Session.Item,0xff,5);
    Dl645Session.BkLightFlag=0x5a;			
    Dl645Session.BkLightStamp=255 - i;
    Dl645Session.Stamp=i;
  	Dl645Session.DisplyErrFlag=1;
  	memset(&Remote,0,sizeof(sIRRemote_TypeDef));
  	Dl645Session.AllOnDispTime =i;
  	return;
}

/*********************
		显示指定数据
**********************/
void fnDl645Disp_Data(u8 *Buf,u8 DataType,u8 Length)
{
  	u8 i=Length;
  	u8 temp;
  	u8 ZeroStatus=0x0;

	if((DataType<=0x6))
	{
		if(Buf[Length-1]&0x80)
		{									// 电量最高位为1既电量为负时
			Buf[Length-1]&=0x7f;
			fnLcd_SetSegment(SEG_FuHao,1);
		}
		for(i=0;i<8;i++) 
		{
			if(i%2==0x0)
				{temp=Buf[3-(i>>1)]>>4;}
			else
				{temp=Buf[3-(i>>1)]&0x0f;}
			if((Length==3)&&(i<2)) temp=0;
			if((Length==2)&&(i<4)) temp=0;
			if(temp==0x0 && ZeroStatus==0x0)
			{
				if((7-i)>DataType)
        		{fnLcd_SetDigit(i+1,0x10);continue;}
			}
			fnLcd_SetDigit(i+1,temp);
			ZeroStatus=0x1;		
		}
		fnLcd_SetSegment(SEG_QsegDp7,0);
		fnLcd_SetSegment(SEG_QsegDp6,0);
		fnLcd_SetSegment(SEG_QsegDp5,0);
		fnLcd_SetSegment(SEG_QsegDp4,0);
		fnLcd_SetSegment(SEG_QsegDp3,0);
		//fnLcd_SetSegment(SEG_QsegDp2,0);
		if(DataType==0x1){fnLcd_SetSegment(SEG_QsegDp7,1);}
		if(DataType==0x2){fnLcd_SetSegment(SEG_QsegDp6,1);}
		if(DataType==0x3){fnLcd_SetSegment(SEG_QsegDp5,1);}
		if(DataType==0x4){fnLcd_SetSegment(SEG_QsegDp4,1);}
		if(DataType==0x5){fnLcd_SetSegment(SEG_QsegDp3,1);} //5位小数 
		//if(DataType==0x6){fnLcd_SetSegment(SEG_QsegDp2,1);}  //6位小数 
	}
  	else if (DataType==0x7)
	{											// 显示日期
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[0]&0x0f);
      
		fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[1]&0x0f);
      
		fnLcd_SetDigit(3,(Buf[2]&0xf0)>>4);
		fnLcd_SetDigit(4,Buf[2]&0x0f);
      
		fnLcd_SetDigit(1,(Buf[3]&0xf0)>>4);
		fnLcd_SetDigit(2,Buf[3]&0x0f);
		//fnLcd_SetSegment(SEG_QsegDp2,1);
		fnLcd_SetSegment(SEG_QsegDp4,1);
		fnLcd_SetSegment(SEG_QsegDp6,1);
	}
  	else if(DataType==0x8)  //显示日时段表起始时间及其费率---01--20：10
	{
		//费率
		fnLcd_SetDigit(1,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(2,Buf[0]&0x0f);   
		
		//小时
		fnLcd_SetDigit(5,(Buf[2]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[2]&0x0f);
    
		//分钟
		fnLcd_SetDigit(7,(Buf[1]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[1]&0x0f);
    
		fnLcd_SetSegment(SEG_QsegDp6,1);
		fnLcd_SetSegment(SEG_QsegDp8,1);
		fnLcd_SetSegment(SEG_Qseg3g,1);
		fnLcd_SetSegment(SEG_Qseg4g,1);
	}
	else if(DataType==0x9)//显示16进制数
  	{
		for(i=0;i<Length;i++)
		{
			fnLcd_SetDigit(7-i*2,(*(Buf+i)&0xf0)>>4);
			fnLcd_SetDigit(7-i*2+1,*(Buf+i)&0x0f);
		}
  	}
    else if(DataType==0xa)  //显示日、时、分
	{
//		BinToBcd(*Buf,(u8 *)&temp,1);//分钟
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[0]&0x0f);

//		BinToBcd(*(Buf+1),(u8 *)&temp,1);//小时
		fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[1]&0x0f);


//		BinToBcd(*(Buf+1),(u8 *)&temp,1);//日
		fnLcd_SetDigit(3,(Buf[2]&0xf0)>>4);
		fnLcd_SetDigit(4,Buf[2]&0x0f);
		fnLcd_SetSegment(SEG_QsegDp6,1);
		fnLcd_SetSegment(SEG_QsegDp8,1);
		fnLcd_SetSegment(SEG_QsegDp4,1);
	}
	else if (DataType==0xb)//显示年、月、日
	{
//		BinToBcd(*Buf,(u8 *)&temp,1);//日
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[0]&0x0f);
      
//		BinToBcd(*(Buf+1),(u8 *)&temp,1);//月
		fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[1]&0x0f);
      
//		BinToBcd(*(Buf+2),(u8 *)&temp,1);//年
		fnLcd_SetDigit(3,(Buf[2]&0xf0)>>4);
		fnLcd_SetDigit(4,Buf[2]&0x0f);

		fnLcd_SetSegment(SEG_QsegDp4,1);
		fnLcd_SetSegment(SEG_QsegDp6,1);
	}

	else if(DataType==0xc)                            // 月 日 时  分
	{
  		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
  		fnLcd_SetDigit(8,Buf[0]&0x0f);

 	 	fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
  		fnLcd_SetDigit(6,Buf[1]&0x0f);
   
  		fnLcd_SetDigit(3,(Buf[2]&0xf0)>>4);
  		fnLcd_SetDigit(4,Buf[2]&0x0f);
   
  		fnLcd_SetDigit(1,(Buf[3]&0xf0)>>4);
  		fnLcd_SetDigit(2,Buf[3]&0x0f);
  		//fnLcd_SetSegment(SEG_QsegDp2,1);
  		fnLcd_SetSegment(SEG_QsegDp6,1);
  		fnLcd_SetSegment(SEG_QsegDp4,1);
  		fnLcd_SetSegment(SEG_QsegDp8,1);
	}
	else if(DataType==0xd)//时、分、秒
    {
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[0]&0x0f);

	    fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
	    fnLcd_SetDigit(6,Buf[1]&0x0f);
	     
	    fnLcd_SetDigit(3,(Buf[2]&0xf0)>>4);
	    fnLcd_SetDigit(4,Buf[2]&0x0f);
	     
		fnLcd_SetSegment(SEG_QsegDp9,1);
		fnLcd_SetSegment(SEG_QsegDp4,1);
		fnLcd_SetSegment(SEG_QsegDp8,1);
		fnLcd_SetSegment(SEG_QsegDp6,1);
    }
	else if(DataType==0xe)//显示状态字
  	{
		fnLcd_SetDigit(8,Buf[0]&0x0f);
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[1]&0x0f);
		fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
  	}
	else if(DataType==0xf)//日--时，
    {
	    fnLcd_SetDigit(3,(Buf[1]&0xf0)>>4);
	    fnLcd_SetDigit(4,Buf[1]&0x0f);
	    fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
	    fnLcd_SetDigit(8,Buf[0]&0x0f);
	    
	    fnLcd_SetSegment(SEG_Qseg5g,1);  //显示：--
	    fnLcd_SetSegment(SEG_Qseg6g,1);  
    }
    else if(DataType==0x10)  //显示 时、分
	{
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[0]&0x0f);

		fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[1]&0x0f);
		fnLcd_SetSegment(SEG_QsegDp6,1);
		fnLcd_SetSegment(SEG_QsegDp8,1);
	}
    else if(DataType==0x11)  //显示日时区表起始时间及其时段表号---01-05.01
	{
//		BinToBcd(*(Buf+2),(u8 *)&temp,1);//费率
		fnLcd_SetDigit(1,(Buf[0]&0xf0)>>4);
		fnLcd_SetDigit(2,Buf[0]&0x0f);   
		
//		BinToBcd(*Buf,(u8 *)&temp,1);//小时
		fnLcd_SetDigit(5,(Buf[2]&0xf0)>>4);
		fnLcd_SetDigit(6,Buf[2]&0x0f);
    
//		BinToBcd(*(Buf+1),(u8 *)&temp,1);//分钟
		fnLcd_SetDigit(7,(Buf[1]&0xf0)>>4);
		fnLcd_SetDigit(8,Buf[1]&0x0f);
    
		fnLcd_SetSegment(SEG_QsegDp6,1);
		fnLcd_SetSegment(SEG_Qseg3g,1);
		fnLcd_SetSegment(SEG_Qseg4g,1);
	}
	else if(DataType==0x12)//模式字、特征字
  	{
		fnLcd_SetDigit(8,Buf[0]&0x0f);
		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
  	}
  	else if(DataType==0x13) // 显示 年 月 日 时
  	{
  		fnLcd_SetDigit(7,(Buf[0]&0xf0)>>4);
  		fnLcd_SetDigit(8,Buf[0]&0x0f);

 	 	fnLcd_SetDigit(5,(Buf[1]&0xf0)>>4);
  		fnLcd_SetDigit(6,Buf[1]&0x0f);
   
  		fnLcd_SetDigit(3,(Buf[2]&0xf0)>>4);
  		fnLcd_SetDigit(4,Buf[2]&0x0f);
   
  		fnLcd_SetDigit(1,(Buf[3]&0xf0)>>4);
  		fnLcd_SetDigit(2,Buf[3]&0x0f);
  	}
	else  //非法显示类型，显示--------
	{
		for (i=1;i<9;i++) fnLcd_SetDigit(i,0x11);
	}
	return;
}


/**************************
               电表状态刷新
***************************/
void fnDl645Disp_RefreshStatus(void)
{
   	fnLcd_SetSegment(SEG_Phone,0);                               
  	if (Dl645Bkgrd.PubData.CommStatus&F_Comm1Event)       //通讯状态指示----电话符号
	{
		Dl645Bkgrd.PubData.CommStatus&=~F_Comm1Event;                              
    	fnLcd_SetSegment(SEG_Phone,1);
	}

  	if (Dl645Front.PubData.PDirect ==  INVERSION)
    {fnLcd_SetSegment(SEG_FanXiang,1);}
  	else
	{fnLcd_SetSegment(SEG_FanXiang,0);}
 	return;  
}


/**************************
  根据显示代码，显示数据，刷新LCD               
***************************/
void fnDl645Disp_RefreshDispData(void)
{
	fnLcd_ClrScr();
                   //显示---数据对象及提示信息 
  	switch(Dl645Session.Item[3])
  	{
    	case 2:
    		fnDl645Disp_Variable();//显示变量
    		break;
    	case 4://显示参变量
      		fnDl645Disp_GeneralPara();
      		break;
    	case 0xff://显示
    		fnLcd_AllOn();
      		return;
    	default:
      		break;
  	}
    //显示---运行状态刷新
  	if(Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWDN) fnDl645Disp_RefreshStatus();
  	return;
}

/******************************************
  根据显示代码，显示当前变量数据，刷新LCD               
*******************************************/
//#pragma location="BANK2"
void fnDl645Disp_Variable(void)
{
	u8	DataType,Buffer[8];
  	u8 Length=0,i;
  	u16 len0;
  
	for(i=0;i<8;i++)
	{
		Buffer[i]=0;
	}
	fnDl645_GetVar(Dl645Session.Item[2] , Dl645Session.Item[1] , Dl645Session.Item[0] , Buffer , &len0);
	Length=(u8)len0;
  	if(Length>8)
    Length=0;
	switch(Dl645Session.Item[2])
	{
		case	1:
			fnLcd_SetSegment(SEG_V1,1);
			DataType=1;
			break;
		case	2:

			fnLcd_SetSegment(SEG_A1,1);
			fnLcd_SetSegment(SEG_L,1);
			DataType=3;
			break;
		case	3:
			fnLcd_SetSegment(SEG_KW,1);
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 3 , &DataType , 1);		//功率小数位数		
			if(DataType==0) 
	        {
	        	Buffer[0]=(0x7f & Buffer[2]);
	        	Buffer[1]=0x00;
	        	Buffer[2]=(0x80 & Buffer[2]);
	        }
	        else if(DataType==1)
	        {
	        	Buffer[0]=((u8)(0x7f & Buffer[2])<<4) | (Buffer[1]>>4);
	        	Buffer[1]=((0x7f & Buffer[2])>>4);
	        	Buffer[2]=(0x80 & Buffer[2]);
	        }
	        else if(DataType==2)
	        {
	        	Buffer[0]=Buffer[1];
	        	Buffer[1]=(0x7f & Buffer[2]);
	        	Buffer[2]=(0x80 & Buffer[2]);
	        }
	        else if(DataType==3)
	        {
	        	Buffer[0]=((u8)Buffer[1]<<4) | (Buffer[0]>>4);
	        	Buffer[1]=((u8)(0x7f & Buffer[2])<<4) | (Buffer[1]>>4);
	        	Buffer[2]=(0x80 & Buffer[2]) | ((0x7f & Buffer[2])>>4);
	        }
	        else
	        {
	        	DataType=4;
	        }
			break;
		case	6:
			if(Dl645Session.Item[1]==0)
			{
				fnLcd_SetSegment(SEG_Qzong,1);
			}
			fnLcd_SetSegment(SEG_COS,1);
			DataType=3;
			break;

		case	7:
			fnLcd_SetSegment(SEG_COS,1);         //A相相角
			DataType=1;
			break;	
		case	0x80:
			if(Dl645Session.Item[0]==1)
			{								// 零线电流
				fnLcd_SetSegment(SEG_A1,1);
			
				fnLcd_SetSegment(SEG_N,1);
				DataType=3;
			}
			else if(Dl645Session.Item[0]==2)
			{								// 电网频率
				DataType=2;
			}

			else if(Dl645Session.Item[0]==7)
			{								// 表内温度
				DataType=1;
			}
			
			else if(Dl645Session.Item[0]==8)
			{
				fnLcd_SetSegment(SEG_V1,1);
				DataType=2;
			}

			else if(Dl645Session.Item[0]==0X0B)
			{								// 当前阶梯电价
    			fnLcd_SetSegment(SEG_dangqian,1);
				fnLcd_SetSegment(SEG_Yuan,1);
				fnLcd_SetSegment(SEG_JieTi,1);
				fnLcd_SetSegment(SEG_Qdian,1);
				fnLcd_SetSegment(SEG_Jia,1);			
				DataType=4;
			}
			
			else
				DataType=9;
			break;
	}
	fnDl645Disp_Data(Buffer,DataType,Length);
   	return;
}
/****************************************
               通用参数显示处理子程序   
*****************************************/
//设备档案
//常规参数
void fnDl645Disp_GeneralPara(void)
{
	u8	DataType=0xff;
  	u8	*Buffer,Length=0;
  	u8	*Buf;
	Buf = malloc(50);
	Buffer = Buf;
	if((Dl645Session.Item[1]==1)&&(Dl645Session.Item[2]==0)&&(Dl645Session.Item[0]==1))		//日期
	{
		Buffer[0] = SysClock.Day; Buffer[1] = SysClock.Month; Buffer[2] = SysClock.Year; Buffer[3] = 0x20;
		Length=4;
		DataType=7;
	}
	else if((Dl645Session.Item[1]==1)&&(Dl645Session.Item[2]==0)&&(Dl645Session.Item[0]==2))//显示时间
	{
       	Buffer[0] = SysClock.Second; Buffer[1] = SysClock.Minute; Buffer[2] = SysClock.Hour;
		Length=3;
		DataType=0xd;
	    fnLcd_SetSegment(SEG_dangqian,1);
		fnLcd_SetSegment(SEG_TIME,1);
		fnLcd_SetSegment(SEG_JIAN,1);
	}
	else if((Dl645Session.Item[1]==1)&&(Dl645Session.Item[2]==0)&&(Dl645Session.Item[0]==0x0c))	// 显示日期时间
	{
		if(Dl645Session.Item[4]==1)
		{
			Buffer[0] = SysClock.Second; Buffer[1] = SysClock.Minute; Buffer[2] = SysClock.Hour;
			Length=3;
			DataType=0xd;
	    	fnLcd_SetSegment(SEG_dangqian,1);
			fnLcd_SetSegment(SEG_TIME,1);
			fnLcd_SetSegment(SEG_JIAN,1);
		}
		else if(Dl645Session.Item[4]==0)
		{
			Buffer[0] = SysClock.Day; Buffer[1] = SysClock.Month; Buffer[2] = SysClock.Year; Buffer[3] = 0x20;
			Length=4;
			DataType=7;
		}
	}
	fnDl645Disp_Data(Buffer,DataType,Length);
	free(Buf);
}

/**************************
            液晶显示控制子程序
***************************/
void fnDl645Disp_SessionExec(void)
{
	u8 temp0,temp1;
	
	if((Dl645Bkgrd.PubData.Active==BKGRD_ACTIVE_POWDN)||(Dl645Inactive.PubData.InactiveStamp)) return;
	if(Dl645Session.BkLightFlag==0x5a) PinWrite_LCDLight(1);;	
	if(Key.Press&KEY_VAL_TURN)    //只要有用户操作按键按下或遥控器操作，则转为按键模式
	{
		if(Dl645Bkgrd.PubData.Active==BKGRD_ACTIVE_POWDN)//修眠唤醒时，若有按键再次按下，则须将唤醒时间再延长30秒。 			 
			Dl645Inactive.PubData.ActiveStamp= SysStamp ? SysStamp:(SysStamp-1);	
		if((Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWDN)&&(Key.Press&KEY_VAL_TURN))
		{
			Dl645Session.BkLightFlag=0x5a;
			Dl645Session.BkLightStamp=195;			//启动背光点亮计时
		}
		Dl645Session.Mode=DISPLAYKEY;
		Dl645Session.Stamp=0;
		Dl645Session.DisplyErrFlag=1;
		goto Start_Display;
	}
	if(!Dl645Bkgrd.PubData.fChange.FlagBit.Second) return;
	if(Dl645Session.BkLightStamp)
	{
		if(++Dl645Session.BkLightStamp>254)
		{
			Dl645Session.BkLightFlag=0x0;
			PinWrite_LCDLight(0);		
			Dl645Session.BkLightStamp=0;
		}
	}
	else 
	{
		Dl645Session.BkLightFlag=0x0;
		PinWrite_LCDLight(0);		
		Dl645Session.BkLightStamp=0;
	}
Start_Display:
	if (Dl645Session.Mode==DISPLAYAUTO) //自动循环显示
	{
		if(Dl645Session.AllOnDispTime) Dl645Session.AllOnDispTime--;
		fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 1 , (u8 *)&temp0,1);  //读取循显时间
		temp0 = fnBcdToHex_u8(temp0);
		if(temp0==0) temp0 =5;		
		if((--Dl645Session.Stamp==0) && (temp0))
		{
			Dl645Session.DisplyErrFlag=1;
			Dl645Session.Stamp=temp0;
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + Dl645Session.AutoSerialNo*5 , Dl645Session.Item , 5);//循显项目
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara  , (u8 *)&temp1,1); //读取自动循显屏数
			temp1 = fnBcdToHex_u8(temp1);
			if((++Dl645Session.AutoSerialNo>temp1)||(Dl645Session.Item[4]==InvalidItem))
			{
				Dl645Session.AutoSerialNo=0x1;
				fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5  , Dl645Session.Item , 5);//循显项目
			}
		}
	}
	else //按键和遥控显示 
	{
		if(Dl645Session.AllOnDispTime) Dl645Session.AllOnDispTime = 0;	// 按键后上电全显结束
		temp0=30;
		if (++Dl645Session.Stamp>=temp0) 				// 按键默认显示30S，如无操作转为自动轮显
		{
			Dl645Session.Stamp=1;
			Dl645Session.Mode=DISPLAYAUTO;
			Dl645Session.DigitPosition=2;
			Dl645Session.KeySeriaNo=0;
		}
		else
		{
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 4 , (u8 *)&temp1,1);//读取按键显示屏数
			
			temp1 = fnBcdToHex_u8(temp1);
			if (Key.Press&KEY_VAL_TURN)
			{
				Dl645Session.Mode=DISPLAYKEY;
				if(++Dl645Session.KeySeriaNo>temp1)
       			{
					Dl645Session.KeySeriaNo=1;
				}
				fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5+5*DL645_MAX_CYCDISP +(Dl645Session.KeySeriaNo-1)*5 , Dl645Session.Item,5);////按键显示项目
				if(Dl645Session.Item[4]==InvalidItem)
				{
				fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + 5*DL645_MAX_CYCDISP  , Dl645Session.Item,5);////按键显示项目
	      			Dl645Session.KeySeriaNo=0;
				}
			}
		}
	}
	
	if(Key.Press & KEY_VAL_TURN)
	{
		Key.Press &= ~KEY_VAL_TURN;
	}

	if(Dl645Session.DisplyErrFlag)
	{
    	fnDl645Disp_RefreshDispData();
	}
	else
	{
		fnDl645Disp_RefreshStatus();			 
	}
	fnLcd_RefreshLcdBuf();
	return;
}
