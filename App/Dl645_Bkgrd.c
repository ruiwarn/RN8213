
#define  _DL645_BKGRD_GLOBALS
#include <App.h>


/*****************************************休眠处理子程序**********************************************/
void SystemDelayUs(u8 i)
{
	if(((SYSCTL->OSC_CTRL1 >> 8) & 0x3) != 0x3)
	{
		for(;i>0;i--) fnDelay_1us();
	}
}


u8 fnDl645Bkgrd_LVDCheck(void)
{
	u8	temp,i,j;
	u8	PowerFlag = 5;
	for(i=0;i<5;i++)
	{
		temp = 50;
		for(j=0;j<50;j++)
		{
			if(!(MADC->LVD_STAT & 0x04)) temp++;
			else temp--;
			SystemDelayUs(10);
			fnWDT_Restart();
		}
		if(temp>90) PowerFlag++;
		if(temp<10) PowerFlag--;
	}
	return(PowerFlag);
}

void fnDl645Bkgrd_IOSetSleep(void)
{
	Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWDN;
   	
	GPIO->PMA = 0xffffffff;		     // p0.0 Rst(ZB)  p0.1 SET p0.2 evenout p0.3 LVD p0.4 JDQ_TEST 
	GPIO->PUA = 0x00000000;
	GPIO->PIMA = 0;				    // P1.0 TURN P1.1 UP_OVER
	GPIO->PCA0 = 0x00000a20;	    // P2.0 RXD P2.1 TXD P2.2 IR_RXD P2.3 IR_TXD P2.4 STA P2.5 SWDIO P2.6 RXD3 P2.7 TXD3   
	GPIO->PCA1 = 0x00000000;	    // P3.2 SP(RTC) 
	GPIO->PIEA = 0xfffffcff; 
	

	GPIO->PMB = 0xffffffff;		    // p4.0 ES_CLK P4.1 SAMIO P4.2 ICIO_OUT P4.3 ICIO_IN P4.4 SPK P4.5 BJ_led p4.6JDQ1A P4.7 JDQ1B 
	GPIO->PUB = 0x0;				// P5.0 CF P5.2 GH  P5.3 TZ_LED P5.4 SamVcc P5.5 SAMRST p5.6 YJBG P5.7 PRG
	GPIO->PCB = 0x00000000;	        // P6.0 - P6.4 SEG
		   					   		// P7.4- P7.7 SEG 
	GPIO->PIEB = 0xffffffff;
	
	GPIO->PMC = 0xffffffff;		    	// P8.0 - P8.7 SEG
	GPIO->PUC = 0x00000000;				// P9.0 - P9.7
	GPIO->PCC = 0x00000000;	        	// P10
		   					   			// P11
	GPIO->PIEC = 0xFFFFFFFF;		
		
	GPIO->PCE = 0x00000000;
	
	SYSCTL->SYS_PS = 0x82;
	
    eepromStandby();
	SYSCTL->SYS_PD = 0x00df;
    SYSCTL->MOD0_EN = 0x0000;
    SYSCTL->MOD1_EN = 0x0c20;
    SYSCTL->INTC_EN = 0x0000;
    SYSCTL->SYS_PS = 0x00;
    NVIC_DisableIRQ(UART0_IRQn);
	NVIC_DisableIRQ(UART1_IRQn);
	NVIC_DisableIRQ(UART3_IRQn);
	NVIC_DisableIRQ(TC0_IRQn);
	NVIC_EnableIRQ(KBI_IRQn);
	NVIC_DisableIRQ(ISO78161_IRQn);
	RTC->IF = 0xff;
	NVIC_EnableIRQ(RTC_IRQn);
}

void fnDl645Bkgrd_SleepSet(void)
{
	fnDl645Bkgrd_IOSetSleep();
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->KBI_EN = 0x0103;
    KBI ->CTRL = 0x03;
	KBI ->SEL = 0x03;
    KBI ->MASK = 0x03;
}

void fnDl645Bkgrd_Sleep(void)
{
    //if(Dl645Inactive.PubData.ActiveState==End_State)
    {
		if(!(MADC->LVD_STAT & 0x04))
		{
			Dl645Inactive.PubData.InactiveStamp= 0;
			__WFI();
		}
    }
}

void fnBeforeSleep(void)
{
    memset(&Dl645Front , 0 , sizeof(sDl645Front_TypeDef) ); 
	memset(&Dl645FrontTmp , 0 , sizeof(sDl645FrontTmp_TypeDef) );	//初始化RAM
}


void fnDl645Bkgrd_Second_Exec(void)
{
	
}

void fnDl645Bkgrd_Minute_Exec(void)
{
	
}

void fnDl645Bkgrd_Hour_Exec(void)
{
	
}

