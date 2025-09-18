#ifndef Driver_Flash_H
#define Driver_Flash_H

#include "stm32f4xx_conf.h"
#include "Use_Struct.h"

#define CalDataPressStart_Address		0x08060000
#define CalDataFlowKStart_Address		(CalDataPressStart_Address+(CALPRESSMaxCount*sizeof(unsigned short)))
#define CalDataFlowStart_Address		(CalDataFlowKStart_Address+(CALFlow_KMaxCount*sizeof(unsigned short)))
#define CalDataPRESSCOMStart_Address	(CalDataFlowStart_Address+(CALFlowMaxCount*sizeof(unsigned int)))
#define CalDataRTCStart_Address         (CalDataPRESSCOMStart_Address+(1*sizeof(unsigned short)))
#define CalDataModelStart_Address       (CalDataRTCStart_Address+(1*sizeof(unsigned short)))


/* This initial function is about flash hardware. */
void Dri_FlashHardware_Init(void);
/* This function is about to write data to flash. */
ErrorStatus Dri_Flash_Write(CalibrationData_t *data_t);
/* This function is about to read data from flash. */
uint16_t Dri_Flash_Read(uint32_t read_address);


#endif

