/*
 * @file: Dl645_Disp.C
 * @brief: 处理LCD显示相关逻辑，包括初始化、数据显示格式化、状态刷新和显示会话管理。
 * @author: [Auto-generated by Roo]
 * @date: 2025/04/11
 * @description: 此文件负责将内部数据转换为LCD可显示的格式，并控制显示流程（自动轮显、按键显示）。
 *               注释已添加以解释寄存器操作和关键流程，目标是让新手易懂。
 */

#define  DL645_Disp_GLOBALS
#include <App.h>

/**************************
				变量定义
***************************/
// 此处可能定义了一些文件作用域的变量，但当前为空

/*****************常量定义******************/
// 此处可能定义了一些文件作用域的常量，但当前为空

/**
 * @brief 初始化LCD显示模块。
 * @param None
 * @return None
 * @note 配置LCD控制器寄存器，根据系统状态设置不同的工作模式，并初始化显示会话状态。
 */
void fnDl645Disp_Init(void)
{
	u8	i; // 循环计数器和临时变量
	fnLcd_Init(); // 调用底层LCD驱动初始化函数
	LCD->CTRL = 0x0; // 先关闭LCD控制器？(CTRL寄存器位定义需查手册)
	
    #if !DL645SOFT_DEBUG // 非软件调试模式下执行
    // 等待LCD控制器状态稳定（例如，等待忙碌标志清除）
    for(i = 0; i < 200; i++)
	{
		// 检查LCD状态寄存器STA的bit 6 (0x40)，可能表示忙碌状态
		if(!(LCD->STA & 0x40)) break; // 如果不忙，退出循环
		SystemDelay(1); // 延时1ms
	}
   	#endif
	
	// 根据后台活动状态设置LCD控制器
    if(Dl645Bkgrd.PubData.Active == BKGRD_ACTIVE_POWDN) // 如果处于掉电/休眠状态
    {
		// 配置系统控制器和LCD控制器以适应低功耗模式
    	SYSCTL->SYS_PS = 0x75; // 设置系统电源状态寄存器 (0x75含义需查手册)
		// 直接写内存地址，可能是配置LCD偏压或对比度相关的特殊寄存器？
		// 地址0x4003405c和值0xa5790020的具体含义需要参考芯片手册或硬件设计
		*(uint32_t *)(0x4003405c) |= 0xa5790020;
		SYSCTL->SYS_PS = 0x0; // 恢复系统电源状态
    	LCD->CTRL = 0xBFB; // 设置LCD控制寄存器，配置低功耗模式下的显示参数 (0xBFB位定义需查手册)
    }
    else // 正常工作状态
	{
		LCD->CTRL = 0x3FB; // 设置LCD控制寄存器，配置正常工作模式下的显示参数 (0x3FB位定义需查手册)
	}
	
    LCD->CLKDIV = 0x1F; // 设置LCD时钟分频寄存器 (0x1F表示的分频系数需查手册)
    LCD->BLINK = 0x000001; // 设置LCD闪烁控制寄存器，可能启用某种闪烁模式 (0x01位定义需查手册)
	
	// 读取上电全显时间配置
	fnDl645File_Read(Dl645FileId_GeneralPara, Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + 10 * DL645_MAX_CYCDISP , &i , 1 );
	i = fnBcdToHex_u8(i); // BCD转HEX
	
	fnLcd_ClrScr(); // 清除LCD屏幕
	
	// 初始化显示会话结构体
	memset(&Dl645Session, 0, sizeof(sDl645B_Session_TYPE)); // 清零会话结构体
	Dl645Session.Mode = DISPLAYAUTO; // 默认设置为自动循环显示模式
    memset(Dl645Session.Item, 0xFF, 5); // 初始化当前显示项为无效
    Dl645Session.BkLightFlag = 0x5A; // 设置背光标志为开启？(0x5A作为特殊标记)
    Dl645Session.BkLightStamp = 255 - i; // 初始化背光计时器（倒计时？）
    Dl645Session.Stamp = i; // 初始化显示项切换计时器
  	Dl645Session.DisplyErrFlag = 1; // 设置显示错误标志？（可能表示需要刷新）
  	memset(&Remote, 0, sizeof(sIRRemote_TypeDef)); // 清零红外遥控相关结构体？(Remote未在此文件定义)
  	Dl645Session.AllOnDispTime = i; // 设置上电全显时间
  	return;
}

