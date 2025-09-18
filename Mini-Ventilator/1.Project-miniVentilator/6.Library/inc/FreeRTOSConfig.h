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
        config�����������ɹ��ܵ����úͲü�                                                     
************************************************************/
//���ϣ��ʹ����ռʽRTOS���������뽫������Ϊ1�����ϣ��ʹ��ЭͬʽRTOS���������뽫������Ϊ 0��
#define configUSE_PREEMPTION                        1	
//����Ϊ1ʱʹ��Ӳ��������һ��Ҫ���е���������Ϊ0ʹ�����������һ��Ҫ���е�����
#define configUSE_PORT_OPTIMISED_TASK_SELECTION     1
//����Ϊ1ʱʹ�ܵ͹���ticklessģʽ������Ϊ0ʱ����ϵͳ����(SysTick)�ж�һֱ����
#define configUSE_TICKLESS_IDLE                     0
//����CPU��Ƶ
#define configCPU_CLOCK_HZ                          SystemCoreClock 
//���õδ�ʱ��ʱ��Դ��Ƶ��ֻ�еδ�ʱ��ʱ��Դ��Ƶ��CPU��Ƶ��ͬ��ʱ�����Ҫ���ã�STM32F429��������
//#define configSYSTICK_CLOCK_HZ                   	1000000 
//���õδ��жϵ�Ƶ�ʣ�1000Hz��1ms�ж�һ��   
#define configTICK_RATE_HZ                          1000
//����������ȼ�����������ȼ��� = configMAX_PRIORITIES-1  
#define configMAX_PRIORITIES                        32 
//���ÿ�������Ķ�ջ��С����λ��word
#define configMINIMAL_STACK_SIZE                    128
//����������������ַ���������NULL��ֹ�ֽڣ�Ĭ��16����
#define configMAX_TASK_NAME_LEN                     16
//����TickType_t�������ͣ�Ϊ1ʱ��Ϊuint16_t��Ϊ0ʱ��Ϊuint32_t
#define configUSE_16_BIT_TICKS                      0 
//���ÿ���������һ��ʱ��Ƭδ����֮ǰ�Ƿ���Ա���ռ
#define configIDLE_SHOULD_YIELD                     1
//�����Ƿ�ʹ�������ֱ�ӵ���Ϣ���ݣ������ź����������
#define configUSE_TASK_NOTIFICATIONS                1
//���������е�������
#define configTASK_NOTIFICATION_ARRAY_ENTRIES       3
//�����Ƿ�ʹ�û�����
#define configUSE_MUTEXES                           0
//�����Ƿ�ʹ�õݹ黥����
#define configUSE_RECURSIVE_MUTEXES                 0
//�����Ƿ�ʹ�ü����ź���
#define configUSE_COUNTING_SEMAPHORES               0
//�������Ѿ�����
#define configUSE_ALTERNATIVE_API                   0 /* Deprecated! */
//ͨ���˶��������ÿ���ע����ź�������Ϣ���и���
#define configQUEUE_REGISTRY_SIZE                   10
//���ó�1ʹ�ܶ��м�����(�������������𵽶�����к��ź���)�����ó�0ȡ�����м�����
#define configUSE_QUEUE_SETS                        0
//�����Ƿ�ʹ��ʱ��Ƭ���ȣ���ͬ���ȼ��������������ִ��
#define configUSE_TIME_SLICING                      1
//�����Ƿ�ʹ�����񴴽�ʱ����newlib������ṹ�壬Ŀǰ����Ҫ
#define configUSE_NEWLIB_REENTRANT                  0
//�����Ƿ���Լ����ϰ汾��FreeRTOS�ں˵Ĵ���
#define configENABLE_BACKWARD_COMPATIBILITY         0
//����ÿ��������̱߳��ش洢ָ�������С 
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS     0
//����MiniListItem_t��ListItem_t�Ƿ񱣳�һ��
#define configUSE_MINI_LIST_ITEM                    1 
//�������õ���xTaskCreate()ʱ����ָ����ջ��ȵ����ͣ��Լ��������ʹ�ö�ջ��С�ĵط�
#define configSTACK_DEPTH_TYPE                      uint16_t 
//FreeRTOS��Ϣ������ʹ��configMESSAGE_BUFFER_LENGTH_TYPE���͵ı����洢ÿ����Ϣ�ĳ���
#define configMESSAGE_BUFFER_LENGTH_TYPE            size_t
//�������Ϊ1����ʹ��pvPortMalloc()������ڴ�� ����ͨ��vPortFree()�ͷ�ʱ����������δ���壬configHEAP_CLEAR_MEMORY_ON_FREEĬ��Ϊ0����ʵ��������
#define configHEAP_CLEAR_MEMORY_ON_FREE             1

