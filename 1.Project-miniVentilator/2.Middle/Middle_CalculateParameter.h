#ifndef Middle_CalculateParameter_H
#define Middle_CalculateParameter_H

#include <stdint.h>
#include "Add_Function.h"

/* Update the baseline of the one cycle of breathe flow. */
int Mid_Update_FlowBaseLine(int *pflowbuff,uint16_t buffcount);
/* Update the flow value about the end of expiration when machine is just starting. */
int Mid_Update_EXEnd_Flow(int *pflow_value);

#endif
