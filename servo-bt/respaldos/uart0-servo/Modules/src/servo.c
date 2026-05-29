#include "servo.h"
#include "lpc17xx_mcpwm.h"
#include "lpc17xx_pinsel.h"

MCPWM_CHANNEL_CFG_Type mcpwmCfg = {0};

void servo_pines(void) {
    PINSEL_CFG_T pinCfg;

    pinCfg.port = PORT_1;
    pinCfg.pin = PIN_19;
    pinCfg.func = PINSEL_FUNC_01;
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = DISABLE;

    PINSEL_ConfigPin(&pinCfg);
}

void conf_servo(void) {
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

uint32_t calcular_ticks_servo(uint8_t valor_entrada) {
    uint32_t duty_escalado = 1250 - (((uint32_t)valor_entrada * 1000) / 255);

    return (PERIOD_TICKS * duty_escalado) / 10000;
}
