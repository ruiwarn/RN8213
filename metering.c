#include "metering.h"
#include <stdio.h>

/**
 * @file metering.c
 * @brief 电能计量模块实现文件
 * @details 实现了电能计量模块的所有功能，包括：
 *          - 初始化
 *          - 数据采集和处理
 *          - 校准
 *          - 参数管理
 */

/**
 * @brief 内部常量定义
 * @{
 */
#define EMU_WRITE_PROTECT_KEY1    0xE5    // EMU写保护解除密钥1
#define EMU_WRITE_PROTECT_KEY2    0xDC    // EMU写保护恢复密钥2
#define EMU_STATUS_BUSY_MASK      0x01000000  // EMU状态寄存器忙标志掩码
#define EMU_STATUS_CHECKSUM_MASK  0x00FFFFFF  // EMU状态寄存器校验和掩码
#define EMU_POWER_DIRECTION_MASK  0x02000000  // EMU状态寄存器功率方向标志掩码
#define EMU_DATA_SIGN_MASK        0x00800000  // EMU数据寄存器符号位掩码
#define EMU_CURRENT_START_THRESH  0x00000150  // 电流启动阈值
#define EMU_UPDATE_PERIOD_MS      500        // 数据更新周期(毫秒)
/** @} */

/**
 * @brief 默认参数值
 * @details 当无法从存储器加载参数时，使用这些默认值
 */
static const Metering_Params_TypeDef DEFAULT_PARAMS = {
    .HFConst = 0,        // 默认高频常数
    .UGain = 0,          // 默认电压增益
    .IAGain = 0,         // 默认A相电流增益
    .IBGain = 0xBFFF,    // 默认B相电流增益
    .GPQA = 0,           // 默认A相功率增益
    .GPQB = 0,           // 默认B相功率增益
    .PhsA = 0,           // 默认A相相位校正
    .PhsB = 0,           // 默认B相相位校正
    .APOSA = 0,          // 默认A相小信号补偿
    .APOSB = 0,          // 默认B相小信号补偿
    .PStart = 0x60,      // 默认有功启动功率
    .QStart = 0x0120,    // 默认无功启动功率
    .IARMSOS = 0,        // 默认A相电流RMS偏移
    .IBRMSOS = 0,        // 默认B相电流RMS偏移
    .KUrms = 1.0f,       // 默认电压系数
    .KIArms = 1.0f,      // 默认A相电流系数
    .KIBrms = 1.0f,      // 默认B相电流系数
    .KPrms = 1.0f,       // 默认功率系数
    .ChkSum = 0          // 默认校验和
};

/**
 * @brief 内部函数声明
 * @{
 */
static void WriteProtectedRegister(EMU_TypeDef* EMU, volatile uint32_t* reg, uint32_t value);
static bool UpdateChecksum(Metering_Handle_TypeDef* handle);
static void ProcessMeasurement(Metering_Handle_TypeDef* handle);
/** @} */

/**
 * @brief 写入受保护的EMU寄存器
 * @details EMU的某些寄存器写入时需要特殊的保护机制
 *          写入前需要先写入解除保护密钥
 *          写入后需要恢复保护
 * 
 * @param EMU EMU寄存器基地址
 * @param reg 要写入的寄存器地址
 * @param value 要写入的值
 */
static void WriteProtectedRegister(EMU_TypeDef* EMU, volatile uint32_t* reg, uint32_t value) {
    EMU->SPCMD = EMU_WRITE_PROTECT_KEY1;  // 解除写保护
    *reg = value;                          // 写入数据
    EMU->SPCMD = EMU_WRITE_PROTECT_KEY2;  // 恢复写保护
}

/**
 * @brief 更新EMU校验和
 * @details 从EMU状态寄存器读取新的校验和并保存
 * 
 * @param handle 模块句柄指针
 * @return true表示更新成功，false表示失败
 */
static bool UpdateChecksum(Metering_Handle_TypeDef* handle) {
    EMU_TypeDef* EMU = handle->Config.EMU;
    
    handle->Config.Delay(1);  // 等待EMU稳定
    
    // 读取并更新校验和
    handle->Params.ChkSum = EMU->EMUStatus & EMU_STATUS_CHECKSUM_MASK;
    
    return true;
}

/**
 * @brief 处理测量数据
 * @details 从EMU寄存器读取原始数据，进行处理后更新到Data结构中
 *          处理过程包括：
 *          - 符号位检查
 *          - 量程转换
 *          - 阈值判断
 *          - 特殊值处理
 * 
 * @param handle 模块句柄指针
 */