void fnDl645Bkgrd_Day_Exec(void)
{
	
}

void fnDl645Bkgrd_Month_Exec(void)
{
}

void fnDl645Bkgrd_Init(void) 
{	
	
	memset(&Dl645Bkgrd , 0 , sizeof(sDl645Bkgrd_TypeDef) );
	//fnDl645File_Read(Dl645FileId_InterTmp , Dl645FileItemInfoOffAddr_InterTmp_Misc +  sizeof(sDF01) , &PowDnTime , sizeof(sDF01) );
	Dl645Bkgrd.PriData.Second = SysClock.Second;
	Dl645Bkgrd.PriData.Minute = SysClock.Minute;
	Dl645Bkgrd.PriData.Hour = SysClock.Hour;
	Dl645Bkgrd.PriData.Day = SysClock.Day;
	Dl645Bkgrd.PriData.Month = SysClock.Month;
	
	Dl645Bkgrd.PubData.Second = SysClock.Second;//
}


//函数功能：根据时钟产生各种标志，完成时段、费率切换，完成事件状态判断
//入口参数：无
//出口参数：无
//Dl645Bkgrd.PubData.Second此变量在主程序或中断累加，Dl645Bkgrd.PriData为用过的时间，用来做比较
void fnDl645Bkgrd_Exec(void)
{
    fnSysClock_Read(&SysClock);
	
	if(SysClock.Second != Dl645Bkgrd.PriData.Second)
	{
	  fnDl645Bkgrd_Second_Exec();
	  Dl645Bkgrd.PubData.fChange.FlagBit.Second = 1;
	  Dl645Bkgrd.PriData.Second = SysClock.Second;
	}
	
	
	if((Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWON)||(Dl645Inactive.PubData.InactiveStamp)) return;
			
	
	
	if(SysClock.Minute != Dl645Bkgrd.PriData.Minute)
	{
		fnDl645Bkgrd_Minute_Exec();
		Dl645Bkgrd.PriData.Minute = SysClock.Minute;
		Dl645Bkgrd.PubData.fChange.FlagBit.Minute = 1;
	}	
	
	if(SysClock.Hour != Dl645Bkgrd.PriData.Hour)
	{
		fnDl645Bkgrd_Hour_Exec();
		Dl645Bkgrd.PriData.Hour = SysClock.Hour;
		Dl645Bkgrd.PubData.fChange.FlagBit.Hour = 1;
	}
	

	if(SysClock.Day != Dl645Bkgrd.PriData.Day)
	{
		fnDl645Bkgrd_Day_Exec();
		Dl645Bkgrd.PriData.Day = SysClock.Day;
		Dl645Bkgrd.PubData.fChange.FlagBit.Day = 1;
	}
	
	if(SysClock.Month !=Dl645Bkgrd.PriData.Month)
	{
		fnDl645Bkgrd_Month_Exec();
		Dl645Bkgrd.PriData.Month = SysClock.Month;
		Dl645Bkgrd.PubData.fChange.FlagBit.Month = 1;
	}
}

