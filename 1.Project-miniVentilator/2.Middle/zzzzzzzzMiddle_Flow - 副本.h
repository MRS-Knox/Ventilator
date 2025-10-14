// // #ifndef Middle_Flow_H
// // #define Middle_Flow_H

// #include "math.h"
// #include "Driver_SDP31.h"
// #include "FreeRTOS.h"
// #include "task.h"
// #include "event_groups.h"
// #include "Add_Function.h"
// #include "Use_Struct.h"
// #include "FreeRTOS_Define.h"

// /* This initial function is about flow sensor hardware. */
// void Mid_Flow_Init(void);
// /* Reset sensor. */
// void Mid_SDP31_RESET(void);
// /* This function is about to calculate difference pressure. */
// FlagStatus Mid_CalDifferenceP(float *pdata);
// /* Calibrate flow base on difference pressure. */
// void Mid_CalibrateFlow(void);
// /* This function is about to calculate flow. */
// void Mid_CalculateFlow(void *pvParameter);
// /* Calculate coefficient(k,b). */
// void Mid_CALCoefficient(void);

// // #endif



