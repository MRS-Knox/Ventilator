#ifndef Application_Main_H
#define Application_Main_H

/* Header file about stm32f4xx. */
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
/* Header file about freertos. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* Header file about user. */
#include "Application_ReceiveBlower.h"
#include "Application_ControlBlower.h"
#include "Application_Calibration.h"
#include "Application_MaskDetection.h"
#include "Application_Machine_OnOff.h"

#include "FreeRTOS_Define.h"
#include "Middle_Blower.h"
#include "Middle_Pressure.h"
#include "Middle_Flow.h"
#include "Middle_RGB.h"
#include "Middle_Flash.h"
#include "Middle_Key.h"
#include "Middle_Timer.h"
#include "Middle_Voice.h"
//#include "Middle_BreatheStage.h"
#include "Middle_TestMask.h"
#include "Middle_Alarm.h"
#include "Middle_EEPROM.h"
#include "Middle_TestPower.h"
#include "Middle_WatchDog.h"


#endif 

