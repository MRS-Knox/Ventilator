 /*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 *
 *-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f4xx.h"
//#include <stdio.h>

/************************************************************
        config开发配置项，完成功能的配置和裁剪                                                     
************************************************************/
//如果希望使用抢占式RTOS调度器，请将其设置为1；如果希望使用协同式RTOS调度器，请将其设置为 0。
#define configUSE_PREEMPTION                        1	
//配置为1时使用硬件计算下一个要运行的任务，配置为0使用软件计算下一个要运行的任务
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     1
//配置为1时使能低功耗tickless模式，配置为0时保持系统节拍(SysTick)中断一直运行
#define configUSE_TICKLESS_IDLE                     0
//配置CPU主频
#define configCPU_CLOCK_HZ                          SystemCoreClock 
//配置滴答定时器时钟源主频，只有滴答定时器时钟源主频与CPU主频不同的时候才需要配置，STM32F429无需配置
//#define configSYSTICK_CLOCK_HZ                   	1000000 
//配置滴答中断的频率，1000Hz即1ms中断一次   
#define configTICK_RATE_HZ                          1000
//配置最大优先级数，最大优先级级 = configMAX_PRIORITIES-1  
#define configMAX_PRIORITIES                        32 
//配置空闲任务的堆栈大小，单位是word
#define configMINIMAL_STACK_SIZE                    128
//配置任务名的最大字符数，包括NULL终止字节，默认16即可
#define configMAX_TASK_NAME_LEN                     16
//配置TickType_t变量类型，为1时其为uint16_t，为0时其为uint32_t
#define configUSE_16_BIT_TICKS                      0 
//配置空闲任务在一个时间片未结束之前是否可以被抢占
#define configIDLE_SHOULD_YIELD                     1
//配置是否使能任务间直接的消息传递，包括信号量、数组等
#define configUSE_TASK_NOTIFICATIONS                1
//设置数组中的索引数
#define configTASK_NOTIFICATION_ARRAY_ENTRIES       3
//配置是否使用互斥量
#define configUSE_MUTEXES                           0
//配置是否使用递归互斥量
#define configUSE_RECURSIVE_MUTEXES                 0
//配置是否使用计数信号量
#define configUSE_COUNTING_SEMAPHORES               0
//！！！已经弃用
#define configUSE_ALTERNATIVE_API                   0 /* Deprecated! */
//通过此定义来设置可以注册的信号量和消息队列个数
#define configQUEUE_REGISTRY_SIZE                   10
//设置成1使能队列集功能(可以阻塞、挂起到多个队列和信号量)，设置成0取消队列集功能
#define configUSE_QUEUE_SETS                        0
//设置是否使能时间片调度，即同优先级的任务可以轮流执行
#define configUSE_TIME_SLICING                      1
//配置是否使能任务创建时分配newlib的重入结构体，目前不需要
#define configUSE_NEWLIB_REENTRANT                  0
//配置是否可以兼容老版本的FreeRTOS内核的代码
#define configENABLE_BACKWARD_COMPATIBILITY         0
//设置每个任务的线程本地存储指针数组大小 
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     0
//配置MiniListItem_t和ListItem_t是否保持一致
#define configUSE_MINI_LIST_ITEM                    1 
//配置设置调用xTaskCreate()时用于指定堆栈深度的类型，以及许多其他使用堆栈大小的地方
#define configSTACK_DEPTH_TYPE                      uint16_t 
//FreeRTOS消息缓冲区使用configMESSAGE_BUFFER_LENGTH_TYPE类型的变量存储每个消息的长度
#define configMESSAGE_BUFFER_LENGTH_TYPE            size_t
//如果设置为1，则使用pvPortMalloc()分配的内存块 会在通过vPortFree()释放时被清除。如果未定义，configHEAP_CLEAR_MEMORY_ON_FREE默认为0，以实现向后兼容
#define configHEAP_CLEAR_MEMORY_ON_FREE             1

