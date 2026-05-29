#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define MANDO_DATA_ADDR 0x20080000UL

typedef struct mando {
    uint8_t gatillo;
    uint8_t joystick;
    uint8_t botones;
} mando_t;

void conf_uart2(void);
void dma_uart(void);

#endif /* UART_H_ */
