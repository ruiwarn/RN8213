#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "metering.h"

/**
 * @brief 模拟EMU硬件寄存器
 */
static EMU_TypeDef mock_EMU;

/**
 * @brief 模拟存储器
 */
static uint8_t mock_storage[1024];

/**
 * @brief 模拟系统时间
 */
static uint32_t mock_system_time = 0;

/**
 * @brief 测试结果统计
 */
typedef struct {
    int total;
    int passed;
    int failed;
} TestResult;

static TestResult test_result = {0, 0, 0};

/**
 * @brief 模拟函数实现
 */
void mock_delay(uint32_t ms) {
    mock_system_time += ms;
    printf("  [DELAY] %d ms, current time: %d\n", ms, mock_system_time);
}

void mock_save_parameter(uint32_t addr, uint8_t* data, uint16_t len) {
    if (addr + len <= sizeof(mock_storage)) {
        memcpy(&mock_storage[addr], data, len);
        printf("  [SAVE] Saved %d bytes at address 0x%04X\n", len, addr);
    } else {
        printf("  [SAVE] ERROR: Address out of range\n");
    }
}

void mock_load_parameter(uint32_t addr, uint8_t* data, uint16_t len) {
    if (addr + len <= sizeof(mock_storage)) {
        memcpy(data, &mock_storage[addr], len);
        printf("  [LOAD] Loaded %d bytes from address 0x%04X\n", len, addr);
    } else {
        printf("  [LOAD] ERROR: Address out of range\n");
    }
}

uint32_t mock_get_time(void) {
    return mock_system_time;
}

/**
 * @brief 初始化模拟环境
 */
void mock_init(void) {
    memset(&mock_EMU, 0, sizeof(EMU_TypeDef));
    memset(mock_storage, 0, sizeof(mock_storage));
    mock_system_time = 0;
    
    // 设置默认EMU值（根据RN8213实际值范围）
    mock_EMU.IARMS = 10000;     // 模拟1A电流（实际寄存器值）
    mock_EMU.IBRMS = 5000;      // 模拟0.5A电流
    mock_EMU.URMS = 2200;       // 模拟220V电压（实际寄存器值）
    mock_EMU.PowerPA = 22000;   // 模拟220W功率
    mock_EMU.PowerPB = 11000;   // 模拟110W功率
    mock_EMU.EnergyP = 100;     // 模拟100个脉冲
    mock_EMU.Ufreq = 92160;     // 模拟50Hz频率（184320000/(4*50)=921600）
    mock_EMU.PFA = 8000000;     // 模拟功率因数
    mock_EMU.PFB = 8000000;     
    mock_EMU.ANGLEA = 16384;    // 模拟相角
    mock_EMU.ANGLEB = 16384;
    mock_EMU.EMUStatus = 0x01000000; // 设置为不忙状态
}

/**
 * @brief 测试宏定义
 */
#define TEST_START(name) \
    do { \
        printf("\n=== Test: %s ===\n", name); \
        test_result.total++; \
    } while(0)

#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            printf("  [PASS] %s\n", message); \
        } else { \
            printf("  [FAIL] %s\n", message); \
            test_result.failed++; \
            return false; \
        } \
    } while(0)

#define TEST_END() \
    do { \
        test_result.passed++; \
        printf("  [SUCCESS] Test completed\n"); \
        return true; \
    } while(0)

/**
 * @brief 测试用例1：初始化测试
 */
bool test_initialization(void) {
    TEST_START("Initialization");
    
    Metering_Handle_TypeDef handle;
    memset(&handle, 0, sizeof(handle));
    
    // 配置测试参数
    handle.Config.EMU = &mock_EMU;
    handle.Config.Delay = mock_delay;
    handle.Config.SaveParameter = mock_save_parameter;
    handle.Config.LoadParameter = mock_load_parameter;
    handle.Config.EnergySaveInterval = 1000;
    handle.Config.EnergySaveAddr = 0x100;
    
    // 测试初始化
    bool result = Metering_Init(&handle);
    TEST_ASSERT(result == true, "Metering_Init should return true");
    TEST_ASSERT(handle.IsInitialized == true, "Handle should be marked as initialized");
    TEST_ASSERT(handle.Data.Energy.PulseConst != 0, "Pulse constant should be set");
    
    TEST_END();
}

/**
 * @brief 测试用例2：数据处理测试
 */
