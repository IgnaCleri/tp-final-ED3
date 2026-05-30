#include "LPC17xx.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_mcpwm.h"
#include "lpc17xx_pinsel.h"
#include <stdio.h>

#define PERIOD_TICKS 500000

// packed es para que lo guarde de forma contigua en la memoria
struct mando {
  volatile uint8_t gatillo;
  volatile uint8_t joystick;
  volatile uint8_t botones;
} __attribute__((packed));

// Puntero constante a la dirección física de inicio de AHBSRAM1 (0x20080000)
volatile struct mando *mando_data = (volatile struct mando *)0x20080000;

MCPWM_CHANNEL_CFG_Type mcpwmCfg = {0};

GPDMA_LLI_T uart_lli;

GPDMA_Channel_CFG_T dma_cfg;

void conf_uart0(void);
void conf_dma(void);
void conf_pines(void);
void conf_mcpwm(void);
uint32_t calcular_ticks_mcpwm(uint8_t valor_entrada);
uint8_t posicion_actual = 0;

int main(void) {
  // Inicializar los campos de la estructura en AHBSRAM1 usando el puntero
  mando_data->gatillo = 0;
  mando_data->joystick = 0;
  mando_data->botones = 0;

  conf_uart0();
  conf_dma();
  conf_pines();
  conf_mcpwm();




  while (1)
  {


	  posicion_actual = mando_data->joystick;
	  mcpwmCfg.channelPulsewidthValue = calcular_ticks_mcpwm(posicion_actual);
	  MCPWM_ConfigChannel(LPC_MCPWM, 0, &mcpwmCfg);


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

uint32_t calcular_ticks_mcpwm(uint8_t valor_entrada)
{
    uint32_t duty_escalado = 1250 - (((uint32_t)valor_entrada * 1000) / 255);

    return (PERIOD_TICKS * duty_escalado) / 10000;
}

void conf_mcpwm(void)
{

    MCPWM_Init(LPC_MCPWM);

    mcpwmCfg.channelType = MCPWM_CHANNEL_EDGE_MODE;
    mcpwmCfg.channelPolarity = MCPWM_CHANNEL_PASSIVE_HI;
    mcpwmCfg.channelTimercounterValue = 0;
    mcpwmCfg.channelDeadtimeEnable = DISABLE;
    mcpwmCfg.channelDeadtimeValue = 0;
    mcpwmCfg.channelUpdateEnable = ENABLE;
    mcpwmCfg.channelPeriodValue = PERIOD_TICKS;
    mcpwmCfg.channelPulsewidthValue = (PERIOD_TICKS * 0.05);

    MCPWM_ConfigChannel(LPC_MCPWM, 0, &mcpwmCfg);

    MCPWM_Start(LPC_MCPWM, 1, 0, 0);


}

void conf_pines(void)
{
    PINSEL_CFG_T pinCfg;

    /* Configuración para P1.19 como MCOA0 */
    pinCfg.port = PORT_1;
    pinCfg.pin = PIN_19;
    pinCfg.func = PINSEL_FUNC_01;
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&pinCfg);
}
