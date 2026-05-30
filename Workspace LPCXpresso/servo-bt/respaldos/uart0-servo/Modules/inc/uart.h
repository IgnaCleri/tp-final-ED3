#ifndef UART_H_
#define UART_H_

#include "LPC17xx.h"
#include <stdint.h>

#define MANDO_DATA_ADDR 0x20080000

/**
 * @brief Estructura para los datos del mando (empaquetada para memoria contigua)
 */
struct mando {
    volatile uint8_t gatillo;
    volatile uint8_t joystick;
    volatile uint8_t botones;
} __attribute__((packed));

/**
 * @brief Configura el periférico UART0 para comunicación con el mando
 */
void conf_uart0(void);

/**
 * @brief Configura el canal DMA para la recepción de datos de UART0
 */
void dma_uart(void);

#endif /* UART_H_ */