/**
 * @brief 根据数据类型格式化数据并在LCD上显示。
 * @param Buf 指向包含待显示数据的缓冲区。
 * @param DataType 数据类型代码，决定显示格式。
 * @param Length 数据长度（字节）。
 * @return None
 * @note 此函数处理多种数据类型，包括数值（带小数位）、日期、时间、十六进制等。
 */
void fnDl645Disp_Data(u8 *Buf, u8 DataType, u8 Length)
{
  	u8 i = Length; // 循环计数器，初始化为长度
  	u8 temp; // 临时存储BCD码位
  	u8 ZeroStatus = 0x0; // 前导零抑制标志

	// 处理数值类型 (DataType 0-6，表示小数位数)
	if((DataType <= 0x6))
	{
		// 处理负号
		if(Buf[Length - 1] & 0x80) // 检查最高字节的最高位
		{	// 如果是负数
			Buf[Length - 1] &= 0x7F; // 清除符号位
			fnLcd_SetSegment(SEG_FuHao, 1); // 显示负号段
		}
		// 逐位显示BCD码，带前导零抑制和小数点处理
		for(i = 0; i < 8; i++) // 假设最多显示8位数字
		{
			// 从BCD缓冲区提取数字位
			if(i % 2 == 0x0) // 偶数位，取高半字节
				{temp = Buf[3 - (i >> 1)] >> 4;}
			else // 奇数位，取低半字节
				{temp = Buf[3 - (i >> 1)] & 0x0F;}
			
			// 根据实际数据长度调整，高位补零（实际显示为空白）
			if((Length == 3) && (i < 2)) temp = 0; // 3字节数据，前两位为0
			if((Length == 2) && (i < 4)) temp = 0; // 2字节数据，前四位为0
			
			// 前导零抑制
			if(temp == 0x0 && ZeroStatus == 0x0) // 如果是零且还未显示过非零数字
			{
				// 如果当前位在小数点左侧，则显示空白 (0x10可能代表空白)
				if((7 - i) > DataType)
        		{
					fnLcd_SetDigit(i + 1, 0x10);
					continue; // 继续下一位
				}
			}
			fnLcd_SetDigit(i + 1, temp); // 显示数字位
			ZeroStatus = 1; // 标记已显示过非零数字
		}
		// 控制小数点段的显示
		fnLcd_SetSegment(SEG_QsegDp7, 0); // 清除所有可能的小数点
		fnLcd_SetSegment(SEG_QsegDp6, 0);
		fnLcd_SetSegment(SEG_QsegDp5, 0);
		fnLcd_SetSegment(SEG_QsegDp4, 0);
		fnLcd_SetSegment(SEG_QsegDp3, 0);
		// 根据DataType设置对应的小数点
		if(DataType == 0x1) {fnLcd_SetSegment(SEG_QsegDp7, 1);} // 1位小数
		if(DataType == 0x2) {fnLcd_SetSegment(SEG_QsegDp6, 1);} // 2位小数
		if(DataType == 0x3) {fnLcd_SetSegment(SEG_QsegDp5, 1);} // 3位小数
		if(DataType == 0x4) {fnLcd_SetSegment(SEG_QsegDp4, 1);} // 4位小数
		if(DataType == 0x5) {fnLcd_SetSegment(SEG_QsegDp3, 1);} // 5位小数
	}
  	else if (DataType == 0x7) // 显示日期 (YYMMDD) + 前缀 "20"
	{
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 日高位
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 日低位
		fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 月高位
		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 月低位
		fnLcd_SetDigit(3, (Buf[2] & 0xF0) >> 4); // 年高位
		fnLcd_SetDigit(4, Buf[2] & 0x0F);      // 年低位
		fnLcd_SetDigit(1, (Buf[3] & 0xF0) >> 4); // 年份前缀高位 (例如 '2')
		fnLcd_SetDigit(2, Buf[3] & 0x0F);      // 年份前缀低位 (例如 '0')
		fnLcd_SetSegment(SEG_QsegDp4, 1); // 显示分隔符 "-"
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 "-"
	}
  	else if(DataType == 0x8)  // 显示日时段表起始时间及其费率 (格式: RR--HH:MM)
	{
		fnLcd_SetDigit(1, (Buf[0] & 0xF0) >> 4); // 费率高位
		fnLcd_SetDigit(2, Buf[0] & 0x0F);      // 费率低位
		fnLcd_SetDigit(5, (Buf[2] & 0xF0) >> 4); // 小时高位
		fnLcd_SetDigit(6, Buf[2] & 0x0F);      // 小时低位
		fnLcd_SetDigit(7, (Buf[1] & 0xF0) >> 4); // 分钟高位
		fnLcd_SetDigit(8, Buf[1] & 0x0F);      // 分钟低位
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 ":"
		fnLcd_SetSegment(SEG_QsegDp8, 1); // 可能用于闪烁？
		fnLcd_SetSegment(SEG_Qseg3g, 1); // 显示分隔符 "--"
		fnLcd_SetSegment(SEG_Qseg4g, 1); // 显示分隔符 "--"
	}
	else if(DataType == 0x9) // 显示十六进制数
  	{
		for(i = 0; i < Length; i++) // 从低字节开始显示
		{
			fnLcd_SetDigit(7 - i * 2, (*(Buf + i) & 0xF0) >> 4); // 高半字节
			fnLcd_SetDigit(7 - i * 2 + 1, *(Buf + i) & 0x0F);   // 低半字节
		}
  	}
    else if(DataType == 0xa)  // 显示日、时、分 (格式: DD HH:MM)
	{
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 分钟高位
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 分钟低位
		fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 小时高位
		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 小时低位
		fnLcd_SetDigit(3, (Buf[2] & 0xF0) >> 4); // 日高位
		fnLcd_SetDigit(4, Buf[2] & 0x0F);      // 日低位
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 ":"
		fnLcd_SetSegment(SEG_QsegDp8, 1); // 可能用于闪烁？
		fnLcd_SetSegment(SEG_QsegDp4, 1); // 显示空格或分隔符？
	}
	else if (DataType == 0xb) // 显示年、月、日 (格式: YY-MM-DD)
	{
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 日高位
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 日低位
		fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 月高位
		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 月低位
		fnLcd_SetDigit(3, (Buf[2] & 0xF0) >> 4); // 年高位
		fnLcd_SetDigit(4, Buf[2] & 0x0F);      // 年低位
		fnLcd_SetSegment(SEG_QsegDp4, 1); // 显示分隔符 "-"
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 "-"
	}
	else if(DataType == 0xc) // 显示月、日、时、分 (格式: MM-DD HH:MM)
	{
  		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 分钟高位
  		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 分钟低位
 	 	fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 小时高位
  		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 小时低位
  		fnLcd_SetDigit(3, (Buf[2] & 0xF0) >> 4); // 日高位
  		fnLcd_SetDigit(4, Buf[2] & 0x0F);      // 日低位
  		fnLcd_SetDigit(1, (Buf[3] & 0xF0) >> 4); // 月高位
  		fnLcd_SetDigit(2, Buf[3] & 0x0F);      // 月低位
  		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 ":"
  		fnLcd_SetSegment(SEG_QsegDp4, 1); // 显示分隔符 "-"
  		fnLcd_SetSegment(SEG_QsegDp8, 1); // 可能用于闪烁？
	}
	else if(DataType == 0xd) // 显示时、分、秒 (格式: HH:MM:SS)
    {
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 秒高位
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 秒低位
	    fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 分钟高位
	    fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 分钟低位
	    fnLcd_SetDigit(3, (Buf[2] & 0xF0) >> 4); // 小时高位
	    fnLcd_SetDigit(4, Buf[2] & 0x0F);      // 小时低位
		fnLcd_SetSegment(SEG_QsegDp9, 1); // 显示分隔符 ":"
		fnLcd_SetSegment(SEG_QsegDp4, 1); // 显示分隔符 ":"
		fnLcd_SetSegment(SEG_QsegDp8, 1); // 可能用于闪烁？
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 可能用于闪烁？
    }
	else if(DataType == 0xe) // 显示状态字 (4位BCD?)
  	{
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 最低位
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 次低位
		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 次高位
		fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 最高位
  	}
	else if(DataType == 0xf) // 显示日--时 (格式: DD--HH)
    {
	    fnLcd_SetDigit(3, (Buf[1] & 0xF0) >> 4); // 小时高位
	    fnLcd_SetDigit(4, Buf[1] & 0x0F);      // 小时低位
	    fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 日高位
	    fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 日低位
	    fnLcd_SetSegment(SEG_Qseg5g, 1); // 显示分隔符 "--"
	    fnLcd_SetSegment(SEG_Qseg6g, 1); // 显示分隔符 "--"
    }
    else if(DataType == 0x10)  // 显示 时、分 (格式: HH:MM)
	{
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 分钟高位
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 分钟低位
		fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 小时高位
		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 小时低位
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 ":"
		fnLcd_SetSegment(SEG_QsegDp8, 1); // 可能用于闪烁？
	}
    else if(DataType == 0x11)  // 显示日时区表起始时间及时段表号 (格式: RR-HH:MM)
	{
		fnLcd_SetDigit(1, (Buf[0] & 0xF0) >> 4); // 时段表号高位
		fnLcd_SetDigit(2, Buf[0] & 0x0F);      // 时段表号低位
		fnLcd_SetDigit(5, (Buf[2] & 0xF0) >> 4); // 小时高位
		fnLcd_SetDigit(6, Buf[2] & 0x0F);      // 小时低位
		fnLcd_SetDigit(7, (Buf[1] & 0xF0) >> 4); // 分钟高位
		fnLcd_SetDigit(8, Buf[1] & 0x0F);      // 分钟低位
		fnLcd_SetSegment(SEG_QsegDp6, 1); // 显示分隔符 ":"
		fnLcd_SetSegment(SEG_Qseg3g, 1); // 显示分隔符 "-"
		fnLcd_SetSegment(SEG_Qseg4g, 1); // 显示分隔符 "-"
	}
	else if(DataType == 0x12) // 显示模式字、特征字 (2位BCD?)
  	{
		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 低位
		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 高位
  	}
  	else if(DataType == 0x13) // 显示 年 月 日 时 (格式: YYMMDDHH)
  	{
  		fnLcd_SetDigit(7, (Buf[0] & 0xF0) >> 4); // 时高位
  		fnLcd_SetDigit(8, Buf[0] & 0x0F);      // 时低位
 	 	fnLcd_SetDigit(5, (Buf[1] & 0xF0) >> 4); // 日高位
  		fnLcd_SetDigit(6, Buf[1] & 0x0F);      // 日低位
  		fnLcd_SetDigit(3, (Buf[2] & 0xF0) >> 4); // 月高位
  		fnLcd_SetDigit(4, Buf[2] & 0x0F);      // 月低位
  		fnLcd_SetDigit(1, (Buf[3] & 0xF0) >> 4); // 年高位
  		fnLcd_SetDigit(2, Buf[3] & 0x0F);      // 年低位
  	}
	else  // 非法显示类型
	{
		for (i = 1; i < 9; i++) fnLcd_SetDigit(i, 0x11); // 显示 "--------"
	}
	return;
}


