#include "Application_Main.h"

#define debug 	0

#if debug
#include "Driver_RTC.h"
#include "Driver_Flash.h"
#include "Middle_PID.h"
#include "Driver_Timer.h"
uint8_t debug_buff[10];
float data1[3] = {0.0f,0.0f,0.0f};	
	
RTC_DateTypeDef RTC_DateTypeDef_t;
RTC_TimeTypeDef RTC_TimeTypeDef_t;
int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Periphrials clock enable. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, 	ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, 	ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,		ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,		ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, 	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,	ENABLE);
	Mid_RGB_Init();
	Mid_Voice_Init();
	Mid_EEPROM_Init();
//	Mid_IWDG_Init();
	Mid_Flash_Init();
	Mid_RGB_Close();
	/* Periphrial config. */
//	GPIO_InitTypeDef 	debug_GPIO_Init;
//	debug_GPIO_Init.GPIO_Pin 	= GPIO_Pin_8; 
//	debug_GPIO_Init.GPIO_Mode 	= GPIO_Mode_OUT;
//	debug_GPIO_Init.GPIO_Speed 	= GPIO_High_Speed;
//	debug_GPIO_Init.GPIO_OType 	= GPIO_OType_PP;
//	debug_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOC,&debug_GPIO_Init);

//	Dri_Timer6Hardware_Init(90,1000);	//F_timer6 = 90MHz.
//	Dri_RTCHardware_Init();
//	Dri_FlashHardware_Init();
//	udelay_100us(3000);
//	Dri_Flash_Write(NULL);
//	uint16_t address = 0x0000;
//	uint8_t  data = 0x01;
//	Dri_Control_StartRGB(R_Light,SET);
	CALPress_Flow.calpress_buff[0] = 17190;
	CALPress_Flow.calpress_buff[1] = 24030;
	CALPress_Flow.calpress_buff[2] = 29250;
	CALPress_Flow.calpress_buff[3] = 33720;
	CALPress_Flow.calpress_buff[4] = 37740;
	CALPress_Flow.calpress_buff[5] = 41160;
	
	while(1){
		
		Dri_Flash_Write(&CALPress_Flow);
//		address = 0x0;
//		data = 0x01;
//		for(uint8_t i = 0;i < 7;i++){
//			Mid_EEPROM_WriteByte(address++,data);
//			data <<= 1;
//			udelay_100us(4);
//		}		
		// Mid_Voice_Alarm(high_pri);
//		RTC_GetDate(RTC_Format_BIN,&RTC_DateTypeDef_t);
//		RTC_GetTime(RTC_Format_BIN,&RTC_TimeTypeDef_t);
		// udelay_100us(400);
//		
//		address = 0x0;
//		for(uint8_t i = 0;i < 7;i++){
//			Mid_EEPROM_ReadByte(address++,&debug_buff[i]);
//		}
		udelay_100us(50000);
	}	
}

#else 
/* Function declaration. */
void Hardware_Init(void *pvParameter);
void MeasureStack(void *pvParameter);
void PowerOn_ValueInit(void);
void PowerOnCheck_Task(void *pvParameter);
void CreateTask_Task(void *pvParameter);

int main(void){
	
	/* ----------------- Create Semaphore. ----------------- */	
	// PowerOnSemaphore_Handle = xSemaphoreCreateBinary();
	
	/* ----------------- Create Queue. ----------------- */	
	BlowerStateQueue_Handle = xQueueCreate(1,sizeof(Blower_State_t));
	BlowerRECQueue_Handle 	= xQueueCreate(10,sizeof(UARTQueue_t));
	BlowerSendQueue_Handle 	= xQueueCreate(10,sizeof(UARTQueue_t));
	CALDataQueue_Handle 	= xQueueCreate(1,sizeof(Calibration_t));
	RunParamQueue_Handle 	= xQueueCreate(1,sizeof(Run_Param_t));

	/* ----------------- Create Event Group. ----------------- */	
	MachineStateEvent_Handle = xEventGroupCreate();
	FeedDogEvent_Handle = xEventGroupCreate();
	
	/* ----------------- Create Task. ----------------- */	
	xTaskCreate(Hardware_Init,"Hardware_Init",100,NULL,11,NULL);		 		
	
	vTaskStartScheduler();
	return 0;
}

