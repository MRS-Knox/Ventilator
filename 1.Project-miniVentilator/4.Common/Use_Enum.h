#ifndef Use_Enum_H
#define Use_Enum_H

/* About blower. */
typedef enum{
	BlowerPowerE = 0,
	BlowerNormal,
	BlowerError,
}eBlower_State;
typedef enum{
	Blower_Stop = 0,		
	Blower_Start,
	Blower_ConstantRPM,		
	Blower_PIDRPM,		
}eBlowerRunStage;

/* About machine run. */
typedef enum{
	Machine_Stop = 0,		//Machine is stopping.
	Machine_Start,			//Machine just turned on.
	Machine_DelayIP,		//Delay increase pressure.
	Machine_Run,			//Machine normal running now.
}eMachine_RunStage;


typedef enum{
	Test_Stop = 0,	
	Test_Start,
	Test_Run,
}eTestMask_Step;

typedef enum{
	/* Alarm. */
	low_alarm			= 52,
	middle_alarm		,
	high_alarm			,
	/* About voice volume. */
	max_volume			= 0xEF,
	/* About voice command */
	loop_broadcast	 	= 0xF2,		
	rank_broadcast		= 0xF3,
	silence				= 0xF8,
	stop_broadcast		= 0xFE,
	none				= 0xFF,
}eVoice_Content;

typedef enum{
	high_pri 	=  0x01,
	middle_pri 	,			
	low_pri		,
	none_alarm	,
}eAlarm_Priority; 

typedef enum{
	None 	=  0x0,
	Ins_Start 	,			
	Ins_End		,
	Ex_Start 	,			
	Ex_End		,	
}eBreathe_Stage; 

typedef enum{
	CPAP 	=  0x03,
	AutoCPAP 	,			
	AutoS		,
}eTherapy_Mode; 

typedef enum{
	Mini_1_A20 = 0x01,		//CPAP  	 2~20cmH2O
	Mini_1_B20 = 0x02,		//APAP  	 2~20cmH2O
	Mini_1_P20 = 0x04,		//CPAP,APAP  2~20cmH2O
}eMachine_Model;

#endif	/* Use_Enum_H */