/**
 * @brief 刷新LCD上的状态指示符。
 * @param None
 * @return None
 * @note 根据全局变量更新通信状态和功率方向指示。
 */
void fnDl645Disp_RefreshStatus(void)
{
   	fnLcd_SetSegment(SEG_Phone, 0); // 清除电话符号（通信指示）
  	if (Dl645Bkgrd.PubData.CommStatus & F_Comm1Event) // 检查通信事件标志
	{
		Dl645Bkgrd.PubData.CommStatus &= ~F_Comm1Event; // 清除标志
    	fnLcd_SetSegment(SEG_Phone, 1); // 显示电话符号
	}

	// 根据功率方向更新反向标志
  	if (Dl645Front.PubData.PDirect == INVERSION) // 如果功率反向
    {
		fnLcd_SetSegment(SEG_FanXiang, 1); // 显示反向标志
	}
  	else // 功率正向
	{
		fnLcd_SetSegment(SEG_FanXiang, 0); // 清除反向标志
	}
 	return;
}


/**
 * @brief 根据当前显示会话中的项目代码，刷新LCD显示数据。
 * @param None
 * @return None
 */
void fnDl645Disp_RefreshDispData(void)
{
	fnLcd_ClrScr(); // 清屏
	
	// 根据显示项代码的最高字节判断要显示的数据类型
  	switch(Dl645Session.Item[3]) // Item[3] 存储数据类型标识
  	{
    	case 2: // 显示瞬时变量
    		fnDl645Disp_Variable();
    		break;
    	case 4: // 显示通用参数
      		fnDl645Disp_GeneralPara();
      		break;
    	case 0xFF: // 全显测试
    		fnLcd_AllOn();
      		return; // 直接返回，不刷新状态
    	default: // 其他或无效类型
      		break;
  	}
	
    // 如果系统不处于掉电状态，刷新状态指示符
  	if(Dl645Bkgrd.PubData.Active != BKGRD_ACTIVE_POWDN) fnDl645Disp_RefreshStatus();
  	return;
}

