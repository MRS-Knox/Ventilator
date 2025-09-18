#ifndef Middle_WatchDog_H
#define Middle_WatchDog_H

#include "stm32f4xx_conf.h"

/* This initial function is about IWG. */
void Mid_IWDG_Init(void);
/* Feed independent Watch Dog. */
void Mid_Feed_IWDG(void);

#endif
