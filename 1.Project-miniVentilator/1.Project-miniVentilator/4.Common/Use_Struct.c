#include "Use_Struct.h"

Blower_State_t			Blower_State;           //Record blower state.
Run_Param_t				Run_Param;              //Record run-data.
Set_Param_t				Set_Param;			    //Record set-data.
UARTQueue_t				BlowerSendQueue;	    //Blower send queue.
UARTQueue_t				BlowerReceiveQueue;	    //Blower receive queue.
CalibrationData_t		CalibrationData;	    //Record calibration data.
Machine_State_t			Machine_State;		    //Record machine state.
Machine_Time_t			Machine_Time;		    //Record machine time.
IncreasePID_t			IncreasePID;		    //PID parameters.
KalmanFilter_t			Flow_Kalman;		    //Flow filter.
Calibration_t			Calibration;		    //Calibration struct.
SetConsumable_Warn_t    SetConsumable_Warn;     //Set warning period of consumable.