void fnDl645Bkgrd_Inactive(void)
{
	u8	i;
	u8	PowerFlag;
	if(Dl645Bkgrd.PubData.Active!=BKGRD_ACTIVE_POWDN)
    {
    	if(Dl645Bkgrd.PubData.Active==BKGRD_ACTIVE_POWUP)   //主电源恢复，系统上电恢复，延时10秒后，程序正常运行
    	{
    		if(fnStamp_Through(Dl645Bkgrd.PubData.PownUpingDelay)>500) 
    		{
    			Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWON;
    			
    		}

    		if(!(MADC->LVD_STAT & 0x04))		//停电后检测到低电压立即进入低功耗
    		{
				if(fnDl645Bkgrd_LVDCheck() > 8)
				{
					Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWDN;
					for(i=0;i<24;i++) {LCD->BUF[i] = 0x0;}
					Dl645Inactive.PubData.ActiveState = End_State;
					LCD->CTRL = 0x00;
					for(i=0;i<60;i++)
					{
						if(!(LCD->STA & 0x40)) break;
						fnRTCDrive_Delay();
					}
					fnDl645Bkgrd_SleepSet();
					fnDl645SubClock_Init();
				}
    		}
    	}
    	if(Dl645Inactive.PubData.InactiveStamp)    //跟踪交流电是否断电
    	{
    		PowerFlag = fnDl645Bkgrd_LVDCheck();
    		if(PowerFlag<2) Dl645Inactive.PubData.InactiveStamp=0;
    		else if(PowerFlag > 8)
    		{
    			for(i=0;i<24;i++) {LCD->BUF[i] = 0x0;}
    			Dl645Inactive.PubData.InactiveStamp=0;
    			LCD->CTRL = 0x00;
    			fnBeforeSleep();
    			Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWDN;
    			Dl645Inactive.PubData.ActiveState = End_State;
    			for(i=0;i<60;i++)
				{
					if(!(LCD->STA & 0x40)) break;
					fnRTCDrive_Delay();
				}
				fnDl645Bkgrd_SleepSet();
				fnDl645SubClock_Init();
    		}
    	}
    }
    while(Dl645Bkgrd.PubData.Active==BKGRD_ACTIVE_POWDN)
    {
        fnWDT_Restart();
        //if(Dl645Inactive.PubData.ActiveState == End_State)
    	{
    		fnDl645Bkgrd_Sleep();
    		fnWDT_Restart();
    	}
    	fnWDT_Restart();
    	if(Dl645Inactive.PubData.ActiveState==Running_State)
    	{
    		if((Dl645Inactive.PubData.ActiveStamp)&&(Dl645Inactive.PubData.ActiveStamp>30))
    		{
            	Dl645Inactive.InactiveFlag.SleepFlag=0;
    			LCD->CTRL = 0x0;
    			for(i=0;i<60;i++)
				{
					if(!(LCD->STA & 0x40)) break;
					fnRTCDrive_Delay();
				}
    			Dl645Inactive.PubData.ActiveState=End_State;
    			Dl645Inactive.PubData.ActiveInit=0;
    			Dl645Inactive.PubData.ActiveStamp=0;
    			fnDl645Bkgrd_IOSetSleep();
    		}
    	}
    	if(Dl645Inactive.InactiveFlag.fChange &0x01)   // 按键按下
    	{
    		fnDl645Bkgrd_SleepWakeupDisp();
    		Dl645Inactive.InactiveFlag.fChange &=0xfe;
    	}
    	if(Dl645Inactive.InactiveFlag.fChange &0x02)   // 秒进位
    	{
    		if(Dl645Inactive.PubData.ActiveState==Running_State) fnDl645Bkgrd_SleepRefreshDisp();
    		Dl645Inactive.InactiveFlag.fChange &=0xfd;
    	}
    	if(MADC->LVD_STAT & 0x04)
		{
			for(i=0;i<100;i++)
			{
				__NOP();
				__NOP();
				__NOP();
			}
			fnWDT_Restart();
			
			if(fnDl645Bkgrd_LVDCheck() < 2 ) 
			{
				Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWUP;
				Dl645Inactive.PubData.PowerErrFlag = 0x00;
				__disable_irq();
				fnTarget_Init();
				fnSysClock_Read(&SysClock);
				fnKey_Init();
				fnWDT_Restart();
				fnDl645Bkgrd_Init();
				fnWDT_Restart();
				fnScom_AllInit();
				fnWDT_Restart();
				fnDl645Disp_Init();
				fnDl645Front_Init();
				fnWDT_Restart();
				__enable_irq();
			}
		}
    }
	return;
}


void fnDl645Bkgrd_SleepWakeupDisp(void)
{
	u8	temp1;
	fnDl645RCClock_Init();
	eepromWakeup();
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->MOD0_EN |= 0x0004;
	if(Dl645Inactive.PubData.ActiveState == End_State) 
	{
		if(Dl645Inactive.PubData.ActiveInit==0x0)
		{
            Dl645Inactive.PubData.ActiveInit=0x1;
			SYSCTL->SYS_PS = 0x82;
			SYSCTL->MOD1_EN |= 0x0660;
			SYSCTL->SYS_PS = 0x00;
			GPIO->PMB &=0x0fe0ffff;
			GPIO->PMC &=0xffffff00;
			GPIO->PCB = 0xF01F0000;
			GPIO->PCC = 0x000000FF;	        		// P10
			GPIO->PCE = 0x0000000f;
            fnDl645Disp_Init();
		}
		Dl645Inactive.PubData.ActiveState=Running_State;
		Dl645Session.Mode=DISPLAYAUTO;
	}
	else Dl645Session.Mode=DISPLAYKEY;
	if(Dl645Session.Mode == DISPLAYAUTO)
	{
		fnWDT_Restart();
		fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5, Dl645Session.Item , 5);//循显项目
		fnDl645Disp_RefreshDispData();
		fnLcd_RefreshLcdBuf();
		fnWDT_Restart();
		fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 1 , (u8 *)&temp1,1);  //读取循显时间
		temp1 = fnBcdToHex_u8(temp1);
		if(temp1==0) temp1 =5;
		Dl645Session.Stamp=temp1;
	}
	else
	{
		fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 4 , (u8 *)&temp1,1);//读取按键显示屏数	
		temp1 = fnBcdToHex_u8(temp1);
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
		fnDl645Disp_RefreshDispData();
		fnLcd_RefreshLcdBuf();
	}
	Dl645Inactive.PubData.ActiveStamp = 0;
	SYSCTL->SYS_PS = 0x00;
	eepromStandby();
	if((Dl645Inactive.PubData.PowerErrFlag!=0x5a)&&(Dl645Inactive.PubData.PowerErrFlag!=0x5b)) fnDl645SubClock_Init();
}

