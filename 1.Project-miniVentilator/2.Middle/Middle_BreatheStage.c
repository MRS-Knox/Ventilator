#include "Middle_BreatheStage.h"

/*!
	@brief 		 Inspiration stage process function.
	@param[in]	 set_press:Current pressure.
				 therapy_mode:Therapy mode.
				 ins_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_Inspiration_Stage(unsigned short set_press,eBreathe_Stage ins_stage){
    uint16_t return_press = set_press;
	if(ins_stage == Ins_Start)
		return_press = set_press + (0.2 * set_press);
	else if(ins_stage == Ins_End)
		return_press = set_press;

	return return_press;
}

/*!
	@brief 		 Expiration stage process function.
	@param[in]	 set_press:Current pressure.
				 therapy_mode:Therapy mode.
				 ex_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_Expiration_Stage(unsigned short set_press,eBreathe_Stage ex_stage){
    uint16_t return_press = set_press;
	if(ex_stage == Ex_Start)
		return_press = Mid_EPR_Function(set_press);
	else if(ex_stage == Ex_End)
		return_press = set_press;


	return return_press;
}

