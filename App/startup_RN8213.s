;/*****************************************************************************
; * @file:    startup_RN8213.s
; * @purpose: CMSIS Cortex-M0 Core Device Startup File for RN8213
; *           锐能微RN8213芯片的CMSIS Cortex-M0内核启动文件
; * @version: V1.00
; * @date:    2012/12/3
; * @author:  [Original author unknown, comments added by Roo]
; * @description: 此文件负责芯片上电或复位后的初始化序列，包括设置堆栈指针、
; *               定义中断向量表、执行复位处理程序以及提供默认的中断服务程序桩。
; *****************************************************************************/


;------------------ 堆栈配置 (Stack Configuration) ------------------
; <h> Stack Configuration ; Keil MDK 配置向导注释
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8> ; 配置堆栈大小（字节）
; </h>
Stack_Size      EQU     0x00000600      ; 定义堆栈大小为 0x600 字节 (1536字节)
UNUSEDIRQ       EQU     0               ; 未使用的IRQ标志？ (未使用)
UNUSEDUART      EQU     0               ; 未使用的UART标志？ (未使用)

                AREA    STACK, NOINIT, READWRITE, ALIGN=3 ; 定义一个名为 STACK 的段，不初始化，可读写，8字节对齐
Stack_Mem       SPACE   Stack_Size      ; 在STACK段中分配 Stack_Size 大小的空间
__initial_sp                            ; 定义标号 __initial_sp，表示堆栈顶部地址 (栈底高地址)


;------------------ 堆配置 (Heap Configuration) ------------------
; <h> Heap Configuration ; Keil MDK 配置向导注释
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8> ; 配置堆大小（字节）
; </h>
Heap_Size       EQU     0x00000200      ; 定义堆大小为 0x200 字节 (512字节)

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3 ; 定义一个名为 HEAP 的段，不初始化，可读写，8字节对齐
__heap_base                             ; 定义标号 __heap_base，表示堆起始地址
Heap_Mem        SPACE   Heap_Size       ; 在HEAP段中分配 Heap_Size 大小的空间
__heap_limit                            ; 定义标号 __heap_limit，表示堆结束地址


;------------------ 代码段设置 ------------------
                PRESERVE8               ; 指示当前文件保持8字节栈对齐
                THUMB                   ; 指示后续指令为Thumb指令集


;------------------ 中断向量表 (Vector Table) ------------------
; 中断向量表在复位后映射到地址0x00000000
                        
                AREA    RESET, DATA, READONLY ; 定义一个名为 RESET 的段，包含数据，只读
                EXPORT  __Vectors           ; 导出向量表起始地址标号 __Vectors
                EXPORT  __Vectors_End       ; 导出向量表结束地址标号 __Vectors_End
                EXPORT  __Vectors_Size      ; 导出向量表大小标号 __Vectors_Size