/* Periphal hardware Initialization. */
void Hardware_Init(void *pvParameter){
	/* FreeRTOS needs to use NVIC_PriorityGroup_4. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Periphrials clock enable. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, 	ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, 	ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,		ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,		ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, 	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,	ENABLE);
	/* Periphrial config. */
	Mid_TestPower_Init();
	while(Mid_TestPower() == RESET);
	Mid_Blower_Init();
	Mid_Pressure_Init();
	Mid_Flow_Init();
	Mid_RGB_Init();
	Mid_Flash_Init();
	Mid_Key_Init();
	Mid_Timer6_Init();
	Mid_Voice_Init();
	Mid_EEPROM_Init();
//	Mid_IWDG_Init();
	/* Initialize some global value. */
	PowerOn_ValueInit();
	if(CalibrationData.flag_rtc_init != 0x01)
		Mid_RTC_Init();
	/* Check peripheral. */
	xTaskCreate(PowerOnCheck_Task,"PowerOnCheck_Task",100,NULL,11,NULL);		
	vTaskDelete(NULL);
}

void PowerOn_ValueInit(void){
	/* ------ PID. ------ */
	Mid_IncreasePID_Init();
	/* ------ Flow filter. ------ */
	Flow_Kalman.q	   = 0.08f;
	Flow_Kalman.r	   = 5.0f;
	Flow_Kalman.p_last = 0.001f;
	Flow_Kalman.x_last = 0.001f;
		
	/* ------ Read data from flash. ------ */
	Mid_ReadCALData_Power();
	
	/* Delete!!! */
	Set_Param.mode = CPAP;
	Set_Param.delaypress_min = 0;
	Set_Param.start_press    = 500;
	Set_Param.therapy_press  = 600;
	Set_Param.epr = 5;
	Set_Param.flag_auto_on = SET;
	Set_Param.flag_auto_off = SET;
}

/* Delete all tasks when the power on check is over!!! */
void PowerOnCheck_Task(void *pvParameter){
	unsigned char time_count = 0;	
	xTaskCreate(App_ReceiveBlower_Task	,		
				"App_ReceiveBlower_Task",       
				RECMotorTask_DERTH	,       
				NULL				,       
				RECMotorTask_Priority,      
				&RECMotorTaskHandle);       
	/* Check blower chip. */
	while(1){
		Mid_Feed_IWDG();
		Mid_SendToBlower(SEND_COMMAND,0x0001);
		if(Blower_State.state == BlowerNormal || time_count++ >= 50){
			vTaskDelete(RECMotorTaskHandle);
			RECMotorTaskHandle = NULL;
			break;
		}
		vTaskDelay(pdMS_TO_TICKS(200));
	}
	xTaskCreate(CreateTask_Task,"CreateTask_Task",100,NULL,11,NULL);	
	vTaskDelete(NULL);
}	