/* 内存分配相关定义 */
//1：则RTOS对象可以通过应用程序编写者提供的RAM创建；0：RTOS对象只能通过FreeRTOS堆中分配的RAM创建 
#define configSUPPORT_STATIC_ALLOCATION             0
//1：RTOS对象可通过RAM(FreeRTOS堆中自动分配)创建；0：则RTOS对象只能通过应用程序编写者提供的RAM创建 
#define configSUPPORT_DYNAMIC_ALLOCATION            1
//FreeRTOS堆中可用的RAM总量,此时定义的为50K
#define configTOTAL_HEAP_SIZE                       ((size_t)(50 * 1024))
//1：用户手动分配FreeRTOS堆中的RAM；0：系统自动分配
#define configAPPLICATION_ALLOCATED_HEAP            0
//1：用户自己编写任务创建时使用的内存申请和释放函数；0：使用RTOS自带的函数
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   0 

/* 钩子函数相关的定义 */
//配置是否使能空闲任务的钩子函数，用户自己实现
#define configUSE_IDLE_HOOK                         0
//配置是否使能系统时钟节拍中断的钩子函数，用户自己实现
#define configUSE_TICK_HOOK                         0
//配置是否使能栈溢出检测。0：不使能；1：使用方法一检测；2：使用方法二检测
#define configCHECK_FOR_STACK_OVERFLOW              0
//配置是否使能使用动态内存分配失败的钩子函数，用户自己实现
#define configUSE_MALLOC_FAILED_HOOK                0
//配置是否使能定时器任务首次执行的钩子函数，用户自己实现
#define configUSE_DAEMON_TASK_STARTUP_HOOK          0
//配置是否将xStreamBufferCreateWithCallback()和xStreamBufferCreateStaticWithCallback()(及其消息缓冲区的等效函数)包含在构建中
#define configUSE_SB_COMPLETED_CALLBACK             0

/* 运行时间和任务状态统计的相关定义 */
//配置是否使能任务的运行时间统计功能
#define configGENERATE_RUN_TIME_STATS               0
//配置是否使能可视化跟踪调试
#define configUSE_TRACE_FACILITY                    1  //该配置通常在调试时才会使用，真正发布程序时必须关闭，因为其对FreeRTOS的性能有影响
//配置是否会编译vTaskList()和vTaskGetRunTimeStats()函数，这两个函数也是调试时使用的
#define configUSE_STATS_FORMATTING_FUNCTIONS        1

/* 协程相关定义(不用协程不需要管) */
#define configUSE_CO_ROUTINES                       0
#define configMAX_CO_ROUTINE_PRIORITIES             1

/* 软件定时器相关定义 */
//是否使能软件定时器功能
#define configUSE_TIMERS                            1
//软件定时器的优先级，通常配置为最大，防止时间不准
#define configTIMER_TASK_PRIORITY                   (configMAX_PRIORITIES-1)
//队列长度
#define configTIMER_QUEUE_LENGTH                    10
//堆栈大小
#define configTIMER_TASK_STACK_DEPTH                (configMINIMAL_STACK_SIZE*2)

/* 中断嵌套行为配置 */
/*！！！！定义优先级时是按照8位中的低四位来写的，因为STM32F4xx硬件中断优先级用的
是高四位(优先级分组四：只有抢占优先级0~15)，所以需要左移四位 */
//Cortex-M内核使用8bit来配置优先级，但是STM32只使用了高4bit，数值越小，优先级越高。
#ifdef __NVIC_PRIO_BITS  //__NVIC_PRIO_BITS已经在stm32f4xx.h里面定义为4
    #define configPRIO_BITS               	    	__NVIC_PRIO_BITS
#else
    #define configPRIO_BITS               	    	4