/* �ڴ������ض��� */
//1����RTOS�������ͨ��Ӧ�ó����д���ṩ��RAM������0��RTOS����ֻ��ͨ��FreeRTOS���з����RAM���� 
#define configSUPPORT_STATIC_ALLOCATION             0
//1��RTOS�����ͨ��RAM(FreeRTOS�����Զ�����)������0����RTOS����ֻ��ͨ��Ӧ�ó����д���ṩ��RAM���� 
#define configSUPPORT_DYNAMIC_ALLOCATION            1
//FreeRTOS���п��õ�RAM����,��ʱ�����Ϊ50K
#define configTOTAL_HEAP_SIZE                       ((size_t)(50 * 1024))
//1���û��ֶ�����FreeRTOS���е�RAM��0��ϵͳ�Զ�����
#define configAPPLICATION_ALLOCATED_HEAP            0
//1���û��Լ���д���񴴽�ʱʹ�õ��ڴ�������ͷź�����0��ʹ��RTOS�Դ��ĺ���
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   0 

/* ���Ӻ�����صĶ��� */
//�����Ƿ�ʹ�ܿ�������Ĺ��Ӻ������û��Լ�ʵ��
#define configUSE_IDLE_HOOK                         0
//�����Ƿ�ʹ��ϵͳʱ�ӽ����жϵĹ��Ӻ������û��Լ�ʵ��
#define configUSE_TICK_HOOK                         0
//�����Ƿ�ʹ��ջ�����⡣0����ʹ�ܣ�1��ʹ�÷���һ��⣻2��ʹ�÷��������
#define configCHECK_FOR_STACK_OVERFLOW              0
//�����Ƿ�ʹ��ʹ�ö�̬�ڴ����ʧ�ܵĹ��Ӻ������û��Լ�ʵ��
#define configUSE_MALLOC_FAILED_HOOK                0
//�����Ƿ�ʹ�ܶ�ʱ�������״�ִ�еĹ��Ӻ������û��Լ�ʵ��
#define configUSE_DAEMON_TASK_STARTUP_HOOK          0
//�����Ƿ�xStreamBufferCreateWithCallback()��xStreamBufferCreateStaticWithCallback()(������Ϣ�������ĵ�Ч����)�����ڹ�����
#define configUSE_SB_COMPLETED_CALLBACK             0

/* ����ʱ�������״̬ͳ�Ƶ���ض��� */
//�����Ƿ�ʹ�����������ʱ��ͳ�ƹ���
#define configGENERATE_RUN_TIME_STATS               0
//�����Ƿ�ʹ�ܿ��ӻ����ٵ���
#define configUSE_TRACE_FACILITY                    1  //������ͨ���ڵ���ʱ�Ż�ʹ�ã�������������ʱ����رգ���Ϊ���FreeRTOS��������Ӱ��
//�����Ƿ�����vTaskList()��vTaskGetRunTimeStats()����������������Ҳ�ǵ���ʱʹ�õ�
#define configUSE_STATS_FORMATTING_FUNCTIONS        1

/* Э����ض���(����Э�̲���Ҫ��) */
#define configUSE_CO_ROUTINES                       0
#define configMAX_CO_ROUTINE_PRIORITIES             1

/* �����ʱ����ض��� */
//�Ƿ�ʹ�������ʱ������
#define configUSE_TIMERS                            1
//�����ʱ�������ȼ���ͨ������Ϊ��󣬷�ֹʱ�䲻׼
#define configTIMER_TASK_PRIORITY                   (configMAX_PRIORITIES-1)
//���г���
#define configTIMER_QUEUE_LENGTH                    10
//��ջ��С
#define configTIMER_TASK_STACK_DEPTH                (configMINIMAL_STACK_SIZE*2)

/* �ж�Ƕ����Ϊ���� */
/*���������������ȼ�ʱ�ǰ���8λ�еĵ���λ��д�ģ���ΪSTM32F4xxӲ���ж����ȼ��õ�
�Ǹ���λ(���ȼ������ģ�ֻ����ռ���ȼ�0~15)��������Ҫ������λ */
//Cortex-M�ں�ʹ��8bit���������ȼ�������STM32ֻʹ���˸�4bit����ֵԽС�����ȼ�Խ�ߡ�
#ifdef __NVIC_PRIO_BITS  //__NVIC_PRIO_BITS�Ѿ���stm32f4xx.h���涨��Ϊ4
    #define configPRIO_BITS               	    	__NVIC_PRIO_BITS
