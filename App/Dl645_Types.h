/*
 * @file: Dl645_Types.h
 * @brief: 定义DL/T645协议和其他模块使用的基本数据格式 (DF)。
 * @author: [Auto-generated by Roo]
 * @date: 2025/04/11
 * @description: 此文件定义了多种sDFxx结构体，用于表示日期、时间、数值、地址等标准数据格式。
 *               还定义了一些基于这些基本格式的应用数据结构。
 */
#ifndef __DL645_TYPE_H
#define __DL645_TYPE_H

//=== 基本数据格式定义 ===
// 设置结构体按1字节对齐，确保内存布局符合协议要求
#pragma pack(1)

/**
 * @brief DF01: 日期和时间 (BCD码, 6字节)
 * SSMMHHDDMMYY (秒分时日月年) + 周信息
 */
typedef struct {
	u8 Second;    // 秒 (BCD)
	u8 Minute;    // 分 (BCD)
	u8 Hour;      // 时 (BCD)
	u8 Day;       // 日 (BCD)
	u8 Month : 5; // 月 (BCD, 5位)
	u8 Week  : 3; // 星期 (3位, 1-7 或 0-6?)
	u8 Year;      // 年 (BCD)
} sDF01;

/**
 * @brief DF02: 带符号和分组的12位数据 (2字节)
 * 通常用于表示带有状态或分组信息的小数值。
 */
typedef struct {
	u16 Dat : 12; // 数据位 (12位)
	u16 S   : 1;  // 符号位 (0:正, 1:负)
	u16 G   : 3;  // 分组或其他状态位
} sDF02;

/**
 * @brief DF03: 带符号和分组的28位数据 (4字节)
 * 通常用于表示带有状态或分组信息的大数值。
 */
typedef struct {
	u32 Dat      : 28; // 数据位 (28位)
	u32 S        : 1;  // 符号位 (0:正, 1:负)
	u32 Reserve1 : 1;  // 保留位
	u32 G        : 1;  // 分组或其他状态位
	u32 Reserve2 : 1;  // 保留位
} sDF03;

/**
 * @brief DF04: 带符号的7位数据 (1字节)
 */
typedef struct {
	u8 Dat : 7; // 数据位 (7位)
	u8 S   : 1; // 符号位 (0:正, 1:负)
} sDF04;

/**
 * @brief DF05: 带符号的15位数据 (2字节)
 * 常用于表示角度等。
 */
typedef struct {
	u16 Dat : 15; // 数据位 (15位)
	u16 S   : 1;  // 符号位 (0:正, 1:负)
} sDF05;

/**
 * @brief DF06: 带符号的15位数据 (2字节) - 与DF05相同?
 * DL/T645标准中DF06通常用于电压、电流互感器变比。
 */
typedef struct {
	u16 Dat : 15; // 数据位 (15位)
	u16 S   : 1;  // 符号位 (0:正, 1:负)
} sDF06;

/**
 * @brief DF07: 无符号16位数据 (2字节 BCD?)
 * 常用于电压、温度等。实际是BCD还是二进制需根据上下文判断。
 */
typedef struct {
	u16 Dat; // 数据位 (16位)
} sDF07;

/**
 * @brief DF08: 无符号16位数据 (2字节 BCD?) - 与DF07相同?
 * DL/T645标准中DF08用途较少。
 */
typedef struct {
	u16 Dat; // 数据位 (16位)
} sDF08;

/**
 * @brief DF09: 带符号的23位数据 (3字节)
 * 常用于表示功率。结构体定义为3字节，最高位为符号位。
 */
typedef struct {
//	u32 Dat : 23; // 注释掉的位域定义
//	u32 S   : 1;
	u8 Dat0;    // 低字节
	u8 Dat1;    // 中字节
	u8 Dat2 : 7; // 高字节数据位 (7位)
	u8 S    : 1; // 符号位 (在高字节的最高位)
} sDF09;

/**
 * @brief DF10: 无符号24位数据 (3字节 BCD?)
 * 常用于表示电能常数。结构体定义为3字节。
 */
typedef struct {
//	u32 Dat : 24; // 注释掉的位域定义
	u8 Dat0; // 低字节
	u8 Dat1; // 中字节
	u8 Dat2; // 高字节
} sDF10;

/**
 * @brief DF11: 无符号32位数据 (4字节 BCD?)
 * 常用于表示电能量。
 */
typedef struct {
	u32 Dat; // 数据位 (32位)
} sDF11;

/**
 * @brief DF12: 6字节数据 (BCD?)
 * 常用于表示通信地址、表号。
 */
typedef struct {
	u8 Dat0; // 字节0 (最低位)
	u8 Dat1;
	u8 Dat2;
	u8 Dat3;
	u8 Dat4;
	u8 Dat5; // 字节5 (最高位)
} sDF12;

/**
 * @brief DF13: 无符号32位数据 (4字节 BCD?) - 与DF11相同?
 * DL/T645标准中DF13通常表示最大需量。
 */
