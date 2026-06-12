#ifndef UART_H_
#define UART_H_

#include <stdint.h>

// packed es para que lo guarde de forma contigua en la memoria
struct mando
{
    volatile uint8_t gatillo;
    volatile uint8_t joystick;
    volatile uint8_t botones;
} __attribute__((packed));

extern volatile struct mando *mando_data;

void conf_uart0(void);
void conf_dma(void);

#endif /* UART_H_ */