bool test_data_processing(void) {
    TEST_START("Data Processing");
    
    Metering_Handle_TypeDef handle;
    memset(&handle, 0, sizeof(handle));
    
    // 初始化
    handle.Config.EMU = &mock_EMU;
    handle.Config.Delay = mock_delay;
    handle.Config.SaveParameter = mock_save_parameter;
    handle.Config.LoadParameter = mock_load_parameter;
    handle.Config.EnergySaveInterval = 1000;
    handle.Config.EnergySaveAddr = 0x100;
    
    Metering_Init(&handle);
    
    // 模拟时间更新
    handle.UpdatePeriod = 500;
    handle.LastUpdateTime = 0;
    mock_system_time = 600; // 超过更新周期
    
    // 处理数据
    Metering_Process(&handle);
    
    // 检查数据是否已更新
    printf("  Current Ia: %d (raw: %d)\n", handle.Data.Ia, mock_EMU.IARMS);
    printf("  Voltage U: %d (raw: %d)\n", handle.Data.U, mock_EMU.URMS);
    printf("  Frequency: %d (raw: %d)\n", handle.Data.Frequency, mock_EMU.Ufreq);
    
    TEST_ASSERT(handle.Data.Ia > 0, "Current should be processed");
    TEST_ASSERT(handle.Data.U > 0, "Voltage should be processed");
    TEST_ASSERT(handle.Data.Frequency > 0, "Frequency should be calculated");
    
    TEST_END();
}

/**
 * @brief 测试用例3：电能量计算测试
 */
bool test_energy_calculation(void) {
    TEST_START("Energy Calculation");
    
    Metering_Handle_TypeDef handle;
    memset(&handle, 0, sizeof(handle));
    
    // 初始化
    handle.Config.EMU = &mock_EMU;
    handle.Config.Delay = mock_delay;
    handle.Config.SaveParameter = mock_save_parameter;
    handle.Config.LoadParameter = mock_load_parameter;
    handle.Config.EnergySaveInterval = 1000;
    handle.Config.EnergySaveAddr = 0x100;
    
    Metering_Init(&handle);
    
    // 设置初始脉冲数
    handle.Data.Energy.Pulse = 50;
    mock_EMU.EnergyP = 150; // 增加100个脉冲
    handle.Data.PDirect = 0; // 正向功率
    
    // 更新电能量
    Metering_UpdateEnergy(&handle);
    
    TEST_ASSERT(handle.Data.Energy.Forward == 100, "Forward energy should be 100 pulses");
    TEST_ASSERT(handle.Data.Energy.Reverse == 0, "Reverse energy should be 0");
    
    printf("  Forward Energy: %d pulses\n", handle.Data.Energy.Forward);
    printf("  Reverse Energy: %d pulses\n", handle.Data.Energy.Reverse);
    
    // 测试反向功率
    handle.Data.PDirect = 1; // 反向功率
    mock_EMU.EnergyP = 200;  // 再增加50个脉冲
    Metering_UpdateEnergy(&handle);
    
    TEST_ASSERT(handle.Data.Energy.Forward == 100, "Forward energy should remain 100");
    TEST_ASSERT(handle.Data.Energy.Reverse == 50, "Reverse energy should be 50");
    
    printf("  After reverse power - Forward: %d, Reverse: %d\n", 
           handle.Data.Energy.Forward, handle.Data.Energy.Reverse);
    
    TEST_END();
}

/**
 * @brief 测试用例4：存储功能测试
 */
bool test_energy_storage(void) {
    TEST_START("Energy Storage");
    
    Metering_Handle_TypeDef handle1, handle2;
    memset(&handle1, 0, sizeof(handle1));
    memset(&handle2, 0, sizeof(handle2));
    
    // 初始化第一个句柄
    handle1.Config.EMU = &mock_EMU;
    handle1.Config.Delay = mock_delay;
    handle1.Config.SaveParameter = mock_save_parameter;
    handle1.Config.LoadParameter = mock_load_parameter;
    handle1.Config.EnergySaveInterval = 1000;
    handle1.Config.EnergySaveAddr = 0x200;
    
    Metering_Init(&handle1);
    
    // 设置一些电能量数据
    handle1.Data.Energy.Forward = 12345;
    handle1.Data.Energy.Reverse = 6789;
    
    // 保存数据
    bool save_result = Metering_SaveEnergy(&handle1, true);
    TEST_ASSERT(save_result == true, "Energy save should succeed");
    
    // 用第二个句柄加载数据
    handle2.Config = handle1.Config;
    Metering_Init(&handle2);
    
    bool load_result = Metering_LoadEnergy(&handle2);
    TEST_ASSERT(load_result == true, "Energy load should succeed");
    TEST_ASSERT(handle2.Data.Energy.Forward == 12345, "Forward energy should match saved value");
    TEST_ASSERT(handle2.Data.Energy.Reverse == 6789, "Reverse energy should match saved value");
    
    printf("  Saved Forward: %d, Loaded Forward: %d\n", 
           handle1.Data.Energy.Forward, handle2.Data.Energy.Forward);
    printf("  Saved Reverse: %d, Loaded Reverse: %d\n", 
           handle1.Data.Energy.Reverse, handle2.Data.Energy.Reverse);
    
    TEST_END();
}

