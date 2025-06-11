#ifndef _METERING_H_
#define _METERING_H_

/**
 * @file metering.h
 * @brief 电能计量模块头文件
 * @details 这个文件定义了电能计量模块所需的所有数据结构和函数接口
 *          通过这些接口可以实现电压、电流、功率等电能参数的测量
 */

#include <stdint.h>  // 包含标准整型定义
#include <stdbool.h> // 包含布尔类型定义
#include <math.h>    // 包含数学函数

/**
 * @brief 计量芯片(EMU)寄存器映射结构体
 * @details 这个结构体映射了计量芯片的所有硬件寄存器
 *          每个成员变量对应芯片中的一个实际寄存器
 *          使用volatile关键字是因为这些寄存器的值可能被硬件异步改变
 */
typedef struct {
    volatile uint32_t EMUCON;    // EMU控制寄存器：用于配置EMU的基本工作模式
    volatile uint32_t EMUCON2;   // EMU控制寄存器2：用于配置EMU的高级功能
    volatile uint32_t HFConst;   // 高频常数寄存器：用于配置电能脉冲常数
    volatile uint32_t PStart;    // 有功功率启动阈值：低于此值的有功功率将被视为零
    volatile uint32_t QStart;    // 无功功率启动阈值：低于此值的无功功率将被视为零
    volatile uint32_t GPQA;      // A相功率增益：用于校准A相功率测量值
    volatile uint32_t GPQB;      // B相功率增益：用于校准B相功率测量值
    volatile uint32_t PhsA;      // A相相位校正：用于补偿A相电压电流相位误差
    volatile uint32_t PhsB;      // B相相位校正：用于补偿B相电压电流相位误差
    volatile uint32_t QPhsCal;   // 无功相位校正：用于补偿无功功率计算的相位误差
    volatile uint32_t APOSA;     // A相小信号补偿：用于改善小电流测量精度
    volatile uint32_t APOSB;     // B相小信号补偿：用于改善小电流测量精度
    volatile uint32_t IARMSOS;   // A相电流RMS偏移：用于补偿A相电流测量的零点误差
    volatile uint32_t IBRMSOS;   // B相电流RMS偏移：用于补偿B相电流测量的零点误差
    volatile uint32_t URMSOS;    // 电压RMS偏移：用于补偿电压测量的零点误差
    volatile uint32_t IAGAIN;    // A相电流增益：用于校准A相电流测量值
    volatile uint32_t IBGAIN;    // B相电流增益：用于校准B相电流测量值
    volatile uint32_t UGAIN;     // 电压增益：用于校准电压测量值
    volatile uint32_t IARMS;     // A相电流RMS值：存储A相电流有效值测量结果
    volatile uint32_t IBRMS;     // B相电流RMS值：存储B相电流有效值测量结果
    volatile uint32_t URMS;      // 电压RMS值：存储电压有效值测量结果
    volatile uint32_t Ufreq;     // 电压频率：存储电网频率测量结果
    volatile uint32_t PowerPA;   // A相有功功率：存储A相有功功率计算结果
    volatile uint32_t PowerPB;   // B相有功功率：存储B相有功功率计算结果
    volatile uint32_t EnergyP;   // 有功电能脉冲计数：累计有功电能量
    volatile uint32_t PFA;       // A相功率因数：存储A相功率因数计算结果
    volatile uint32_t PFB;       // B相功率因数：存储B相功率因数计算结果
    volatile uint32_t ANGLEA;    // A相角度：存储A相电压电流相角
    volatile uint32_t ANGLEB;    // B相角度：存储B相电压电流相角
    volatile uint32_t EMUStatus; // EMU状态：存储EMU工作状态和错误标志
    volatile uint32_t SPCMD;     // 特殊命令寄存器：用于执行特殊操作如写保护
} EMU_TypeDef;

/**
 * @brief 计量参数结构体
 * @details 存储所有校准参数和运行参数
 *          这些参数需要定期保存到非易失性存储器中
 */
