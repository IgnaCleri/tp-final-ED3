#include "dma.h"

#include <stdint.h>

#include "LPC17xx.h"
#include "dac.h"
#include "lpc17xx_gpdma.h"

static GPDMA_LLI_T lliSenoidal;

static uint32_t lli_control(uint32_t transferSize)
{
    return GPDMA_DMACCxControl_TransferSize(transferSize) |
           GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_1) |
           GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_1) |
           GPDMA_DMACCxControl_SWidth(GPDMA_HALFWORD) |
           GPDMA_DMACCxControl_DWidth(GPDMA_HALFWORD) |
           GPDMA_DMACCxControl_SI;
}

void DMA_ConfigurarSirenaDAC(void)
{
    uint16_t *vectorDAC = DAC_ObtenerBufferSirena();
    uint32_t cantidadMuestras = DAC_ObtenerCantidadMuestrasSirena();
    GPDMA_Channel_CFG_T conDMA = {0};

    lliSenoidal.srcAddr = (uint32_t)vectorDAC;
    lliSenoidal.dstAddr = (uint32_t)&(LPC_DAC->DACR);
    lliSenoidal.nextLLI = (uint32_t)&lliSenoidal;
    lliSenoidal.control = lli_control(cantidadMuestras);

    conDMA.channelNum = GPDMA_CH_3;
    conDMA.transferSize = cantidadMuestras;
    conDMA.type = GPDMA_M2P;
    conDMA.srcMemAddr = (uint32_t)vectorDAC;
    conDMA.dstMemAddr = (uint32_t)&(LPC_DAC->DACR);
    conDMA.srcConn = 0;
    conDMA.dstConn = GPDMA_DAC;
    conDMA.src.width = GPDMA_HALFWORD;
    conDMA.src.burst = GPDMA_BSIZE_1;
    conDMA.src.increment = ENABLE;
    conDMA.dst.width = GPDMA_HALFWORD;
    conDMA.dst.burst = GPDMA_BSIZE_1;
    conDMA.dst.increment = DISABLE;
    conDMA.intErr = ENABLE;
    conDMA.intTC = DISABLE;
    conDMA.linkedList = (uint32_t)&lliSenoidal;

    GPDMA_SetupChannel(&conDMA);
    GPDMA_ChannelStart(GPDMA_CH_3);
}
