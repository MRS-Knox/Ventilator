#ifndef Driver_RGB_H
#define Driver_RGB_H

#include "stm32f4xx_conf.h"

typedef enum{
    R_Light = 0x01,
    G_Light	= 0x02,
    B_Light	= 0x04,
}eRGB_Light;

#define StartRGB_Group		GPIOC
#define StartR_Pin			GPIO_Pin_2
#define StartG_Pin  		GPIO_Pin_0
#define StartB_Pin			GPIO_Pin_1
#define BlueRGB_Group		GPIOB
#define BlueR_Pin			GPIO_Pin_15
#define BlueG_Pin			GPIO_Pin_13
#define BlueB_Pin			GPIO_Pin_12

/* Initialize RGB light hardware. */
void Dri_RGBHardware_Init(void);
/* Control RGB light. */
void Dri_Control_StartRGB(uint8_t light_color,FlagStatus gpio_level);
void Dri_Control_BlueRGB(uint8_t light_color,FlagStatus gpio_level);

#endif


