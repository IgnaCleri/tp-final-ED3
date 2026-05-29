#include "timer0.h"

#include <stdint.h>

#include "dac.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"

static uint16_t delaySirena[200];
static uint32_t indiceDelay = 0;

static void TIMER0_GenerarBufferDelay(void)
{
    const float pclkDac = 25000000.0f;
    int i;

    for (i = 0; i < 200; i++) {
        float progreso = (float)i / (200 - 1);
        float barrido = (progreso < 0.5f) ? (2.0f * progreso) : (2.0f * (1.0f - progreso));
        float frecuencia = 800.0f + (barrido * (2000.0f - 800.0f));
        float delayFloat = pclkDac / (128.0f * frecuencia);
        uint32_t delay = (uint32_t)(delayFloat + 0.5f);

        if (delay > 65535U) {
            delay = 65535U;
        }

        delaySirena[i] = (uint16_t)delay;
    }
}

void TIMER0_ConfigurarSirena(void)
{
    TIM_TIMERCFG_T timerCfg = {0};
    TIM_MATCHCFG_T matchCfg = {0};

    TIMER0_GenerarBufferDelay();
    DAC_ActualizarDelay(delaySirena[0]);

    timerCfg.prescaleOpt = TIM_US;
    timerCfg.prescaleValue = 1;
    TIM_InitTimer(LPC_TIM0, &timerCfg);

    matchCfg.channel = TIM_MATCH_0;
    matchCfg.intEn = ENABLE;
    matchCfg.stopEn = DISABLE;
    matchCfg.resetEn = ENABLE;
    matchCfg.extOpt = TIM_NOTHING;
    matchCfg.matchValue = 5000;
    TIM_ConfigMatch(LPC_TIM0, &matchCfg);

    NVIC_EnableIRQ(TIMER0_IRQn);
    TIM_Enable(LPC_TIM0);
}

void TIMER0_IRQHandler(void)
{
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

    indiceDelay++;
    if (indiceDelay >= 200U) {
        indiceDelay = 0;
    }

    DAC_ActualizarDelay(delaySirena[indiceDelay]);
}