#else
    #define configPRIO_BITS               	    	4
#endif
//SysTick��PenV�ж����ȼ����ã�SysTick�����ȼ�����һ������Ϊ��ͣ���0x0f�������������ϵͳ��ʵʱ��Ӧ���������������ⲿ�жϿ��Լ�ʱ�ĵõ���Ӧ��
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY     15
#define configKERNEL_INTERRUPT_PRIORITY             (configLIBRARY_LOWEST_INTERRUPT_PRIORITY<<(8-configPRIO_BITS))
//�����ε��ж����ȼ�����
/* ��������STM32������Ĵ���basepri�Ĵ�����ֵ�����������жϣ�������basepriֵ�����ȼ����жϽ���ȫ�����Ρ�basepriֻ��4bit��Ч��
 * Ĭ��ֻΪ0����ȫ���ж϶�û�б����Ρ�configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY����Ϊ��5����˼�����ж����ȼ�����5���ж϶������Ρ�
 * �������úõ����ȼ�д���Ĵ�����ʱ���ǰ���8bit��д�ģ���������д��ʱ����Ҫ����ת������ʽΪ��
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)�����е�priority�����������õ����������ȼ���*/
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
#define configMAX_SYSCALL_INTERRUPT_PRIORITY         (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY<<(8-configPRIO_BITS))
#define configMAX_API_CALL_INTERRUPT_PRIORITY        configMAX_SYSCALL_INTERRUPT_PRIORITY

/* FreeRTOS�жϷ�������ض��� */
#define xPortPendSVHandler                           PendSV_Handler
#define vPortSVCHandler                              SVC_Handler

//#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

/* �����ڼ䲶׽������ض��� */
//#define vAssertCalled(char,int) 		     printf("ERROR:%s,%d\r\n",char,int)
//#define configASSERT( x )  			     if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* FreeRTOS MPU ���ⶨ�� */
// #define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
// #define configTOTAL_MPU_REGIONS                     8 /* Default value */
// #define configTEX_S_C_B_FLASH                       0x07UL /* Default value */
// #define configTEX_S_C_B_SRAM                        0x07UL /* Default value */
// #define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY 1
// #define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS  1
// #define configENABLE_ERRATA_837070_WORKAROUND       1

/* ARMv8-M ��ȫ��˿���ض��� */
// #define secureconfigMAX_SECURE_CONTEXTS             5

/************************************************************
        include��ͷ�����������FreeRTOS�п�ѡ��API����                                                    
************************************************************/
/* ��ѡ������1:ʹ�� */
//���úͻ�ȡ�������ȼ�����
#define INCLUDE_vTaskPrioritySet                    1
#define INCLUDE_uxTaskPriorityGet             	    1
//����ɾ����������
#define INCLUDE_vTaskDelete                  	    1
#define INCLUDE_vTaskSuspend                  	    1
//�ָ����ж��й����������
#define INCLUDE_xResumeFromISR                	    1
//���������ʱ����
#define INCLUDE_vTaskDelayUntil               	    1
//������ʱ����
#define INCLUDE_vTaskDelay                    	    1
//��ȡ���������״̬����
#define INCLUDE_xTaskGetSchedulerState         	    1
//��ȡ��ǰ���������������
#define INCLUDE_xTaskGetCurrentTaskHandle      	    1 
//uxTaskGetStackHighWaterMark()��uxTaskGetStackHighWaterMark2()
#define INCLUDE_uxTaskGetStackHighWaterMark    	    1
#define INCLUDE_uxTaskGetStackHighWaterMark2   	    0
//��ȡ�������������������
#define INCLUDE_xTaskGetIdleTaskHandle       	    1
//��ȡ����״̬����
#define INCLUDE_eTaskGetState                  	    1
//���ж��������¼���־λ
#define INCLUDE_xEventGroupSetBitFromISR      	    1
//��������ִ�йҵ���ʱ����������
#define INCLUDE_xTimerPendFunctionCall        	    1
//�ж�������ʱ
#define INCLUDE_xTaskAbortDelay                     1
//ͨ����������ȡ������
#define INCLUDE_xTaskGetHandle                	    1
//�ָ����ж��й����������
#define INCLUDE_xTaskResumeFromISR           	    1

#endif /* FREERTOS_CONFIG_H */
