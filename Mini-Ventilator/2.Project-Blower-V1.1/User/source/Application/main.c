/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      main.c
 * @author    Fortiortech  Appliction Team
 * @since     Create:2021-05-14
 * @date      Last modify:2022-07-14
 * @brief     包含主函数，软硬件初始化函数，VRER配置函数
 */


#include <FU68xx_4.h>
#include <Myproject.h>
#include "UART.h"
/*  ------------------------------------------------------------------------------------
    Internal Routine Prototypes
    ------------------------------------------------------------------------------------ */
void HardwareInit(void);
void SoftwareInit(void);
void VREFConfigInit(void);

/**
 * @brief        主函数主要功能是初始化，
                     1、包括上电等待，软件初始化，硬件初始化，调试模式设置，主循环扫描。
                     2、软件初始化--初始化所有定义的变量
                     3、硬件初始化--初始化硬件设备配置
                     4、大循环运行偏置电流采集函数，电机状态机控制函数，以及环路响应函数
 */
void main(void)
{
    uint32 PowerUpCnt = 0;
    
    for (PowerUpCnt = 0; PowerUpCnt < SystemPowerUpTime; PowerUpCnt++) {};
    
    /* -----ADC参考电压配置----- */
    VREFConfigInit();
    
    /* -----硬件初始化----- */
    HardwareInit();
    
    /* -----软件初始化----- */
    SoftwareInit();

	FG.TIMcalc_temp = 60 / Pole_Pairs * TIM4_Fre / FG_K;          //用来定时器输出FG信号
	EA = 1;

    /* -----等待主芯片发送通电数据----- */
	while(1){
		if(Motor_Return() == 1)
			break;
		for(PowerUpCnt = 0;PowerUpCnt <= 2000;PowerUpCnt++)
			_nop_();
	}
    
    /* -----看门狗初始化----- */
    WatchDogConfig(200);
	
	PowerUpCnt = 0;
	
    while (1)
    {
		/* -----机器开机运行----- */
		if(blower_state_t.set_speed > 0 || (blower_state_t.set_speed == 0 && blower_state_t.actual_speed > 0)){
			/* -----硬件电路校准---- */
			GetCurrentOffset();
			/* -----电机状态机----- */
			MC_Control();
		}
		/* -----1ms中断服务函数----- */
		if (g_1mTick)
		{
			Function_1_MS_((uint32)blower_state_t.set_speed);
			SetBit(WDT_CR, WDTRF);                    //看门狗清零
			EA = 0;
			g_1mTick = 0;
			EA = 1;
			blower_state_t.actual_speed = (mcFocCtrl.SpeedFlt*MOTOR_SPEED_BASE)/32767;	
			PowerUpCnt++;
		}
		/* -----200ms返回一次数据----- */
		if(PowerUpCnt >= 200)	
		{
			PowerUpCnt = 0;
			Motor_Return();
		}
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : HardwareInit
    Description    : 硬件初始化，初始化需要使用的硬件设备配置，FOC必须配置的是运放电压、运放初始化、ADC初始化、Driver初始化
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void HardwareInit(void)
{
    GPIO_DefaultInit();   //配置上拉
    #if (HardwareCurrent_Protect == Hardware_CMP_Protect)
    {
        /* -----硬件过流，比较器初始化用于硬件过流保护----- */
        CMP3_Init();
    }
    #else
    {
        _nop_();
    }
    #endif
    /* -----GPIO初始化----- */
    GPIO_Init();
    /* -----ADC初始化----- */
    ADC_Init();
    /* -----驱动初始化配置----- */
    Driver_Init();
    /* -----运放初始化----- */
    AMP_Init();
    /* -----中断配置----- */
    CMP3_Interrupt_Init();
    /* -----UART2配置----- */
	UART2_Init();
    #if (FGEnable)
    {
        /* -----FG对应Timer初始化配置----- */
        Timer4_Init();
    }
    #endif
    #if (SPEED_MODE == PWMMODE)
    {
        /* -----PWM捕获对应Timer初始化配置----- */
        Timer3_Init();
    }
    #endif
    PTIM4S1 = 1;
    PTIM4S0 = 0;                                    //Systick(1ms定时中断)优先级别为1
    SetBit(DRV_SR, SYSTIE);
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : SoftwareInit
    Description    : 软件初始化
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void SoftwareInit(void)
{
    /* -----变量初始化----- */
    MotorcontrolInit();
    mcState = mcReady;
    mcFaultSource = FaultNoSource;
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : VREFConfigInit
    Description    : 配置VREF/VHALF输出
    Date           : 2021-12-03
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void VREFConfigInit(void)
{
    ClrBit(VREF_VHALF_CR, VRVSEL1);             //00-->4.5V   01-->VDD5
    SetBit(VREF_VHALF_CR, VRVSEL0);             //10-->3.0V   11-->4.0V
    SetBit(VREF_VHALF_CR, VREFEN | VHALFEN);
}
