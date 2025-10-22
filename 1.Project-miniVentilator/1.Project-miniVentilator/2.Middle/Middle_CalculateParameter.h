#ifndef Middle_CalculateParameter_H
#define Middle_CalculateParameter_H

#include <stdint.h>
#include "Add_Function.h"

#define LITTLERANGE_LENGENTH	5
#define CONTINUES_LENGENTH		10	

/* Update the flow value about the end of expiration when machine is just starting. */
int Mid_Update_EXEnd_Flow(int *pflow_value,int *pstd_buff);
/* Calculate the running pressure. */
void Mid_CalculateRunPRESS(uint16_t *prun_press,uint16_t set_press,eBreathe_Stage now_stage);
/* Update some parameters when machine on. */
void Mid_Update_PARAM_AboutBPM(eBreathe_Stage now_stage);


/* Update the baseline of the one cycle of breathe flow. */
// int Mid_Update_FlowBaseLine(int *pflowbuff,uint16_t buffcount);

#endif