__Vectors       DCD     __initial_sp        ; 0x00: 堆栈指针初始值 (MSP)
                DCD     Reset_Handler       ; 0x04: 复位中断处理程序地址
                DCD     NMI_Handler         ; 0x08: NMI (不可屏蔽中断) 处理程序地址
                DCD     HardFault_Handler   ; 0x0C: 硬故障处理程序地址
                DCD     0                   ; 0x10: 保留
                DCD     0                   ; 0x14: 保留
                DCD     0                   ; 0x18: 保留
                DCD     0                   ; 0x1C: 保留
                DCD     0                   ; 0x20: 保留
                DCD     0                   ; 0x24: 保留
                DCD     0                   ; 0x28: 保留
                DCD     SVC_Handler         ; 0x2C: SVCall (系统服务调用) 处理程序地址
                DCD     0                   ; 0x30: 保留
                DCD     0                   ; 0x34: 保留
                DCD     PendSV_Handler      ; 0x38: PendSV (可挂起系统调用) 处理程序地址
                DCD     SysTick_Handler     ; 0x3C: SysTick (系统嘀嗒定时器) 处理程序地址
                
                ; -- 外部中断向量 (IRQ Handler Addresses) --
                ; 这些地址对应于芯片特定的外设中断源。
                ; 0 表示该中断未使用或未定义处理程序。
                ; 具体中断源请参考RN8213数据手册的中断向量表部分。
                DCD     0                   ; IRQ 0? (保留)
                DCD     CMP_HANDLER         ; IRQ 1: 比较器中断
                DCD     VCH_HANDLER         ; IRQ 2: 电压通道相关中断？
                DCD     RTC_HANDLER         ; IRQ 3: RTC (实时时钟) 中断
                DCD     EMU_HANDLER         ; IRQ 4: EMU (计量单元) 中断
                DCD     MADC_HANDLER        ; IRQ 5: MADC (测量ADC) 中断
                DCD     UART0_HANDLER       ; IRQ 6: UART0 中断
                DCD     UART1_HANDLER       ; IRQ 7: UART1 中断
                DCD     UART2_HANDLER       ; IRQ 8: UART2 中断
                DCD     UART3_HANDLER       ; IRQ 9: UART3 中断
                DCD     SPI_HANDLER         ; IRQ 10: SPI 中断
                DCD     I2C_HANDLER         ; IRQ 11: I2C 中断
                DCD     ISO78160_HANDLER    ; IRQ 12: ISO7816 接口 0 中断
                DCD     ISO78161_HANDLER    ; IRQ 13: ISO7816 接口 1 中断
                DCD     TC0_HANDLER         ; IRQ 14: 定时器/计数器 0 中断
                DCD     TC1_HANDLER         ; IRQ 15: 定时器/计数器 1 中断
                DCD     0                   ; IRQ 16? (保留)
                DCD     UART5_HANDLER       ; IRQ 17: UART5 中断
                DCD     WDT_HANDLER         ; IRQ 18: WDT (看门狗) 中断
                DCD     KBI_HANDLER         ; IRQ 19: KBI (键盘) 中断
                DCD     LCD_HANDLER         ; IRQ 20: LCD 控制器中断
                DCD     0                   ; IRQ 21? (保留)
                DCD     0                   ; IRQ 22? (保留)
                DCD     0                   ; IRQ 23? (保留)
                DCD     EXT0_HANDLER        ; IRQ 24: 外部中断 0
                DCD     0                   ; IRQ 25? (保留)
                DCD     EXT2_HANDLER        ; IRQ 26: 外部中断 2
                DCD     0                   ; IRQ 27? (保留)
                DCD     0                   ; IRQ 28? (保留)
                DCD     0                   ; IRQ 29? (保留)
                DCD     EXT6_HANDLER        ; IRQ 30: 外部中断 6
                DCD     EXT7_HANDLER        ; IRQ 31: 外部中断 7
                
__Vectors_End                           ; 向量表结束地址标号

__Vectors_Size  EQU  __Vectors_End - __Vectors ; 计算向量表大小


;------------------ 代码段定义 ------------------
                AREA    |.text|, CODE, READONLY ; 定义一个名为 .text 的段，包含代码，只读


;------------------ 复位处理程序 (Reset Handler) ------------------
; 这是芯片上电或复位后执行的第一个程序。
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK] ; 导出 Reset_Handler 标号，弱定义，允许用户覆盖
                IMPORT  SystemInit                ; 导入外部 C 函数 SystemInit (通常在 system_RN8213.c 中)
                IMPORT  __main                    ; 导入 C 库入口函数 __main
                LDR     R0, =SystemInit           ; 将 SystemInit 函数地址加载到 R0
                BLX     R0                        ; 调用 SystemInit 函数 (进行时钟等基本初始化)
                LDR     R0, =__main               ; 将 C 库入口函数 __main 地址加载到 R0
                BX      R0                        ; 跳转到 __main (初始化 C 运行时环境，然后调用 main 函数)
                ENDP                            ; 过程结束


;------------------ 默认异常/中断处理程序桩 (Dummy Handlers) ------------------
; 这些是向量表中引用的中断服务程序的默认实现。
; 它们通常是无限循环，表示该中断未被应用程序处理。
; 用户应在自己的代码中提供实际的中断服务程序来覆盖这些弱定义。

;                IF UNUSEDIRQ <> 1 ; Keil MDK 条件编译 (未使用)
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK] ; 导出标号，弱定义
                B       .                         ; 无限循环 (跳转到当前指令)
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP
EXT0_HANDLER    PROC
                EXPORT  EXT0_HANDLER			  [WEAK]
                B       .
                ENDP
EXT2_HANDLER    PROC
                EXPORT  EXT2_HANDLER			  [WEAK]
                B       .
                ENDP