typedef struct {
    uint16_t HFConst;   // 高频常数：决定电能脉冲输出频率
    uint16_t UGain;     // 电压增益：电压测量的校准系数
    uint16_t IAGain;    // A相电流增益：A相电流测量的校准系数
    uint16_t IBGain;    // B相电流增益：B相电流测量的校准系数
    uint16_t GPQA;      // A相功率增益：A相功率测量的校准系数
    uint16_t GPQB;      // B相功率增益：B相功率测量的校准系数
    uint16_t PhsA;      // A相相位校正：A相相位补偿值
    uint16_t PhsB;      // B相相位校正：B相相位补偿值
    uint16_t APOSA;     // A相小信号补偿：改善小电流测量精度
    uint16_t APOSB;     // B相小信号补偿：改善小电流测量精度
    uint16_t PStart;    // 有功启动功率：有功功率计量启动阈值
    uint16_t QStart;    // 无功启动功率：无功功率计量启动阈值
    uint16_t IARMSOS;   // A相电流RMS偏移：A相电流零点校准值
    uint16_t IBRMSOS;   // B相电流RMS偏移：B相电流零点校准值
    float KUrms;        // 电压系数：电压测量值到实际值的转换系数
    float KIArms;       // A相电流系数：A相电流测量值到实际值的转换系数
    float KIBrms;       // B相电流系数：B相电流测量值到实际值的转换系数
    float KPrms;        // 功率系数：功率测量值到实际值的转换系数
    uint32_t ChkSum;    // 校验和：用于验证参数的完整性
} Metering_Params_TypeDef;

/**
 * @brief 计量数据结构体
 * @details 存储所有测量结果
 *          这些数据会被定期更新
 */
typedef struct {
    uint32_t Ia;        // A相电流：当前A相电流值(单位由KIArms决定)
    uint32_t In;        // 零线电流：当前零线电流值(单位由KIBrms决定)
    uint16_t U;         // 电压：当前电压值(单位由KUrms决定)
    uint16_t Frequency; // 频率：当前电网频率(单位：0.01Hz)
    uint16_t Pf;        // 功率因数：当前功率因数值(0-1000对应0-1.000)
    uint16_t Angle;     // 相角：电压电流相角值(单位：0.1度)
    struct {
        uint8_t Dat0;   // 功率数据低字节
        uint8_t Dat1;   // 功率数据中字节
        uint8_t Dat2;   // 功率数据高字节
    } Pw;              // 功率数据：当前功率值(3字节格式)
    uint8_t PDirect;   // 功率方向：0表示正向，1表示反向
    uint8_t Chnsel;    // 通道选择：0表示A相，1表示B相
    
    // 添加电能量相关字段
    struct {
        uint32_t Forward;    // 正向有功电能量计数
        uint32_t Reverse;    // 反向有功电能量计数
        uint32_t Pulse;      // 当前脉冲计数
        uint32_t PulseConst; // 电能脉冲常数(imp/kWh)
    } Energy;
} Metering_Data_TypeDef;

/**
 * @brief 回调函数类型定义
 * @details 这些函数需要由使用者实现
 *          模块通过这些函数与外部系统交互
 */
typedef void (*Delay_Func)(uint32_t ms);  // 延时函数：实现毫秒级延时
typedef void (*SaveParam_Func)(uint32_t addr, uint8_t* data, uint16_t len);  // 保存参数函数：将参数保存到非易失性存储器
typedef void (*LoadParam_Func)(uint32_t addr, uint8_t* data, uint16_t len);  // 加载参数函数：从非易失性存储器读取参数

/**
 * @brief 电能量存储数据结构
 * @details 用于在非易失性存储器中保存的电能量数据
 */
typedef struct {
    uint32_t ForwardEnergy;    // 正向有功电能量脉冲数
    uint32_t ReverseEnergy;    // 反向有功电能量脉冲数
    uint32_t LastSaveTime;     // 上次保存时间戳
    uint32_t ChkSum;           // 校验和
} Metering_EnergyStore_TypeDef;

/**
 * @brief 计量模块配置结构体
 * @details 包含模块运行所需的所有配置信息
 */
typedef struct {
    EMU_TypeDef* EMU;              // EMU寄存器基地址：指向计量芯片寄存器映射的指针
    Delay_Func Delay;              // 延时函数：用于实现时序要求
    SaveParam_Func SaveParameter;   // 保存参数函数：用于保存校准参数
    LoadParam_Func LoadParameter;   // 加载参数函数：用于读取校准参数
    uint32_t EnergySaveInterval;   // 电能量数据保存间隔(ms)
    uint32_t EnergySaveAddr;       // 电能量数据在存储器中的地址
} Metering_Config_TypeDef;

/**
 * @brief 计量模块句柄
 * @details 这是模块的主要数据结构，包含了运行所需的所有信息
 */
