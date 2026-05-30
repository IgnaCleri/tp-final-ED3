#include "uart.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_gpdma.h"

// Estructuras de configuración para DMA
GPDMA_LLI_T uart_lli;
GPDMA_Channel_CFG_T dma_cfg;

void conf_uart2(void) {
    // Configuración de pines para UART2 (P0.10 TX, P0.11 RX)
    UART_PinConfig(UART_TX2_P0_10);
    UART_PinConfig(UART_RX2_P0_11);

    UART_CFG_T confuart2 = {
        .baudRate = 115200,
        .parity = UART_PARITY_NONE,
        .dataBits = UART_DBITS_8,
        .stopBits = UART_STOPBIT_1,
    };

    UART_Init(LPC_UART2, &confuart2);

    UART_FIFO_CFG_T conf_fifo = {
        .resetRxBuf = ENABLE, .resetTxBuf = ENABLE, .dmaMode = ENABLE, .level = UART_FIFO_TRGLEV0};

    UART_FIFOConfig(LPC_UART2, &conf_fifo);
}

void dma_uart(void) {
    GPDMA_Init();

    uart_lli.srcAddr = (uint32_t)&LPC_UART2->RBR;
    uart_lli.dstAddr = (uint32_t)MANDO_DATA_ADDR;
    uart_lli.nextLLI = (uint32_t)&uart_lli;
    uart_lli.control = 3 | (1 << 27);

    dma_cfg.channelNum = GPDMA_CH_0;
    dma_cfg.transferSize = 3;
    dma_cfg.type = GPDMA_P2M;
    dma_cfg.srcMemAddr = 0;
    dma_cfg.dstMemAddr = (uint32_t)MANDO_DATA_ADDR;
    dma_cfg.srcConn = GPDMA_UART2_Rx;
    dma_cfg.dstConn = 0; // No usado para P2M
    dma_cfg.src.width = GPDMA_BYTE;
    dma_cfg.src.burst = GPDMA_BSIZE_1;
    dma_cfg.src.increment = DISABLE;
    dma_cfg.dst.width = GPDMA_BYTE;
    dma_cfg.dst.burst = GPDMA_BSIZE_1;
    dma_cfg.dst.increment = ENABLE;
    dma_cfg.intTC = DISABLE;
    dma_cfg.intErr = DISABLE;
    dma_cfg.linkedList = (uint32_t)&uart_lli;

    GPDMA_SetupChannel(&dma_cfg);
    GPDMA_ChannelStart(GPDMA_CH_0);
}