/**
 * @brief 显示瞬时变量数据。
 * @param None
 * @return None
 * @note 根据Dl645Session.Item中的代码获取并格式化显示电压、电流、功率、功率因数等。
 */
void fnDl645Disp_Variable(void)
{
	u8	DataType, Buffer[8]; // 数据类型和临时缓冲区
  	u8 Length = 0, i; // 数据长度和循环计数器
  	u16 len0; // 存储GetVar返回的长度
  
	// 清零缓冲区
	for(i = 0; i < 8; i++)
	{
		Buffer[i] = 0;
	}
	// 获取变量数据
	fnDl645_GetVar(Dl645Session.Item[2], Dl645Session.Item[1], Dl645Session.Item[0], Buffer, &len0);
	Length = (u8)len0; // 获取实际长度
  	if(Length > 8) Length = 0; // 长度校验
	
	// 根据数据标识符确定显示格式和单位符号
	switch(Dl645Session.Item[2]) // Item[2] 通常表示数据大类
	{
		case 1: // 电压
			fnLcd_SetSegment(SEG_V1, 1); // 显示电压单位 "V"
			DataType = 1; // 1位小数
			break;
		case 2: // 电流
			fnLcd_SetSegment(SEG_A1, 1); // 显示电流单位 "A"
			fnLcd_SetSegment(SEG_L, 1); // 显示线路 "L"？
			DataType = 3; // 3位小数
			break;
		case 3: // 功率
			fnLcd_SetSegment(SEG_KW, 1); // 显示功率单位 "kW"
			// 读取配置的功率小数位数
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 3 , &DataType , 1);
			// 根据小数位数调整缓冲区数据（BCD码移位）
			if(DataType == 0)
	        { // 0位小数
	        	Buffer[0] = (0x7F & Buffer[2]); // 取高字节（去除符号位）
	        	Buffer[1] = 0x00;
	        	Buffer[2] = (0x80 & Buffer[2]); // 保留符号位
	        }
	        else if(DataType == 1)
	        { // 1位小数
	        	Buffer[0] = ((u8)(0x7F & Buffer[2]) << 4) | (Buffer[1] >> 4);
	        	Buffer[1] = ((0x7F & Buffer[2]) >> 4);
	        	Buffer[2] = (0x80 & Buffer[2]);
	        }
	        else if(DataType == 2)
	        { // 2位小数
	        	Buffer[0] = Buffer[1];
	        	Buffer[1] = (0x7F & Buffer[2]);
	        	Buffer[2] = (0x80 & Buffer[2]);
	        }
	        else if(DataType == 3)
	        { // 3位小数
	        	Buffer[0] = ((u8)Buffer[1] << 4) | (Buffer[0] >> 4);
	        	Buffer[1] = ((u8)(0x7F & Buffer[2]) << 4) | (Buffer[1] >> 4);
	        	Buffer[2] = (0x80 & Buffer[2]) | ((0x7F & Buffer[2]) >> 4);
	        }
	        else // 默认4位小数
	        {
	        	DataType = 4;
	        }
			break;
		case 6: // 功率因数
			if(Dl645Session.Item[1] == 0) // Item[1] 可能表示总/分相
			{
				fnLcd_SetSegment(SEG_Qzong, 1); // 显示 "总" 标志
			}
			fnLcd_SetSegment(SEG_COS, 1); // 显示 "COS" 标志
			DataType = 3; // 3位小数
			break;
		case 7: // 相角
			fnLcd_SetSegment(SEG_COS, 1); // 复用 "COS" 标志？可能应有角度符号
			DataType = 1; // 1位小数
			break;
		case 0x80: // 其他自定义变量
			if(Dl645Session.Item[0] == 1) // 零线电流
			{
				fnLcd_SetSegment(SEG_A1, 1); // 显示 "A"
				fnLcd_SetSegment(SEG_N, 1); // 显示 "N" (零线)
				DataType = 3; // 3位小数
			}
			else if(Dl645Session.Item[0] == 2) // 电网频率
			{
				// 单位符号应在fnLcd_SetSegment中设置，例如 SEG_Hz
				DataType = 2; // 2位小数
			}
			else if(Dl645Session.Item[0] == 7) // 表内温度
			{
				// 单位符号应在fnLcd_SetSegment中设置，例如 SEG_C
				DataType = 1; // 1位小数
			}
			else if(Dl645Session.Item[0] == 8) // 时钟电池电压
			{
				fnLcd_SetSegment(SEG_V1, 1); // 显示 "V"
				DataType = 2; // 2位小数
			}
			else if(Dl645Session.Item[0] == 0x0B) // 当前阶梯电价
			{
    			fnLcd_SetSegment(SEG_dangqian, 1); // "当前"
				fnLcd_SetSegment(SEG_Yuan, 1);     // "元"
				fnLcd_SetSegment(SEG_JieTi, 1);    // "阶梯"
				fnLcd_SetSegment(SEG_Qdian, 1);    // "电"
				fnLcd_SetSegment(SEG_Jia, 1);      // "价"
				DataType = 4; // 4位小数
			}
			else // 未知类型
			{
				DataType = 9; // 显示为十六进制
			}
			break;
	}
	// 调用数据显示函数
	fnDl645Disp_Data(Buffer, DataType, Length);
   	return;
}

