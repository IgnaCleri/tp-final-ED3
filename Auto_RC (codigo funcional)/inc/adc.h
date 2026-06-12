#ifndef ADC_H_
#define ADC_H_

#include "LPC17xx.h"
#include <stdint.h>

#define ADC_VALUE_ADDR 0x20080010

void ADC_Config(void);
void ADC_dma(void);

#endif /* ADC_H_ */
