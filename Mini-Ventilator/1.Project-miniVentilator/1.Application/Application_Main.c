#include "Application_Main.h"

void Hardware_Init(void);
void MeasureStack(void *pvParameter);

int main(void){
	Hardware_Init();
	
	/* Create queue. */	
//	PRESSQueue_Handle = xCreateQueue(1,);
	
	/* Create task. */	
	xTaskCreate(Mid_CalculatePRESS	,		//Function name.
				"Mid_CalculatePRESS",		//Task name.		
				PRESSTask_DERTH		,		//Task depth.
				NULL				,		//Task parameter.
				PRESSTask_Priority	,		//Task priority.
				&PRESSTaskHandle)	;		//Task handle.
	
	xTaskCreate(MeasureStack		,		//Function name.
				"MeasureStack"		,       //Task name.	
				MeasureTask_DERTH	,       //Task depth.
				NULL				,       //Task parameter.
				MeasureTask_Priority,       //Task priority.
				&MeasureTaskHandle)	;       //Task handle.
	
	vTaskStartScheduler();
	return 0;
}

/* Periphal hardware Initialization. */
void Hardware_Init(void){
	unsigned char time_count = 0;
	/* FreeRTOS needs to use NVIC_PriorityGroup_4. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/* Periphrials clock enable. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,	ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,	ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,	ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, 	ENABLE);
	/* Periphrial config. */
	Mid_Blower_Init();
	Mid_Pressure_Init();
	
	
	
	udelay_1ms(20);
	/* Check blower chip. */
	while(1){
		Mid_SendToBlower(SEND_COMMAND,0x0001);
		udelay_1ms(20);
		if(Blower_State.state == BlowerNormal || time_count++ >= 50)
			break;
		udelay_1ms(500);
	}
	Mid_SendToBlower(SEND_RPM,20000);	
}

UBaseType_t ii;
void MeasureStack(void *pvParameter){
	while(1){
		ii = uxTaskGetStackHighWaterMark(PRESSTaskHandle);
		vTaskDelay(pdTICKS_TO_MS(50));
	}
}	