/**
 * @brief 显示通用参数数据（主要是日期和时间）。
 * @param None
 * @return None
 * @note 根据Dl645Session.Item中的代码获取并格式化显示日期或时间。
 */
void fnDl645Disp_GeneralPara(void)
{
	u8	DataType = 0xFF; // 初始化为无效类型
  	u8	*Buffer, Length = 0; // 缓冲区指针和长度
  	u8	*Buf; // 动态分配内存指针
	
	Buf = malloc(50); // 分配临时缓冲区
	if (Buf == NULL) return; // 内存分配失败则返回
	Buffer = Buf; // 指向缓冲区
	
	// 根据数据标识符判断显示内容
	if((Dl645Session.Item[1] == 1) && (Dl645Session.Item[2] == 0) && (Dl645Session.Item[0] == 1)) // 显示日期
	{
		Buffer[0] = SysClock.Day;
		Buffer[1] = SysClock.Month;
		Buffer[2] = SysClock.Year;
		Buffer[3] = 0x20; // 年份前缀 "20"
		Length = 4;
		DataType = 7; // 日期格式
	}
	else if((Dl645Session.Item[1] == 1) && (Dl645Session.Item[2] == 0) && (Dl645Session.Item[0] == 2)) // 显示时间
	{
       	Buffer[0] = SysClock.Second;
		Buffer[1] = SysClock.Minute;
		Buffer[2] = SysClock.Hour;
		Length = 3;
		DataType = 0xd; // 时分秒格式
	    fnLcd_SetSegment(SEG_dangqian, 1); // 显示 "当前"
		fnLcd_SetSegment(SEG_TIME, 1);     // 显示 "时间"
		fnLcd_SetSegment(SEG_JIAN, 1);     // 显示费率时段？
	}
	else if((Dl645Session.Item[1] == 1) && (Dl645Session.Item[2] == 0) && (Dl645Session.Item[0] == 0x0c)) // 分别显示日期和时间
	{
		if(Dl645Session.Item[4] == 1) // Item[4]可能用于区分日期和时间
		{ // 显示时间
			Buffer[0] = SysClock.Second;
			Buffer[1] = SysClock.Minute;
			Buffer[2] = SysClock.Hour;
			Length = 3;
			DataType = 0xd; // 时分秒格式
	    	fnLcd_SetSegment(SEG_dangqian, 1);
			fnLcd_SetSegment(SEG_TIME, 1);
			fnLcd_SetSegment(SEG_JIAN, 1);
		}
		else if(Dl645Session.Item[4] == 0)
		{ // 显示日期
			Buffer[0] = SysClock.Day;
			Buffer[1] = SysClock.Month;
			Buffer[2] = SysClock.Year;
			Buffer[3] = 0x20;
			Length = 4;
			DataType = 7; // 日期格式
		}
	}
	
	// 调用数据显示函数
	fnDl645Disp_Data(Buffer, DataType, Length);
	free(Buf); // 释放内存
}