EXT6_HANDLER    PROC
                EXPORT  EXT6_HANDLER			  [WEAK]
                B       .
                ENDP
EXT7_HANDLER    PROC
                EXPORT  EXT7_HANDLER			  [WEAK]
                B       .
                ENDP
RTC_HANDLER     PROC
                EXPORT  RTC_HANDLER				  [WEAK]
                B       .
                ENDP
EMU_HANDLER     PROC
                EXPORT  EMU_HANDLER				  [WEAK]
                B       .
                ENDP
MADC_HANDLER    PROC
                EXPORT  MADC_HANDLER			  [WEAK]
                B       .
                ENDP
UART0_HANDLER    PROC
                EXPORT  UART0_HANDLER			  [WEAK]
                B       .
                ENDP
UART1_HANDLER   PROC
                EXPORT  UART1_HANDLER			  [WEAK]
                B       .
                ENDP
UART2_HANDLER   PROC
                EXPORT  UART2_HANDLER			  [WEAK]
                B       .
                ENDP
UART3_HANDLER   PROC
                EXPORT  UART3_HANDLER			  [WEAK]
                B       .
                ENDP
SPI_HANDLER     PROC
                EXPORT  SPI_HANDLER				  [WEAK]
                B       .
                ENDP
I2C_HANDLER     PROC
                EXPORT  I2C_HANDLER				  [WEAK]
                B       .
                ENDP
ISO78161_HANDLER    PROC
                EXPORT  ISO78161_HANDLER		  [WEAK]
                B       .
                ENDP
ISO78160_HANDLER    PROC
                EXPORT  ISO78160_HANDLER		  [WEAK]
                B       .
                ENDP
TC0_HANDLER     PROC
                EXPORT  TC0_HANDLER				  [WEAK]
                B       .
                ENDP
TC1_HANDLER     PROC
                EXPORT  TC1_HANDLER				  [WEAK]
                B       .
                ENDP
UART5_HANDLER     PROC
                EXPORT  UART5_HANDLER				  [WEAK]
                B       .
                ENDP
WDT_HANDLER     PROC
                EXPORT  WDT_HANDLER				  [WEAK]
                B       .
                ENDP
KBI_HANDLER     PROC
                EXPORT  KBI_HANDLER				  [WEAK]
                B       .
                ENDP
LCD_HANDLER     PROC
                EXPORT  LCD_HANDLER				  [WEAK]
                B       .
                ENDP
VCH_HANDLER     PROC
                EXPORT  VCH_HANDLER				  [WEAK]
                B       .
                ENDP
CMP_HANDLER   PROC
                EXPORT  CMP_HANDLER			    [WEAK]
                B       .
                ENDP
                
                ALIGN                   ; 确保后续代码对齐


;------------------ 用户堆栈和堆初始化 ------------------
; 这部分代码用于配置堆和栈的内存区域，供C库使用。

                IF      :DEF:__MICROLIB ; 如果使用了Microlib C库
                
                EXPORT  __initial_sp    ; 导出堆栈顶部地址
                EXPORT  __heap_base     ; 导出堆起始地址
                EXPORT  __heap_limit    ; 导出堆结束地址
                
                ELSE                    ; 如果使用标准 C 库
                
                IMPORT  __use_two_region_memory ; 导入标准库函数，指示使用两块内存区域（栈和堆）
                EXPORT  __user_initial_stackheap ; 导出用户自定义的堆栈初始化函数
__user_initial_stackheap ; 用户自定义堆栈初始化函数入口
; 这个函数会被C库的初始化代码调用，用于获取堆和栈的地址范围。
                LDR     R0, =Heap_Mem               ; R0 = 堆起始地址 (__heap_base)
                LDR     R1, =(Stack_Mem + Stack_Size) ; R1 = 栈顶地址 (__initial_sp)
                LDR     R2, =(Heap_Mem + Heap_Size)   ; R2 = 堆结束地址 (__heap_limit)
                LDR     R3, =Stack_Mem              ; R3 = 栈底地址 (最低地址)
                BX      LR                          ; 返回，将地址信息传递给调用者

                ALIGN                   ; 确保后续代码对齐

                ENDIF                   ; 结束 IF 指令


;------------------ 文件结束 ------------------
                END                     ; 文件结束指令