typedef struct {
	u32 Dat; // 数据位 (32位)
} sDF13;

/**
 * @brief DF14: 5字节数据 (BCD?)
 * 常用于最大需量发生时间 (YYMMDDHHMM)。
 */
typedef struct {
	u8 Dat0; // 分 (MM)
	u8 Dat1; // 时 (HH)
	u8 Dat2; // 日 (DD)
	u8 Dat3; // 月 (MM)
	u8 Dat4; // 年 (YY)
} sDF14;

/**
 * @brief DF15: 日期和时间 (YYMMDDHHMM, 5字节 BCD)
 */
typedef struct {
	u8 Minute; // 分 (MM)
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
	u8 Month;  // 月 (MM)
	u8 Year;   // 年 (YY)
} sDF15;

/**
 * @brief DF16: 时间和日期 (DDHHMMSS, 4字节 BCD)
 */
typedef struct {
	u8 Second; // 秒 (SS)
	u8 Minute; // 分 (MM)
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
} sDF16;

/**
 * @brief DF17: 日期和时间 (MMDDHHMM, 4字节 BCD)
 */
typedef struct {
	u8 Minute; // 分 (MM)
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
	u8 Month;  // 月 (MM)
} sDF17;

/**
 * @brief DF18: 时间和日期 (DDHHMM, 3字节 BCD)
 */
typedef struct {
	u8 Minute; // 分 (MM)
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
} sDF18;

/**
 * @brief DF19: 时间 (HHMM, 2字节 BCD)
 */
typedef struct {
	u8 Minute; // 分 (MM)
	u8 Hour;   // 时 (HH)
} sDF19;

/**
 * @brief DF20: 日期 (YYMMDD, 3字节 BCD)
 */
typedef struct {
	u8 Day;    // 日 (DD)
	u8 Month;  // 月 (MM)
	u8 Year;   // 年 (YY)
} sDF20;

/**
 * @brief DF21: 日期 (YYMM, 2字节 BCD)
 */
typedef struct {
	u8 Month;  // 月 (MM)
	u8 Year;   // 年 (YY)
} sDF21;

/**
 * @brief DF22: 无符号8位数据 (1字节 BCD?)
 */
typedef struct {
	u8 Dat; // 数据位 (8位)
} sDF22;

/**
 * @brief DF23: 无符号24位数据 (3字节 BCD?) - 与DF10相同?
 * DL/T645标准中DF23通常表示费率数、时段数等。
 */
typedef struct {
	u8 Dat0; // 低字节
	u8 Dat1; // 中字节
	u8 Dat2; // 高字节
} sDF23;

/**
 * @brief DF24: 时间和日期 (DDHH, 2字节 BCD)
 */
typedef struct {
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
} sDF24;

/**
 * @brief DF25: 带符号的23位数据 (3字节) - 与DF09相同?
 * 常用于表示电流。
 */
typedef struct {
	u8 Dat0;    // 低字节
	u8 Dat1;    // 中字节
	u8 Dat2 : 7; // 高字节数据位 (7位)
	u8 S    : 1; // 符号位 (在高字节的最高位)
} sDF25;

/**
 * @brief DF26: 无符号16位数据 (2字节 BCD?) - 与DF07/DF08相同?
 * 常用于表示功率因数。
 */
typedef struct {
	u16 Dat; // 数据位 (16位)
} sDF26;

/**
 * @brief DF27: 无符号32位数据 (4字节 BCD?) - 与DF11/DF13相同?
 * 常用于表示累计工作时间等。
 */
typedef struct {
	u32 Dat; // 数据位 (32位)
} sDF27;

/**
 * @brief DF28: 日期和时间 (MMDDHH, 3字节 BCD)
 */
typedef struct {
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
	u8 Month;  // 月 (MM)
} sDF28;

/**
 * @brief DF29: 日期和时间 (YYMMDDHH, 4字节 BCD)
 */
typedef struct {
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
	u8 Month;  // 月 (MM)
	u8 Year;   // 年 (YY)
} sDF29;

//=== 以下为增加的基本数据格式 (非DL/T645标准?) ===
/**
 * @brief DF30: 无符号16位数据 (2字节 BCD?) - 与DF07/DF08/DF26相同?
 * 用于频率、电池电压等。
 */
typedef struct {
	u16 Dat; // 数据位 (16位)
} sDF30;

/**
 * @brief DF31: 无符号24位数据 (3字节 BCD?) - 与DF10/DF23相同?
 */
typedef struct {
	u8 Dat0; // 低字节
	u8 Dat1; // 中字节
	u8 Dat2; // 高字节
} sDF31;

/**
 * @brief DF32: 带符号的31位数据 (4字节)
 */
typedef struct {
	u32 Dat : 31; // 数据位 (31位)
	u32 S   : 1;  // 符号位
} sDF32;

/**
 * @brief DF33: 无符号16位数据 (2字节 BCD?) - 与DF07/DF08/DF26/DF30相同?
 */