static void ProcessMeasurement(Metering_Handle_TypeDef* handle) {
    EMU_TypeDef* EMU = handle->Config.EMU;
    
    #ifdef DEBUG_PROCESSING
    printf("  [DEBUG] ProcessMeasurement called\n");
    #endif
    
    // 读取并处理电流值
    uint32_t iarms = EMU->IARMS;  // 读取A相电流
    uint32_t ibrms = EMU->IBRMS;  // 读取B相电流
    uint32_t urms = EMU->URMS;    // 读取电压
    
    // 检查符号位并处理
    if (iarms & EMU_DATA_SIGN_MASK) iarms = 0;  // A相电流为负时清零
    if (ibrms & EMU_DATA_SIGN_MASK) ibrms = 0;  // B相电流为负时清零
    if (urms & EMU_DATA_SIGN_MASK) urms = 0;    // 电压为负时清零
    
    // 应用校准系数
    float kIArms = (handle->Params.KIArms != 0.0f) ? handle->Params.KIArms : 1.0f;
    float kIBrms = (handle->Params.KIBrms != 0.0f) ? handle->Params.KIBrms : 1.0f;
    float kUrms = (handle->Params.KUrms != 0.0f) ? handle->Params.KUrms : 1.0f;
    
    handle->Data.Ia = (uint32_t)(iarms / kIArms);  // 转换A相电流
    handle->Data.In = (uint32_t)(ibrms / kIBrms);  // 转换B相电流
    handle->Data.U = (uint16_t)(urms / (10 * kUrms));  // 转换电压
    
    // 调试信息（仅在测试时）
    #ifdef DEBUG_PROCESSING
    printf("  [DEBUG] Raw: Ia=%d, U=%d, Freq=%d\n", iarms, urms, EMU->Ufreq);
    printf("  [DEBUG] Coeffs: KIArms=%f, KUrms=%f\n", kIArms, kUrms);
    printf("  [DEBUG] Processed: Ia=%d, U=%d\n", handle->Data.Ia, handle->Data.U);
    #endif
    
    // 处理功率方向
    handle->Data.PDirect = (EMU->EMUStatus & EMU_POWER_DIRECTION_MASK) ? 1 : 0;
    
    // 处理功率因数
    uint32_t pf = handle->Data.Chnsel ? EMU->PFB : EMU->PFA;  // 根据通道选择读取功率因数
    pf &= 0x00FFFFFF;  // 清除高8位
    if (pf & 0x00800000) {  // 如果是负值
        pf = ((~pf) & 0x00FFFFFF) + 1;  // 转换为正值
    }
    handle->Data.Pf = (uint16_t)((float)pf / 8388.608f);  // 转换为0-1000范围
    
    // 处理相角
    uint16_t angle = handle->Data.Chnsel ? EMU->ANGLEB : EMU->ANGLEA;  // 根据通道选择读取相角
    handle->Data.Angle = (uint16_t)(angle * 3600 / 32768);  // 转换为0.1度单位
    
    // 小电流处理
    if (handle->Data.Ia < EMU_CURRENT_START_THRESH) {  // 如果A相电流小于启动阈值
        handle->Data.Ia = 0;  // 清零电流值
        handle->Data.Pf = 0x0999;  // 设置功率因数为0.999
    }
    if (handle->Data.In < EMU_CURRENT_START_THRESH) {  // 如果B相电流小于启动阈值
        handle->Data.In = 0;  // 清零电流值
    }
    
    // 处理频率
    if (EMU->Ufreq != 0) {
        handle->Data.Frequency = (uint16_t)(184320000 / (4 * EMU->Ufreq));  // 计算电网频率
    } else {
        handle->Data.Frequency = 5000; // 默认50.00Hz
    }
    
    // 添加电能量更新
    Metering_UpdateEnergy(handle);
}

/**
 * @brief 计算电能量数据校验和
 */
static uint32_t CalculateEnergyChecksum(Metering_EnergyStore_TypeDef* energy) {
    uint32_t* data = (uint32_t*)energy;
    uint32_t sum = 0;
    for (int i = 0; i < (sizeof(Metering_EnergyStore_TypeDef) / 4 - 1); i++) {
        sum += data[i];
    }
    return sum;
}

/**
 * @brief 保存电能量数据到非易失性存储器
 */