typedef struct {
    Metering_Config_TypeDef Config;     // 模块配置：包含所有配置信息
    Metering_Params_TypeDef Params;     // 计量参数：包含所有校准参数
    Metering_Data_TypeDef Data;         // 计量数据：包含所有测量结果
    uint32_t UpdatePeriod;              // 更新周期：数据更新的时间间隔(ms)
    uint32_t LastUpdateTime;            // 上次更新时间：用于控制更新频率
    bool IsInitialized;                 // 初始化标志：指示模块是否已经初始化
} Metering_Handle_TypeDef;

/**
 * @brief 初始化计量模块
 * @param handle 模块句柄指针
 * @return true表示初始化成功，false表示失败
 * @note 使用模块前必须先调用此函数进行初始化
 */
bool Metering_Init(Metering_Handle_TypeDef* handle);

/**
 * @brief 执行计量处理
 * @param handle 模块句柄指针
 * @note 此函数应该在主循环中周期性调用
 */
void Metering_Process(Metering_Handle_TypeDef* handle);

/**
 * @brief 电压校准函数
 * @param handle 模块句柄指针
 * @param standard_voltage 标准电压值
 * @return true表示校准成功，false表示失败
 */
bool Metering_Calibrate_Voltage(Metering_Handle_TypeDef* handle, float standard_voltage);

/**
 * @brief 电流校准函数
 * @param handle 模块句柄指针
 * @param standard_current 标准电流值
 * @return true表示校准成功，false表示失败
 */
bool Metering_Calibrate_Current(Metering_Handle_TypeDef* handle, float standard_current);

/**
 * @brief 功率校准函数
 * @param handle 模块句柄指针
 * @param standard_power 标准功率值
 * @return true表示校准成功，false表示失败
 */
bool Metering_Calibrate_Power(Metering_Handle_TypeDef* handle, float standard_power);

/**
 * @brief 相位校准函数
 * @param handle 模块句柄指针
 * @param phase_error 相位误差值(度)
 * @return true表示校准成功，false表示失败
 */
bool Metering_Calibrate_Phase(Metering_Handle_TypeDef* handle, float phase_error);

/**
 * @brief 加载默认参数
 * @param handle 模块句柄指针
 * @return true表示加载成功，false表示失败
 */
bool Metering_LoadDefaultParams(Metering_Handle_TypeDef* handle);

/**
 * @brief 保存当前参数
 * @param handle 模块句柄指针
 * @return true表示保存成功，false表示失败
 */
bool Metering_SaveParams(Metering_Handle_TypeDef* handle);

/**
 * @brief 加载已保存的参数
 * @param handle 模块句柄指针
 * @return true表示加载成功，false表示失败
 */
bool Metering_LoadParams(Metering_Handle_TypeDef* handle);

/**
 * @brief 更新电能量计数
 * @param handle 模块句柄指针
 * @note 此函数会根据EMU的脉冲输出更新电能量计数
 */
void Metering_UpdateEnergy(Metering_Handle_TypeDef* handle);

/**
 * @brief 获取正向有功电能量
 * @param handle 模块句柄指针
 * @return 正向有功电能量值(单位：kWh)
 */
float Metering_GetForwardEnergy(Metering_Handle_TypeDef* handle);

/**
 * @brief 获取反向有功电能量
 * @param handle 模块句柄指针
 * @return 反向有功电能量值(单位：kWh)
 */
float Metering_GetReverseEnergy(Metering_Handle_TypeDef* handle);

/**
 * @brief 清除电能量计数
 * @param handle 模块句柄指针
 * @param clear_type 清除类型：0-全部清除，1-仅清除正向，2-仅清除反向
 * @return true表示清除成功，false表示失败
 */
bool Metering_ClearEnergy(Metering_Handle_TypeDef* handle, uint8_t clear_type);

/**
 * @brief 保存电能量数据到非易失性存储器
 * @param handle 模块句柄指针
 * @param force 是否强制保存，true则忽略保存间隔直接保存
 * @return true表示保存成功，false表示失败
 */
bool Metering_SaveEnergy(Metering_Handle_TypeDef* handle, bool force);

/**
 * @brief 从非易失性存储器加载电能量数据
 * @param handle 模块句柄指针
 * @return true表示加载成功，false表示失败
 */
bool Metering_LoadEnergy(Metering_Handle_TypeDef* handle);

#endif // _METERING_H_ 