typedef struct {
	u16 Dat; // 数据位 (16位)
} sDF33;

/**
 * @brief DF34: 日期和时间 (YYMMDDHHMMSS, 6字节 BCD) - 与DF01类似但无星期
 */
typedef struct {
	u8 Second; // 秒 (SS)
	u8 Minute; // 分 (MM)
	u8 Hour;   // 时 (HH)
	u8 Day;    // 日 (DD)
	u8 Month;  // 月 (MM)
	u8 Year;   // 年 (YY)
} sDF34;

// --- 以下结构体 (DF35-DF38) 和联合体 (u64) 可能是用于处理64位数据的自定义格式 ---
/**
 * @brief DF35: 64位数据，由两个32位组成？
 */
typedef struct {
	u32 Dat0; // 低32位
	u32 Dat1; // 高32位
} sDF35;

/**
 * @brief DF36: 64位数据，由16+32+16位组成？
 */
typedef struct {
	u16 Dat0; // 低16位
	u32 Dat1; // 中32位
	u16 Dat2; // 高16位
} sDF36;

/**
 * @brief DF37: 64位数据，由四个16位组成？
 */
typedef struct {
	u16 Dat0; // 最低16位
	u16 Dat1;
	u16 Dat2;
	u16 Dat3; // 最高16位
} sDF37;

/**
 * @brief DF38: 10字节数据 (BCD?)
 */
typedef struct {
	u8 Dat0;
	u8 Dat1;
	u8 Dat2;
	u8 Dat3;
	u8 Dat4;
	u8 Dat5;
	u8 Dat6;
	u8 Dat7;
	u8 Dat8;
	u8 Dat9;
} sDF38;

/**
 * @brief 用于访问64位数据的联合体。
 */
typedef union {
	sDF35	DF01; // 按两个32位访问
	sDF36	DF02; // 按16+32+16位访问
	sDF37	DF03; // 按四个16位访问
} u64;


//-----------------------
/**
 * @brief 包含所有已定义DF结构体的联合体。
 * 方便在不知道具体类型时存储或传递数据，或进行类型转换。
 */
typedef union {
	sDF01	DF01;
	sDF02	DF02;
	sDF03	DF03;
	sDF04	DF04;
	sDF05	DF05;
	sDF06	DF06;
	sDF07	DF07;
	sDF08	DF08;
	sDF09	DF09;
	sDF10	DF10;
	sDF11	DF11;
	sDF12	DF12;
	sDF13	DF13;
	sDF14	DF14;
	sDF15	DF15;
	sDF16	DF16;
	sDF17	DF17;
	sDF18	DF18;
	sDF19	DF19;
	sDF20	DF20;
	sDF21	DF21;
	sDF22	DF22;
	sDF23	DF23;
	sDF24	DF24;
	sDF25	DF25;
	sDF26	DF26;
	sDF27	DF27;
//	sDF28	DF28; // DF28 与 sDF28 重名，注释掉
//	sDF29	DF29; // DF29 与 sDF29 重名，注释掉
	sDF30	DF30;
	sDF31	DF31;
	sDF32	DF32;
	sDF33	DF33;
//	sDF34	DF34; // DF34 与 sDF34 重名，且与DF33类型相同? 用DF33代替
} uDFAll;

//=== 以下为基于基本DF格式的应用数据类型定义 ===

/**
 * @brief 分时电能量类型定义 (使用DF11: 32位无符号数)。
 */
typedef	sDF11 sFsEgSh_TypeDef;

/**
 * @brief 最大需量及其发生时间类型定义。
 */
typedef struct {
	sDF09	Need;     // 最大需量值 (DF09: 带符号23位)
	sDF15	NeedTime; // 发生时间 (DF15: YYMMDDHHMM)
} sFsNeed_TypeDef;

/**
 * @brief 时区表定义。
 * 用于定义每个时区的起始日期和使用的日时段表号。
 */
typedef struct {
	u8		nPOTTable; // 该时区使用的日时段表号
	u8		Day;       // 时区切换日 (BCD)
	u8		Month;     // 时区切换月 (BCD)
} sTimeZoneTable_TypeDef;

/**
 * @brief 日时段表定义。
 * 用于定义一天内各个时段的起始时间和对应的费率号。
 */
typedef struct {
	u8		Fl;     // 该时段对应的费率号
	u8 		Minute; // 时段起始分钟 (BCD)
	u8		Hour;   // 时段起始小时 (BCD)
} sPOTTable_TypeDef;

/**
 * @brief 节假日/特殊日时段表定义。
 */
typedef struct {
	u8		nPOTTable; // 该节假日使用的日时段表号
	u8		Day;       // 节假日日期 - 日 (BCD)
	u8		Month;     // 节假日日期 - 月 (BCD)
	u8		Year;      // 节假日日期 - 年 (BCD, 或用通配符)
} sHolidayTable_TypeDef;


#pragma pack() // 恢复默认的字节对齐方式


#endif	//__DL645_TYPE_H


