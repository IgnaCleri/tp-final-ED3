#include "adc.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpdma.h"

GPDMA_LLI_T adc_lli;
GPDMA_Channel_CFG_T dma_adc;

void ADC_Config(void)
{
    PINSEL_CFG_T PinCfg;
    PinCfg.port = 0;
    PinCfg.pin = 22;
    PinCfg.func = PINSEL_FUNC_00;
    PinCfg.mode = PINSEL_PULLUP;
    PinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&PinCfg);

    GPIO_SetDir(0, (1 << 22), 1);
    GPIO_ClearPins(0, (1 << 22));

    ADC_Init(200000);
    ADC_PinConfig(ADC_CHANNEL_0);
    ADC_ChannelEnable(ADC_CHANNEL_0);
    // ADC_IntEnable(ADC_INT_CH0); // Removido para no colgar el sistema ya que usamos DMA
    ADC_BurstEnable();
    ADC_PowerUp();
    // NVIC_EnableIRQ(ADC_IRQn); // Removido
}

void ADC_dma(void)
{
    adc_lli.srcAddr = (uint32_t)&(LPC_ADC->ADGDR);
    adc_lli.dstAddr = ADC_VALUE_ADDR;
    adc_lli.nextLLI = (uint32_t)&adc_lli;
    // control = transferSize(1) | src_width(word:2) | dst_width(word:2)
    // Incrementos desactivados
    adc_lli.control = 1 | (2 << 18) | (2 << 21);

    dma_adc.channelNum = GPDMA_CH_1;
    dma_adc.transferSize = 1;
    dma_adc.type = GPDMA_P2M;
    dma_adc.srcMemAddr = 0;
    dma_adc.dstMemAddr = ADC_VALUE_ADDR;
    dma_adc.srcConn = GPDMA_ADC;
    dma_adc.dstConn = 0;
    dma_adc.src.width = GPDMA_WORD;
    dma_adc.src.burst = GPDMA_BSIZE_1;
    dma_adc.src.increment = DISABLE;
    dma_adc.dst.width = GPDMA_WORD;
    dma_adc.dst.burst = GPDMA_BSIZE_1;
    dma_adc.dst.increment = DISABLE;
    dma_adc.intTC = DISABLE;
    dma_adc.intErr = DISABLE;
    dma_adc.linkedList = (uint32_t)&adc_lli;

    GPDMA_SetupChannel(&dma_adc);
    GPDMA_ChannelStart(GPDMA_CH_1);
}
