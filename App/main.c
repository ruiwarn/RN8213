
/*
*******************************************************************************
**  Include files
*******************************************************************************
*/
#include <Includes.h>


int32_t  main ( void )
{
	__disable_irq();            // 关闭中断 // 关闭全局中断，防止初始化过程中被中断打断
	SYSCTL->SYS_PS = 0x82;      // 设置系统电源状态，具体含义参考芯片手册
	SYSCTL->SYS_PD = 0x001f;    // 设置系统掉电控制，关闭不必要的模块以节能
    SYSCTL->MOD0_EN = 0xfff4;  // 使能主模块0的时钟，具体位含义见手册
    SYSCTL->MOD1_EN = 0x0f60;  // 使能主模块1的时钟
    SYSCTL->INTC_EN = 0x01ff;  // 使能相关中断
    SYSCTL->KBI_EN = 0x0000;   // 禁用外部中断（KBI）
	SYSCTL->SYS_PS = 0x00;      // 恢复系统电源状态
	GPIO->PCA0 = 0x20000020;	// 开启CMP2检测
	GPIO->PIEA = 0xfffffcf7;    // 配置GPIO中断使能
	fnLVD_Init();               // 初始化低压检测模块
	fnINTRTC_Init();            // 初始化内部RTC
	NVIC_EnableIRQ(CMP_IRQn);   // 使能CMP中断
	fnDl645File_Read(Dl645FileId_FirmPara, Dl645FileItemInfoOffAddr_FirmPara_RTCDota0, (u8 *)&Dl645FirmPara.RTCDota0, 2); // 读取RTC校准参数
	RtcWriteDota(Dl645FirmPara.RTCDota0); // 写入RTC校准参数
	
	if(fnDl645Bkgrd_LVDCheck()>2) // 低压检测流程
	{
		__enable_irq(); // 允许中断
		Dl645Inactive.PubData.InactiveStamp=0; // 清除非活动时间戳
    	Dl645Bkgrd.PubData.Active=BKGRD_ACTIVE_POWDN; // 设置后台活动状态为掉电
    	Dl645Inactive.PubData.ActiveState = End_State;  // 设置非活动状态
    	fnDl645Bkgrd_SleepSet();    // 进入休眠准备
    	fnDl645SubClock_Init();     // 初始化子时钟
    	fnDl645Bkgrd_Inactive();         // 后台非活动处理    // 后台进入非活动处理
    	__disable_irq();            // 关闭中断 // 关闭全局中断，防止初始化过程中被中断打断
    	fnTarget_Init();            // 目标系统初始化
	}
	
	SystemDelay(100);           // 延时，确保系统稳定
    fnWDT_Restart();            // 喂狗，防止系统复位
    SystemDelay(100);           // 延时，确保系统稳定
    fnWDT_Restart();            // 喂狗，防止系统复位
    SystemDelay(100);           // 延时，确保系统稳定
    fnWDT_Restart();            // 喂狗，防止系统复位
    SystemDelay(100);           // 延时，确保系统稳定
    fnWDT_Restart();            // 喂狗，防止系统复位				// 等待第1次上电的稳定
	fnTarget_Init();            // 目标系统初始化
	
	fnDl645File_Init();      // DL645文件系统初始化
	fnSysClock_Read(&SysClock); // 读取系统时钟参数
	fnKey_Init();            // 按键初始化
	fnDl645Bkgrd_Init();     // 后台任务初始化
	fnScom_AllInit();        // 通讯接口初始化
	fnWDT_Restart();            // 喂狗，防止系统复位
	fnDl645Disp_Init();      // 显示模块初始化
	//上电EMU模块初始化
	Dl645Front.PriData.Flag=FRONT_FLAG_INIT; // 设置前台标志为初始化
	fnDl645Front_Init();    // 前台初始化
	__enable_irq(); // 允许中断
	while (1) // 主循环，持续运行
	{
		fnWDT_Restart();            // 喂狗，防止系统复位
		fnDl645Front_Exec();     // 前台主循环，处理计量、数据采集等，典型周期800ms
  		fnDl645Bkgrd_Exec();     // 后台主循环，处理数据存储、定时任务等，典型周期488ms

		fnWDT_Restart();            // 喂狗，防止系统复位
		//通讯，显示，按键等其他处理
  		fnDl645Disp_SessionExec(); // 显示会话处理
		fnScomPk_Exec(SCOM_PORT_RS485A); // RS485A端口通讯
		fnScomPk_Exec(SCOM_PORT_IR);     // 红外端口通讯
		fnScomPk_Exec(SCOM_PORT_ZBCOM);  // 载波通讯端口
		fnWDT_Restart();            // 喂狗，防止系统复位
		fnICEsam_Card_Proc();            // IC卡处理
        Dl645Bkgrd.PubData.fChange.Flag = 0; // 清除后台数据变更标志
        fnDl645Bkgrd_Inactive();         // 后台非活动处理    // 后台进入非活动处理
        fnWDT_Restart();            // 喂狗，防止系统复位
	}
	/* End user code. Do not edit comment generated here */
}

/* Start adding user code. Do not edit comment generated here */
/* End user code adding. Do not edit comment generated here */