/* Create all tasks. */
void CreateTask_Task(void *pvParameter){
	Mid_Feed_IWDG();
	xTaskCreate(App_ReceiveBlower_Task,	
				"App_ReceiveBlower_Task",   
				RECMotorTask_DERTH,       
				NULL,       
				RECMotorTask_Priority,      
				&RECMotorTaskHandle);      
	
	xTaskCreate(App_ControlBlower_Task,		
				"App_ControlBlower_Task",   	
				CONBlowerTask_DERTH,   	
				NULL,      
				CONBlowerTask_Priority,    
				&CONBlowerTaskHandle);    

	xTaskCreate(App_Calibration_Task,		
				"App_Calibration_Task",   	
				CALDataTask_DERTH,   	
				NULL,      
				CALDataTask_Priority,    
				&CALDataTaskHandle);     

	xTaskCreate(App_Machine_OnOff_Task,		
				"App_Machine_OnOff_Task",   	
				MachineOnOffTask_DERTH,   	
				NULL,      
				MachineOnOffTask_Priority,    
				&MachineOnOffTaskHandle);     

	xTaskCreate(App_MaskDetection_Task,		
				"App_MaskDetection_Task",   	
				TestMaskTask_DERTH,   	
				NULL,      
				TestMaskTask_Priority,    	
				&TestMaskTaskHandle);     	

	// xTaskCreate(Mid_CalculateFlow	,		//Function name.
	// 			"Mid_CalculateFlow"	,       //Task name.	
	// 			FlowTask_DERTH		,       //Task depth.
	// 			NULL				,       //Task parameter.
	// 			FlowTask_Priority	,      	//Task priority.
	// 			&FlowTaskHandle)	;       //Task handle.
	
	// xTaskCreate(Mid_RGB_Task		,		//Function name.
	// 			"Mid_RGB_Task"		,       //Task name.	
	// 			RGBTask_DERTH		,       //Task depth.
	// 			NULL				,       //Task parameter.
	// 			RGBTask_Priority	,      	//Task priority.
	// 			&RGBTaskHandle)		;       //Task handle.
				
	// xTaskCreate(Mid_WriteCALData	,		//Function name.
	// 			"Mid_WriteCALData"	,       //Task name.	
	// 			WCALDataTask_DERTH	,       //Task depth.
	// 			NULL				,       //Task parameter.
	// 			WCALDataTask_Priority,      //Task priority.
	// 			&WCALDataTaskHandle);   	//Task handle.

	// xTaskCreate(Mid_JudgeInsEx		,		//Function name.
	//  			"Mid_JudgeInsEx"	,  		//Task name.	
	//  			JudgeInsExTask_DERTH,       //Task depth.
	//  			NULL				,       //Task parameter.
	//  			JudgeInsExTask_Priority,    //Task priority.
	//  			&JudgeInsExTaskHandle);   	//Task handle.		
	
	// xTaskCreate(Mid_MachineAlarm	,		//Function name.
	// 			"Mid_MachineAlarm"	,  		//Task name.	
	// 			AlarmTask_Depth     ,       //Task depth.
	// 			NULL				,       //Task parameter.
	// 			AlarmTask_Priority  ,    	//Task priority.
	// 			&AlarmTaskHandle)   ;   	//Task handle.					
				
	// xTaskCreate(Mid_CalFlow_Pressure,		//Function name.
	// 			"Mid_CalFlow_Pressure",     //Task name.	
	// 			CALDataTask_DERTH	,       //Task depth.
	// 			NULL				,       //Task parameter.
	// 			CALDataTask_Priority,      	//Task priority.
	// 			&CALDataTaskHandle)	;       //Task handle.
				
	xTaskCreate(MeasureStack		,		//Function name.
				"MeasureStack"		,       //Task name.	
				MeasureTask_DERTH	,       //Task depth.
				NULL				,       //Task parameter.
				MeasureTask_Priority,       //Task priority.
				&MeasureTaskHandle)	;       //Task handle.

	xTaskCreate(App_Cali_Debug_Function,//Function name.
				"Debug_Function"	,   	//Task name.	
				100	,       				//Task depth.
				NULL				,       //Task parameter.
				2,       					//Task priority.
				NULL);       				//Task handle.
	vTaskDelete(NULL);
}

UBaseType_t depth[5];
void MeasureStack(void *pvParameter){
	while(1){
		depth[0] = uxTaskGetStackHighWaterMark(CONBlowerTaskHandle);
		depth[1] = uxTaskGetStackHighWaterMark(MachineOnOffTaskHandle);
//		depth[2] = uxTaskGetStackHighWaterMark(RGBTaskHandle);
//		depth[3] = uxTaskGetStackHighWaterMark(NULL);
//		depth[4] = uxTaskGetStackHighWaterMark(MachineRunTaskHandle);
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}	

#endif