/**
 * @brief LCD显示会话执行函数。
 * @param None
 * @return None
 * @note 管理显示模式（自动/按键），处理按键事件，控制背光，切换显示项目。
 */
void fnDl645Disp_SessionExec(void)
{
	u8 temp0, temp1; // 临时变量
	
	// 如果系统处于掉电或非活动状态，直接返回
	if((Dl645Bkgrd.PubData.Active == BKGRD_ACTIVE_POWDN) || (Dl645Inactive.PubData.InactiveStamp)) return;
	
	// 控制背光
	if(Dl645Session.BkLightFlag == 0x5A) PinWrite_LCDLight(1); // 如果背光标志设置，点亮背光
	
	// 处理按键事件
	if(Key.Press & KEY_VAL_TURN) // 如果检测到按键按下
	{
		// 如果在休眠状态下按键，延长唤醒时间
		if(Dl645Bkgrd.PubData.Active == BKGRD_ACTIVE_POWDN)
			Dl645Inactive.PubData.ActiveStamp = SysStamp ? SysStamp : (SysStamp - 1); // 重置唤醒时间戳
		
		// 如果在正常工作状态下按键，启动背光计时
		if((Dl645Bkgrd.PubData.Active != BKGRD_ACTIVE_POWDN) && (Key.Press & KEY_VAL_TURN))
		{
			Dl645Session.BkLightFlag = 0x5A; // 设置背光标志
			Dl645Session.BkLightStamp = 195; // 启动背光计时器（例如，倒计时到255）
		}
		Dl645Session.Mode = DISPLAYKEY; // 切换到按键显示模式
		Dl645Session.Stamp = 0; // 重置按键模式超时计时器
		Dl645Session.DisplyErrFlag = 1; // 设置刷新标志
		goto Start_Display; // 跳转到显示处理部分
	}
	
	// 如果没有秒变化标志，直接返回（减少CPU占用）
	if(!Dl645Bkgrd.PubData.fChange.FlagBit.Second) return;
	
	// 处理背光超时
	if(Dl645Session.BkLightStamp) // 如果背光计时器在运行
	{
		if(++Dl645Session.BkLightStamp > 254) // 计时器增加，如果超时
		{
			Dl645Session.BkLightFlag = 0x0; // 清除背光标志
			PinWrite_LCDLight(0); // 关闭背光
			Dl645Session.BkLightStamp = 0; // 停止计时器
		}
	}
	else // 如果计时器未运行（已超时或未启动）
	{
		Dl645Session.BkLightFlag = 0x0; // 确保标志清除
		PinWrite_LCDLight(0); // 确保背光关闭
		Dl645Session.BkLightStamp = 0; // 确保计时器停止
	}
	
Start_Display: // 显示处理跳转标签
	if (Dl645Session.Mode == DISPLAYAUTO) // 自动循环显示模式
	{
		if(Dl645Session.AllOnDispTime) Dl645Session.AllOnDispTime--; // 上电全显时间递减
		
		// 读取自动轮显时间间隔
		fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 1 , (u8 *)&temp0, 1);
		temp0 = fnBcdToHex_u8(temp0); // BCD转HEX
		if(temp0 == 0) temp0 = 5; // 默认5秒
		
		// 如果当前显示项时间到
		if((--Dl645Session.Stamp == 0) && (temp0))
		{
			Dl645Session.DisplyErrFlag = 1; // 设置刷新标志
			Dl645Session.Stamp = temp0; // 重置当前项显示时间戳
			
			// 读取下一个要显示的项目
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + Dl645Session.AutoSerialNo * 5 , Dl645Session.Item , 5);
			
			// 读取总的自动轮显屏数
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara , (u8 *)&temp1, 1);
			temp1 = fnBcdToHex_u8(temp1); // BCD转HEX
			
			// 切换到下一个项目序号，如果超过总数或项目无效，则回到第一个项目
			if((++Dl645Session.AutoSerialNo > temp1) || (Dl645Session.Item[4] == InvalidItem))
			{
				Dl645Session.AutoSerialNo = 0x1; // 回到第一个项目
				fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 , Dl645Session.Item , 5); // 读取第一个项目
			}
		}
	}
	else // 按键显示模式
	{
		if(Dl645Session.AllOnDispTime) Dl645Session.AllOnDispTime = 0; // 如果处于上电全显状态，按键后立即结束
		
		temp0 = 30; // 按键显示默认超时时间30秒
		if (++Dl645Session.Stamp >= temp0) // 如果超时
		{
			// 切换回自动显示模式
			Dl645Session.Stamp = 1; // 重置时间戳
			Dl645Session.Mode = DISPLAYAUTO; // 设置模式
			Dl645Session.DigitPosition = 2; // 重置数字位置？
			Dl645Session.KeySeriaNo = 0; // 清零按键序号
		}
		else // 未超时
		{
			// 读取按键显示总屏数
			fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 4 , (u8 *)&temp1, 1);
			temp1 = fnBcdToHex_u8(temp1); // BCD转HEX
			
			if (Key.Press & KEY_VAL_TURN) // 如果再次检测到按键按下（在goto之前已处理过一次，这里处理切换项目）
			{
				Dl645Session.Mode = DISPLAYKEY; // 确保是按键模式
				if(++Dl645Session.KeySeriaNo > temp1) // 切换到下一个按键项目序号
       			{
					Dl645Session.KeySeriaNo = 1; // 循环到第一个
				}
				// 读取对应的按键显示项目
				fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + 5 * DL645_MAX_CYCDISP + (Dl645Session.KeySeriaNo - 1) * 5 , Dl645Session.Item, 5);
				if(Dl645Session.Item[4] == InvalidItem) // 如果项目无效，回到第一个
				{
					fnDl645File_Read(Dl645FileId_GeneralPara , Dl645FileItemInfoOffAddr_GeneralPara_DispPara + 5 + 5 * DL645_MAX_CYCDISP , Dl645Session.Item, 5);
	      			Dl645Session.KeySeriaNo = 0;
				}
			}
		}
	}
	
	// 清除按键标志
	if(Key.Press & KEY_VAL_TURN)
	{
		Key.Press &= ~KEY_VAL_TURN;
	}

	// 根据刷新标志决定是刷新数据还是只刷新状态
	if(Dl645Session.DisplyErrFlag)
	{
    	fnDl645Disp_RefreshDispData(); // 刷新整个显示数据
	}
	else
	{
		fnDl645Disp_RefreshStatus(); // 只刷新状态指示符
	}
	fnLcd_RefreshLcdBuf(); // 将缓冲区内容更新到LCD
	return;
}
