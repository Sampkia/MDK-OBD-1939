#ifndef __WDG_H
#define __WDG_H

#include "stm32f10x.h"

void IWDGInit(uint8_t prer, uint16_t rlr);
void IWDGFeed(void);

#endif  /* _WDG_H */