bool Metering_SaveEnergy(Metering_Handle_TypeDef* handle, bool force) {
    if (!handle || !handle->IsInitialized || !handle->Config.SaveParameter) {
        return false;
    }
    
    // 准备存储数据
    Metering_EnergyStore_TypeDef energyStore;
    energyStore.ForwardEnergy = handle->Data.Energy.Forward;
    energyStore.ReverseEnergy = handle->Data.Energy.Reverse;
    energyStore.LastSaveTime = 0; // 简化版本，不使用时间戳
    energyStore.ChkSum = CalculateEnergyChecksum(&energyStore);
    
    // 保存到存储器
    handle->Config.SaveParameter(
        handle->Config.EnergySaveAddr,
        (uint8_t*)&energyStore,
        sizeof(Metering_EnergyStore_TypeDef)
    );
    
    return true;
}

/**
 * @brief 从非易失性存储器加载电能量数据
 */
bool Metering_LoadEnergy(Metering_Handle_TypeDef* handle) {
    if (!handle || !handle->IsInitialized || !handle->Config.LoadParameter) {
        return false;
    }
    
    // 从存储器读取数据
    Metering_EnergyStore_TypeDef energyStore;
    handle->Config.LoadParameter(
        handle->Config.EnergySaveAddr,
        (uint8_t*)&energyStore,
        sizeof(Metering_EnergyStore_TypeDef)
    );
    
    // 验证校验和
    uint32_t checksum = CalculateEnergyChecksum(&energyStore);
    if (checksum != energyStore.ChkSum) {
        return false; // 校验和错误
    }
    
    // 更新电能量数据
    handle->Data.Energy.Forward = energyStore.ForwardEnergy;
    handle->Data.Energy.Reverse = energyStore.ReverseEnergy;
    
    return true;
}

bool Metering_Init(Metering_Handle_TypeDef* handle) {
    // 参数检查
    if (!handle || !handle->Config.EMU || !handle->Config.Delay || 
        !handle->Config.SaveParameter || !handle->Config.LoadParameter) {
        return false;
    }

    // 设置默认更新周期
    handle->UpdatePeriod = EMU_UPDATE_PERIOD_MS;
    handle->LastUpdateTime = 0;
    
    // 加载参数，如果失败则使用默认值
    if (!Metering_LoadParams(handle)) {
        handle->Params = DEFAULT_PARAMS;
    }
    
    // 写入所有校准参数到EMU寄存器
    EMU_TypeDef* EMU = handle->Config.EMU;
    WriteProtectedRegister(EMU, &EMU->HFConst, handle->Params.HFConst);
    WriteProtectedRegister(EMU, &EMU->UGAIN, handle->Params.UGain);
    WriteProtectedRegister(EMU, &EMU->IAGAIN, handle->Params.IAGain);
    WriteProtectedRegister(EMU, &EMU->IBGAIN, handle->Params.IBGain);
    WriteProtectedRegister(EMU, &EMU->GPQA, handle->Params.GPQA);
    WriteProtectedRegister(EMU, &EMU->GPQB, handle->Params.GPQB);
    WriteProtectedRegister(EMU, &EMU->PhsA, handle->Params.PhsA);
    WriteProtectedRegister(EMU, &EMU->PhsB, handle->Params.PhsB);
    WriteProtectedRegister(EMU, &EMU->APOSA, handle->Params.APOSA);
    WriteProtectedRegister(EMU, &EMU->APOSB, handle->Params.APOSB);
    WriteProtectedRegister(EMU, &EMU->PStart, handle->Params.PStart);
    WriteProtectedRegister(EMU, &EMU->QStart, handle->Params.QStart);
    WriteProtectedRegister(EMU, &EMU->IARMSOS, handle->Params.IARMSOS);
    WriteProtectedRegister(EMU, &EMU->IBRMSOS, handle->Params.IBRMSOS);
    
    // 更新校验和
    if (!UpdateChecksum(handle)) {
        return false;
    }
    
    // 初始化电能量相关参数
    handle->Data.Energy.Forward = 0;
    handle->Data.Energy.Reverse = 0;
    handle->Data.Energy.Pulse = 0;
    handle->Data.Energy.PulseConst = handle->Params.HFConst; // 使用高频常数作为脉冲常数
    if (handle->Data.Energy.PulseConst == 0) {
        handle->Data.Energy.PulseConst = 1600; // 默认脉冲常数 1600 imp/kWh
    }
    
    // 设置默认的电能量保存参数
    if (handle->Config.EnergySaveInterval == 0) {
        handle->Config.EnergySaveInterval = 3600000; // 默认1小时保存一次
    }
    
    // 尝试加载已保存的电能量数据
    if (!Metering_LoadEnergy(handle)) {
        // 加载失败，使用默认值
        handle->Data.Energy.Forward = 0;
        handle->Data.Energy.Reverse = 0;
        handle->Data.Energy.Pulse = 0;
    }
    
    handle->IsInitialized = true;
    return true;
}