void fnDl645Bkgrd_SleepRefreshDisp(void)
{
	u8	temp1;
	u8	i;
	if(((SysClock.Second)%2) | ((Dl645Session.Item[3]==4)&&(Dl645Session.Item[1]==1)&&(Dl645Session.Item[2]==0)&&((Dl645Session.Item[0]==1) || (Dl645Session.Item[0]==2))))
	{
		fnLcd_ClrScr();
		fnWDT_Restart();
		if(Dl645Session.Item[0]==1)
		{
			fnLcd_SetDigit(7,(SysClock.Day&0xf0)>>4);
			fnLcd_SetDigit(8,SysClock.Day&0x0f);
	      
			fnLcd_SetDigit(5,(SysClock.Month&0xf0)>>4);
			fnLcd_SetDigit(6,SysClock.Month&0x0f);
	      
			fnLcd_SetDigit(3,(SysClock.Year&0xf0)>>4);
			fnLcd_SetDigit(4,SysClock.Year&0x0f);
	      
			fnLcd_SetDigit(1,(0x20&0xf0)>>4);
			fnLcd_SetDigit(2,0x20&0x0f);
			//fnLcd_SetSegment(SEG_QsegDp2,1);
			fnLcd_SetSegment(SEG_QsegDp4,1);
			fnLcd_SetSegment(SEG_QsegDp6,1);
		}
		else
		{
		    fnLcd_SetSegment(SEG_dangqian,1);
			fnLcd_SetSegment(SEG_TIME,1);
			fnLcd_SetSegment(SEG_JIAN,1);
			
			fnLcd_SetDigit(7,(SysClock.Second&0xf0)>>4);
			fnLcd_SetDigit(8,SysClock.Second&0x0f);
	
		    fnLcd_SetDigit(5,(SysClock.Minute&0xf0)>>4);
		    fnLcd_SetDigit(6,SysClock.Minute&0x0f);
		     
		    fnLcd_SetDigit(3,(SysClock.Hour&0xf0)>>4);
		    fnLcd_SetDigit(4,SysClock.Hour&0x0f);
		     
			fnLcd_SetSegment(SEG_QsegDp9,1);
			fnLcd_SetSegment(SEG_QsegDp4,1);
			fnLcd_SetSegment(SEG_QsegDp8,1);
			fnLcd_SetSegment(SEG_QsegDp6,1);
		}
		fnLcd_RefreshLcdBuf();
		fnWDT_Restart();
	}
	if(Dl645Session.Mode == DISPLAYAUTO)
	{
		if(Dl645Inactive.PubData.ActiveStamp > Dl645Session.Stamp)
		{
			Dl645Inactive.PubData.ActiveStamp = 0;
			fnDl645RCClock_Init();
			eepromWakeup();
			SYSCTL->SYS_PS = 0x82;
			SYSCTL->MOD0_EN |= 0x0004;
			Dl645Session.AutoSerialNo++;
			
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara  , (u8 *)&temp1,1); //读取自动循显屏数
			temp1 = fnBcdToHex_u8(temp1);
			if(Dl645Session.AutoSerialNo >= temp1)		// 轮显周期到，关闭显示
			{
				eepromStandby();
				fnWDT_Restart();
				SYSCTL->SYS_PS = 0x00;
				fnDl645SubClock_Init();
				fnWDT_Restart();
				Dl645Inactive.InactiveFlag.SleepFlag=0;
    			LCD->CTRL = 0x0;
    			for(i=0;i<60;i++)
				{
					if(!(LCD->STA & 0x40)) break;
					fnRTCDrive_Delay();
				}
    			Dl645Inactive.PubData.ActiveState=End_State;
    			Dl645Inactive.PubData.ActiveInit=0;
    			Dl645Inactive.PubData.ActiveStamp=0;
    			fnWDT_Restart();
    			fnDl645Bkgrd_IOSetSleep();
			}
			else
			{
				fnWDT_Restart();
				fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + 5*Dl645Session.AutoSerialNo, Dl645Session.Item , 5);//循显项目
				if(Dl645Session.Item[4]==InvalidItem)
				{
					fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 , Dl645Session.Item,5);////轮显显示项目
				}
				fnWDT_Restart();
				fnDl645Disp_RefreshDispData();
				fnLcd_RefreshLcdBuf();
				eepromStandby();
				fnWDT_Restart();
				SYSCTL->SYS_PS = 0x00;
				fnDl645SubClock_Init();
			}
		}
	}
}









