#include "metering.h"

/**
 * @file metering_example.c
 * @brief 电能计量模块使用示例
 * @details 本文件展示了如何使用电能计量模块的基本功能：
 *          - 初始化模块
 *          - 配置参数
 *          - 读取测量值
 *          - 校准功能
 */

/**
 * @brief 模拟延时函数
 * @details 这是一个简单的延时函数示例
 *          在实际应用中，应该使用系统的延时功能
 *          或者定时器来实现更精确的延时
 * 
 * @param ms 延时时间(毫秒)
 */
static void MyDelay(uint32_t ms) {
    // 在这里实现你的延时函数
    // 示例使用简单的循环延时
    // 实际应用中应该使用系统的延时功能
    for(uint32_t i = 0; i < ms * 1000; i++) {
        __NOP();  // 空操作指令，防止编译器优化掉循环
    }
}

/**
 * @brief 模拟参数保存函数
 * @details 这是一个将参数保存到存储器的示例
 *          在实际应用中，应该使用FLASH或EEPROM来保存
 * 
 * @param addr 存储地址
 * @param data 要保存的数据
 * @param len 数据长度
 */
static void MySaveParameter(uint32_t addr, uint8_t* data, uint16_t len) {
    // 在这里实现参数保存到FLASH或EEPROM的功能
    // 示例：
    // Flash_Write(addr, data, len);
}

/**
 * @brief 模拟参数加载函数
 * @details 这是一个从存储器加载参数的示例
 *          在实际应用中，应该从FLASH或EEPROM中读取
 * 
 * @param addr 存储地址
 * @param data 数据缓冲区
 * @param len 数据长度
 */
static void MyLoadParameter(uint32_t addr, uint8_t* data, uint16_t len) {
    // 在这里实现从FLASH或EEPROM加载参数的功能
    // 示例：
    // Flash_Read(addr, data, len);
}

// 全局变量
static Metering_Handle_TypeDef g_MeteringHandle;  // 计量模块句柄

/**
 * @brief 计量模块初始化示例
 * @details 展示如何初始化计量模块：
 *          1. 配置必要的回调函数
 *          2. 设置EMU寄存器基地址
 *          3. 调用初始化函数
 *          4. 检查初始化结果
 */
void Metering_Example_Init(void) {
    // 配置计量模块
    g_MeteringHandle.Config.EMU = (EMU_TypeDef*)0x40000000;  // 设置EMU寄存器基地址(需要根据实际硬件修改)
    g_MeteringHandle.Config.Delay = MyDelay;                  // 设置延时函数
    g_MeteringHandle.Config.SaveParameter = MySaveParameter;  // 设置参数保存函数
    g_MeteringHandle.Config.LoadParameter = MyLoadParameter;  // 设置参数加载函数
    
    // 初始化模块
    if (!Metering_Init(&g_MeteringHandle)) {
        // 初始化失败处理
        while(1) {
            // 可以在这里添加错误指示，如点亮错误LED等
        }
    }
}

/**
 * @brief 计量数据处理示例
 * @details 展示如何周期性处理计量数据：
 *          1. 调用处理函数
 *          2. 读取测量结果
 *          3. 处理测量数据
 */
void Metering_Example_Process(void) {
    // 调用计量处理函数
    Metering_Process(&g_MeteringHandle);
    
    // 读取测量结果
    float voltage = g_MeteringHandle.Data.U * g_MeteringHandle.Params.KUrms;          // 计算实际电压值
    float current = g_MeteringHandle.Data.Ia * g_MeteringHandle.Params.KIArms;        // 计算实际电流值
    float power_factor = g_MeteringHandle.Data.Pf / 1000.0f;                          // 计算实际功率因数
    float frequency = g_MeteringHandle.Data.Frequency / 100.0f;                        // 计算实际频率(Hz)
    float phase_angle = g_MeteringHandle.Data.Angle / 10.0f;                          // 计算实际相角(度)
    
    // 在这里可以添加数据处理代码
    // 例如：显示数据、保存数据、通信发送等
}

/**
 * @brief 校准示例
 * @details 展示如何进行电压和电流校准：
 *          1. 使用标准源输入已知值
 *          2. 调用校准函数
 *          3. 检查校准结果
 * 
 * @param voltage 标准电压值
 * @param current 标准电流值
 * @return true表示校准成功，false表示失败
 */
bool Metering_Example_Calibrate(float voltage, float current) {
    bool result = true;
    
    // 电压校准
    if (!Metering_Calibrate_Voltage(&g_MeteringHandle, voltage)) {
        result = false;
    }
    
    // 等待稳定
    MyDelay(1000);
    
    // 电流校准
    if (!Metering_Calibrate_Current(&g_MeteringHandle, current)) {
        result = false;
    }
    
    return result;
}

/**
 * @brief 主函数示例
 * @details 展示完整的使用流程：
 *          1. 初始化模块
 *          2. 进行必要的校准
 *          3. 循环处理数据
 */
int main(void) {
    // 系统初始化(根据实际硬件添加必要的初始化代码)
    
    // 初始化计量模块
    Metering_Example_Init();
    
    // 如果需要校准，可以在这里进行
    // Metering_Example_Calibrate(220.0f, 5.0f);
    
    // 主循环
    while(1) {
        // 处理计量数据
        Metering_Example_Process();
        
        // 可以添加其他任务处理代码
        
        // 适当延时
        MyDelay(100);  // 100ms的处理周期
    }
} 