void Metering_Process(Metering_Handle_TypeDef* handle) {
    #ifdef DEBUG_PROCESSING
    printf("  [DEBUG] Metering_Process called\n");
    #endif
    
    if (!handle || !handle->IsInitialized) {
        #ifdef DEBUG_PROCESSING
        printf("  [DEBUG] Handle not initialized\n");
        #endif
        return;
    }
    
    // 简化版本：直接处理数据，时间控制由外部调用者管理
    // 检查EMU状态 - bit 24为1表示不忙
    if (!(handle->Config.EMU->EMUStatus & EMU_STATUS_BUSY_MASK)) {
        #ifdef DEBUG_PROCESSING
        printf("  [DEBUG] EMU is busy\n");
        #endif
        return;
    }
    
    // 处理测量数据
    ProcessMeasurement(handle);
}

bool Metering_LoadDefaultParams(Metering_Handle_TypeDef* handle) {
    if (!handle) {
        return false;
    }
    
    handle->Params = DEFAULT_PARAMS;
    return Metering_SaveParams(handle);
}

bool Metering_SaveParams(Metering_Handle_TypeDef* handle) {
    if (!handle || !handle->Config.SaveParameter) {
        return false;
    }
    
    // 保存参数到外部存储
    handle->Config.SaveParameter(0, (uint8_t*)&handle->Params, sizeof(Metering_Params_TypeDef));
    return true;
}

bool Metering_LoadParams(Metering_Handle_TypeDef* handle) {
    if (!handle || !handle->Config.LoadParameter) {
        return false;
    }
    
    // 从外部存储加载参数
    handle->Config.LoadParameter(0, (uint8_t*)&handle->Params, sizeof(Metering_Params_TypeDef));
    return true;
}

bool Metering_Calibrate_Voltage(Metering_Handle_TypeDef* handle, float standard_voltage) {
    if (!handle || !handle->IsInitialized) {
        return false;
    }
    
    EMU_TypeDef* EMU = handle->Config.EMU;
    float measured = EMU->URMS;  // 读取当前测量值
    float error = (measured - standard_voltage) / standard_voltage;  // 计算相对误差
    float gain = -error / (1 + error);  // 计算增益调整值
    
    // 将增益转换为寄存器格式
    uint16_t gainValue;
    if (gain > 0) {
        gainValue = (uint16_t)(gain * 32768);  // 正增益值
    } else {
        gainValue = (uint16_t)(65535 + gain * 32768);  // 负增益值
    }
    
    // 更新参数并写入寄存器
    handle->Params.UGain = gainValue;
    WriteProtectedRegister(EMU, &EMU->UGAIN, gainValue);
    
    return UpdateChecksum(handle) && Metering_SaveParams(handle);
}

bool Metering_Calibrate_Current(Metering_Handle_TypeDef* handle, float standard_current) {
    if (!handle || !handle->IsInitialized) {
        return false;
    }
    
    EMU_TypeDef* EMU = handle->Config.EMU;
    float measured = EMU->IARMS;  // 读取当前测量值
    float error = (measured - standard_current) / standard_current;  // 计算相对误差
    float gain = -error / (1 + error);  // 计算增益调整值
    
    // 将增益转换为寄存器格式
    uint16_t gainValue;
    if (gain > 0) {
        gainValue = (uint16_t)(gain * 32768);  // 正增益值
    } else {
        gainValue = (uint16_t)(65535 + gain * 32768);  // 负增益值
    }
    
    // 更新参数并写入寄存器
    handle->Params.IAGain = gainValue;
    WriteProtectedRegister(EMU, &EMU->IAGAIN, gainValue);
    
    return UpdateChecksum(handle) && Metering_SaveParams(handle);
}