/**
 * @brief 测试用例5：校准功能测试
 */
bool test_calibration(void) {
    TEST_START("Calibration Functions");
    
    Metering_Handle_TypeDef handle;
    memset(&handle, 0, sizeof(handle));
    
    // 初始化
    handle.Config.EMU = &mock_EMU;
    handle.Config.Delay = mock_delay;
    handle.Config.SaveParameter = mock_save_parameter;
    handle.Config.LoadParameter = mock_load_parameter;
    handle.Config.EnergySaveInterval = 1000;
    handle.Config.EnergySaveAddr = 0x300;
    
    Metering_Init(&handle);
    
    uint16_t old_ugain = handle.Params.UGain;
    
    // 测试电压校准
    bool cal_result = Metering_Calibrate_Voltage(&handle, 220.0f);
    TEST_ASSERT(cal_result == true, "Voltage calibration should succeed");
    
    // 检查增益是否已更新
    bool gain_updated = (handle.Params.UGain != old_ugain);
    TEST_ASSERT(gain_updated, "Voltage gain should be updated");
    
    printf("  Old UGain: 0x%04X, New UGain: 0x%04X\n", old_ugain, handle.Params.UGain);
    
    // 测试电流校准
    uint16_t old_iagain = handle.Params.IAGain;
    cal_result = Metering_Calibrate_Current(&handle, 1.0f);
    TEST_ASSERT(cal_result == true, "Current calibration should succeed");
    
    gain_updated = (handle.Params.IAGain != old_iagain);
    TEST_ASSERT(gain_updated, "Current gain should be updated");
    
    printf("  Old IAGain: 0x%04X, New IAGain: 0x%04X\n", old_iagain, handle.Params.IAGain);
    
    TEST_END();
}

/**
 * @brief 测试用例6：边界条件测试
 */
bool test_boundary_conditions(void) {
    TEST_START("Boundary Conditions");
    
    Metering_Handle_TypeDef handle;
    memset(&handle, 0, sizeof(handle));
    
    // 测试空指针
    bool result = Metering_Init(NULL);
    TEST_ASSERT(result == false, "Init with NULL should fail");
    
    // 测试未初始化的处理
    Metering_Process(&handle);
    // 应该不会崩溃
    
    // 正确初始化
    handle.Config.EMU = &mock_EMU;
    handle.Config.Delay = mock_delay;
    handle.Config.SaveParameter = mock_save_parameter;
    handle.Config.LoadParameter = mock_load_parameter;
    handle.Config.EnergySaveInterval = 1000;
    handle.Config.EnergySaveAddr = 0x400;
    
    Metering_Init(&handle);
    
    // 测试脉冲溢出情况
    handle.Data.Energy.Pulse = 0xFFFFFFFE;
    mock_EMU.EnergyP = 5; // 模拟溢出
    Metering_UpdateEnergy(&handle);
    
    // 应该正确处理溢出
    TEST_ASSERT(handle.Data.Energy.Forward > 0, "Overflow should be handled correctly");
    
    printf("  Overflow handling: Forward energy = %d\n", handle.Data.Energy.Forward);
    
    // 测试清除功能
    result = Metering_ClearEnergy(&handle, 0);
    TEST_ASSERT(result == true, "Clear energy should succeed");
    TEST_ASSERT(handle.Data.Energy.Forward == 0, "Forward energy should be cleared");
    TEST_ASSERT(handle.Data.Energy.Reverse == 0, "Reverse energy should be cleared");
    
    TEST_END();
}

/**
 * @brief 运行所有测试
 */
void run_all_tests(void) {
    printf("Starting Metering Framework Test Suite\n");
    printf("=====================================\n");
    
    // 初始化模拟环境
    mock_init();
    
    // 运行测试用例
    test_initialization();
    test_data_processing();
    test_energy_calculation();
    test_energy_storage();
    test_calibration();
    test_boundary_conditions();
    
    // 打印测试结果
    printf("\n=====================================\n");
    printf("Test Results:\n");
    printf("  Total:  %d\n", test_result.total);
    printf("  Passed: %d\n", test_result.passed);
    printf("  Failed: %d\n", test_result.failed);
    
    if (test_result.failed == 0) {
        printf("  Status: ALL TESTS PASSED ✓\n");
    } else {
        printf("  Status: %d TESTS FAILED ✗\n", test_result.failed);
    }
    printf("=====================================\n");
}

/**
 * @brief 主函数
 */
int main(void) {
    run_all_tests();
    return (test_result.failed == 0) ? 0 : 1;
} 