#endif
//SysTick和PenV中断优先级配置，SysTick的优先级我们一般配置为最低，即0x0f，这样可以提高系统的实时响应能力，即其他的外部中断可以及时的得到响应。
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     15
#define configKERNEL_INTERRUPT_PRIORITY             (configLIBRARY_LOWEST_INTERRUPT_PRIORITY<<(8-configPRIO_BITS))
//可屏蔽的中断优先级配置
/* 用于配置STM32的特殊寄存器basepri寄存器的值，用于屏蔽中断，当大于basepri值的优先级的中断将被全部屏蔽。basepri只有4bit有效，
 * 默认只为0，即全部中断都没有被屏蔽。configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY配置为：5，意思就是中断优先级大于5的中断都被屏蔽。
 * 当把配置好的优先级写到寄存器的时候，是按照8bit来写的，所以真正写的时候需要经过转换，公式为：
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)，其中的priority就是我们配置的真正的优先级。*/
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configMAX_SYSCALL_INTERRUPT_PRIORITY         (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY<<(8-configPRIO_BITS))
#define configMAX_API_CALL_INTERRUPT_PRIORITY        configMAX_SYSCALL_INTERRUPT_PRIORITY

/* FreeRTOS中断服务函数相关定义 */
#define xPortPendSVHandler                           PendSV_Handler
#define vPortSVCHandler                              SVC_Handler

//#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

/* 开发期间捕捉错误相关定义 */
//#define vAssertCalled(char,int) 		     printf("ERROR:%s,%d\r\n",char,int)
//#define configASSERT( x )  			     if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* FreeRTOS MPU 特殊定义 */
// #define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
// #define configTOTAL_MPU_REGIONS                     8 /* Default value */
// #define configTEX_S_C_B_FLASH                       0x07UL /* Default value */
// #define configTEX_S_C_B_SRAM                        0x07UL /* Default value */
// #define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY 1
// #define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS  1
// #define configENABLE_ERRATA_837070_WORKAROUND       1

/* ARMv8-M 安全侧端口相关定义 */
// #define secureconfigMAX_SECURE_CONTEXTS             5

/************************************************************
        include开头的配置项，配置FreeRTOS中可选的API函数                                                    
************************************************************/
/* 可选函数：1:使能 */
//设置和获取任务优先级函数
#define INCLUDE_vTaskPrioritySet                    1
#define INCLUDE_uxTaskPriorityGet             	    1
//任务删除、挂起函数
#define INCLUDE_vTaskDelete                  	    1
#define INCLUDE_vTaskSuspend                  	    1
//恢复在中断中挂起的任务函数
#define INCLUDE_xResumeFromISR                	    1
//任务绝对延时函数
#define INCLUDE_vTaskDelayUntil               	    1
//任务延时函数
#define INCLUDE_vTaskDelay                    	    1
//获取任务调度器状态函数
#define INCLUDE_xTaskGetSchedulerState         	    1
//获取当前任务的任务句柄函数
#define INCLUDE_xTaskGetCurrentTaskHandle      	    1 
//uxTaskGetStackHighWaterMark()和uxTaskGetStackHighWaterMark2()
#define INCLUDE_uxTaskGetStackHighWaterMark    	    1
#define INCLUDE_uxTaskGetStackHighWaterMark2   	    0
//获取空闲任务的任务句柄函数
#define INCLUDE_xTaskGetIdleTaskHandle       	    1
//获取任务状态函数
#define INCLUDE_eTaskGetState                  	    1
//在中断中设置事件标志位
#define INCLUDE_xEventGroupSetBitFromISR      	    1
//将函数的执行挂到定时器服务任务
#define INCLUDE_xTimerPendFunctionCall        	    1
//中断任务延时
#define INCLUDE_xTaskAbortDelay                     1
//通过任务名获取任务句柄
#define INCLUDE_xTaskGetHandle                	    1
//恢复在中断中挂起的任务函数
#define INCLUDE_xTaskResumeFromISR           	    1

#endif /* FREERTOS_CONFIG_H */
