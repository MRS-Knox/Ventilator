#ifndef Driver_RGB_H
#define Driver_RGB_H

#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "FreeRTOS.h"
#include "task.h"

#define SPI2_MOSI_Pin		GPIO_Pin_3
#define SPI2MOSI_Group		GPIOC
#define SPI2_NSS_Pin		GPIO_Pin_12
#define SPI2_SCK_Pin		GPIO_Pin_13
#define SPI2SCKNSS_Group	GPIOB
#define Code_1				0xFE
#define Code_0				0x80


/* This initial function is about RGB light hardware. */
void Dri_RGBHardware_Init(void);
/* This function is about main chip send data to WS2812b. */
ErrorStatus Dri_SPI_Send(uint16_t data);
/* This function is about main chip send data to WS2812b. */
void Dri_SendDataTo_WS2812B(uint8_t rgb_count,uint8_t (*prgb_depth)[3]);

#endif