bool Metering_Calibrate_Power(Metering_Handle_TypeDef* handle, float standard_power) {
    if (!handle || !handle->IsInitialized) {
        return false;
    }
    
    EMU_TypeDef* EMU = handle->Config.EMU;
    float measured = EMU->PowerPA;  // 读取当前测量值
    float error = (measured - standard_power) / standard_power;  // 计算相对误差
    float gain = -error / (1 + error);  // 计算增益调整值
    
    // 将增益转换为寄存器格式
    uint16_t gainValue;
    if (gain > 0) {
        gainValue = (uint16_t)(gain * 32768);  // 正增益值
    } else {
        gainValue = (uint16_t)(65535 + gain * 32768);  // 负增益值
    }
    
    // 更新参数并写入寄存器
    handle->Params.GPQA = gainValue;
    WriteProtectedRegister(EMU, &EMU->GPQA, gainValue);
    
    return UpdateChecksum(handle) && Metering_SaveParams(handle);
}

bool Metering_Calibrate_Phase(Metering_Handle_TypeDef* handle, float phase_error) {
    if (!handle || !handle->IsInitialized) {
        return false;
    }
    
    // 计算相位调整值
    float phaseAdjust = (asin(-phase_error / 1.732)) * 100 * 57.29578;  // 转换为角度
    
    // 将相位值转换为寄存器格式
    uint16_t phaseValue;
    if (phaseAdjust > 0) {
        phaseValue = (uint16_t)(phaseAdjust);  // 正相位值
    } else {
        phaseValue = (uint16_t)(512 + phaseAdjust);  // 负相位值
    }
    
    // 更新参数并写入寄存器
    handle->Params.PhsA = phaseValue;
    WriteProtectedRegister(handle->Config.EMU, &handle->Config.EMU->PhsA, phaseValue);
    
    return UpdateChecksum(handle) && Metering_SaveParams(handle);
}

/**
 * @brief 更新电能量计数
 * @details 从EMU读取脉冲计数，根据功率方向更新正反向电能量
 */
void Metering_UpdateEnergy(Metering_Handle_TypeDef* handle) {
    if (!handle || !handle->IsInitialized) {
        return;
    }
    
    EMU_TypeDef* EMU = handle->Config.EMU;
    
    // 读取当前脉冲计数
    uint32_t currentPulse = EMU->EnergyP;
    
    // 计算新增脉冲数
    uint32_t deltaPulse = 0;
    if (currentPulse > handle->Data.Energy.Pulse) {
        deltaPulse = currentPulse - handle->Data.Energy.Pulse;
    } else if (currentPulse < handle->Data.Energy.Pulse) {
        // 脉冲计数器溢出处理
        deltaPulse = (0xFFFFFFFF - handle->Data.Energy.Pulse) + currentPulse + 1;
    }
    
    // 更新脉冲计数
    handle->Data.Energy.Pulse = currentPulse;
    
    // 根据功率方向累加电能量
    if (deltaPulse > 0) {
        if (handle->Data.PDirect == 0) { // 正向
            handle->Data.Energy.Forward += deltaPulse;
        } else { // 反向
            handle->Data.Energy.Reverse += deltaPulse;
        }
    }
    
    // 尝试保存电能量数据（非强制）
    Metering_SaveEnergy(handle, false);
}

/**
 * @brief 获取正向有功电能量
 * @details 将脉冲数转换为千瓦时
 */
float Metering_GetForwardEnergy(Metering_Handle_TypeDef* handle) {
    if (!handle || !handle->IsInitialized) {
        return 0.0f;
    }
    
    // 将脉冲数转换为电能量(kWh)
    return (float)handle->Data.Energy.Forward / handle->Data.Energy.PulseConst;
}

/**
 * @brief 获取反向有功电能量
 * @details 将脉冲数转换为千瓦时
 */
float Metering_GetReverseEnergy(Metering_Handle_TypeDef* handle) {
    if (!handle || !handle->IsInitialized) {
        return 0.0f;
    }
    
    // 将脉冲数转换为电能量(kWh)
    return (float)handle->Data.Energy.Reverse / handle->Data.Energy.PulseConst;
}

/**
 * @brief 清除电能量计数
 */
bool Metering_ClearEnergy(Metering_Handle_TypeDef* handle, uint8_t clear_type) {
    if (!handle || !handle->IsInitialized) {
        return false;
    }
    
    switch (clear_type) {
        case 0: // 清除全部
            handle->Data.Energy.Forward = 0;
            handle->Data.Energy.Reverse = 0;
            handle->Data.Energy.Pulse = 0;
            break;
        case 1: // 仅清除正向
            handle->Data.Energy.Forward = 0;
            break;
        case 2: // 仅清除反向
            handle->Data.Energy.Reverse = 0;
            break;
        default:
            return false;
    }
    
    // 强制保存清除后的数据
    return Metering_SaveEnergy(handle, true);
} 