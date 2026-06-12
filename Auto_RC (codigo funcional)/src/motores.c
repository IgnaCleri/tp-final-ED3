#include "motores.h"
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"

MCPWM_CHANNEL_CFG_Type mcpwmCfg = {0};

uint32_t calcular_ticks_motores(uint8_t valor_entrada)
{
    if (valor_entrada < 55)
        valor_entrada = 55;

    if (valor_entrada > 195)
        valor_entrada = 195;

    uint32_t duty_escalado = 1250 - (((uint32_t)valor_entrada * 1000) / 255);
    return (PERIOD_TICKS * duty_escalado) / 10000;
}

uint32_t calcular_ticks_brush(uint8_t valor_entrada)
{

    if (valor_entrada > 120)
        valor_entrada = 120;

    uint32_t duty_escalado = 1250 - (((uint32_t)valor_entrada * 1000) / 255);
    return (PERIOD_TICKS * duty_escalado) / 10000;
}

void conf_motores(void)
{
    MCPWM_Init(LPC_MCPWM);

    // Configuracion comun
    mcpwmCfg.channelType = MCPWM_CHANNEL_EDGE_MODE;
    mcpwmCfg.channelPolarity = MCPWM_CHANNEL_PASSIVE_HI;
    mcpwmCfg.channelTimercounterValue = 0;
    mcpwmCfg.channelDeadtimeEnable = DISABLE;
    mcpwmCfg.channelDeadtimeValue = 0;
    mcpwmCfg.channelUpdateEnable = ENABLE;
    mcpwmCfg.channelPeriodValue = PERIOD_TICKS;

    // Configurar el canal 0 (Servo)
    mcpwmCfg.channelPulsewidthValue = (PERIOD_TICKS * 0.05);
    MCPWM_ConfigChannel(LPC_MCPWM, 0, &mcpwmCfg);

    // Configurar el canal 1 (Motor ESC)
    MCPWM_ConfigChannel(LPC_MCPWM, 1, &mcpwmCfg);

    // Iniciar el canal 0 y canal 1 al mismo tiempo
    MCPWM_Start(LPC_MCPWM, 1, 1, 0);
}

void pines_motores(void)
{
    PINSEL_CFG_T pinCfg;

    // Pin Servo (Canal 0) -> P1.19
    pinCfg.port = PORT_1;
    pinCfg.pin = PIN_19;
    pinCfg.func = PINSEL_FUNC_01;
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&pinCfg);

    // Pin Motor (Canal 1) -> P1.25
    pinCfg.pin = PIN_25;
    PINSEL_ConfigPin(&pinCfg);
}
