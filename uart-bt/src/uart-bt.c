#include "LPC17xx.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_uart.h"
#include <stdio.h>


// packed es para que lo guarde de forma contigua en la memoria
struct mando {
  volatile uint8_t gatillo;
  volatile uint8_t joystick;
  volatile uint8_t botones;
} __attribute__((packed));

// Puntero constante a la dirección física de inicio de AHBSRAM1 (0x20080000)
volatile struct mando *mando_data = (volatile struct mando *)0x20080000;

GPDMA_LLI_T uart_lli;

GPDMA_Channel_CFG_T dma_cfg;

void conf_uart0(void);
void conf_dma(void);

int main(void) {
  // Inicializar los campos de la estructura en AHBSRAM1 usando el puntero
  mando_data->gatillo = 0;
  mando_data->joystick = 0;
  mando_data->botones = 0;

  conf_uart0();
  conf_dma();

  printf("Comunicacion con joystick empezada... \n");

  while (1) {

    printf("botones: %d\n", mando_data->botones);
    printf("gatillo: %d\n", mando_data->gatillo);
    printf("joystick: %d\n", mando_data->joystick);
  }
}

void conf_uart0(void) {
  UART_PinConfig(UART_TX0_P0_2);
  UART_PinConfig(UART_RX0_P0_3);

  UART_CFG_T confuart0 = {
      .baudRate = 115200,
      .parity = UART_PARITY_NONE,
      .dataBits = UART_DBITS_8,
      .stopBits = UART_STOPBIT_1,
  };

  UART_Init(UART0, &confuart0);

  UART_FIFO_CFG_T conf_fifo = {.resetRxBuf = ENABLE,
                               .resetTxBuf = ENABLE,
                               .dmaMode = ENABLE,
                               .level = UART_FIFO_TRGLEV0};

  UART_FIFOConfig(UART0, &conf_fifo);
}

void conf_dma(void) {
  GPDMA_Init();

  uart_lli.srcAddr = (uint32_t)&LPC_UART0->RBR;
  uart_lli.dstAddr = (uint32_t)mando_data;
  uart_lli.nextLLI = (uint32_t)&uart_lli;
  uart_lli.control = 3 | (1 << 27);

  dma_cfg.channelNum = GPDMA_CH_0;
  dma_cfg.transferSize = 3;
  dma_cfg.type = GPDMA_P2M;
  dma_cfg.srcMemAddr = 0;
  dma_cfg.dstMemAddr = (uint32_t)mando_data;
  dma_cfg.srcConn = GPDMA_UART0_Rx;
  dma_cfg.dstConn = GPDMA_UART0_Tx;

  dma_cfg.src.width = GPDMA_BYTE;
  dma_cfg.src.burst = GPDMA_BSIZE_1;
  dma_cfg.src.increment = DISABLE;

  dma_cfg.dst.width = GPDMA_BYTE;
  dma_cfg.dst.burst = GPDMA_BSIZE_1;
  dma_cfg.dst.increment = ENABLE;

  dma_cfg.intTC = ENABLE;
  dma_cfg.intErr = ENABLE;

  dma_cfg.linkedList = (uint32_t)&uart_lli;

  GPDMA_SetupChannel(&dma_cfg);
  GPDMA_ChannelStart(GPDMA_CH_0);
}
