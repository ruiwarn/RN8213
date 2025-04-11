
/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include <Includes.h>


int32_t  main ( void )
{
	__disable_irq();
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->SYS_PD = 0x001f;
    SYSCTL->MOD0_EN = 0xfff4;
    SYSCTL->MOD1_EN = 0x0f60;
    SYSCTL->INTC_EN = 0x01ff;
    SYSCTL->KBI_EN = 0x0000;
	SYSCTL->SYS_PS = 0x00;
	GPIO->PCA0 = 0x20000020;	// ����CMP2���
	GPIO->PIEA = 0xfffffcf7;
	fnLVD_Init();
	fnINTRTC_Init();
	NVIC_EnableIRQ(CMP_IRQn);
	fnDl645File_Read(Dl645FileId_FirmPara,Dl645FileItemInfoOffAddr_FirmPara_RTCDota0,(u8 *)&Dl645FirmPara.RTCDota0,2);
	RtcWriteDota(Dl645FirmPara.RTCDota0);
	
	if(fnDl645Bkgrd_LVDCheck()>2)
	{
		__enable_irq();
		Dl645Inactive.PubData.InactiveStamp=0;
    	Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWDN;
    	Dl645Inactive.PubData.ActiveState = End_State;
    	fnDl645Bkgrd_SleepSet();
    	fnDl645SubClock_Init();
    	fnDl645Bkgrd_Inactive();
    	__disable_irq();
    	fnTarget_Init();
	}
	
	SystemDelay(100);
    fnWDT_Restart();
    SystemDelay(100);
    fnWDT_Restart();
    SystemDelay(100);
    fnWDT_Restart();
    SystemDelay(100);
    fnWDT_Restart();				// �ȴ���1���ϵ���ȶ�
	fnTarget_Init();
	
	fnDl645File_Init();
	fnSysClock_Read(&SysClock);
	fnKey_Init();
	fnDl645Bkgrd_Init();
	fnScom_AllInit();
	fnWDT_Restart();
	fnDl645Disp_Init();
	//�ϵ�EMUģ���ʼ��
	Dl645Front.PriData.Flag=FRONT_FLAG_INIT;
	fnDl645Front_Init();
	__enable_irq();
	while (1)
	{
		fnWDT_Restart();
		fnDl645Front_Exec();  		//ǰ̨,����оƬ������������������,ǰִ̨��ʱ��800ms
  		fnDl645Bkgrd_Exec();     	//��̨�����������־��ʱ���־������ʱ���־,����ִ��ʱ��488ms

		fnWDT_Restart();
		//ͨѶ����ʾ����������������
  		fnDl645Disp_SessionExec();
		fnScomPk_Exec(SCOM_PORT_RS485A);
		fnScomPk_Exec(SCOM_PORT_IR);
		fnScomPk_Exec(SCOM_PORT_ZBCOM);
		fnWDT_Restart();
		fnICEsam_Card_Proc();
        Dl645Bkgrd.PubData.fChange.Flag = 0;
        fnDl645Bkgrd_Inactive();
        fnWDT_Restart();
	}
	/* End user code. Do not edit comment generated here */
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */

