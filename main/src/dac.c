#include "dac.h"

#include <math.h>

#include "lpc17xx_dac.h"

static uint16_t * const vectorDAC = (uint16_t *)0x2007C000UL;

void DAC_ConfigurarSalidaDMA(void)
{
    DAC_CONVERTER_CFG_T dacConfig = {0};

    dacConfig.doubleBuffer = DISABLE;
    dacConfig.dmaRequest = ENABLE;
    dacConfig.dmaCounter = ENABLE;

    DAC_Init();
    DAC_SetBias(DAC_700uA);
    DAC_UpdateValue(0);
    DAC_SetDMATimeOut(244);
    DAC_ConfigDAConverterControl(&dacConfig);
}

void DAC_GenerarSenoidal(void)
{
    const float dosPi = 2.0f * acosf(-1.0f);
    int i;

    for (i = 0; i < 128; i++) {
        float angulo = (dosPi * i) / 128.0f;
        uint32_t muestra = (uint32_t)(512.0f + (511.0f * sinf(angulo)));

        if (muestra > 1023U) {
            muestra = 1023U;
        }

        vectorDAC[i] = (uint16_t)(muestra << 6);
    }
}

void DAC_ActualizarDelay(uint16_t delay)
{
    DAC_SetDMATimeOut(delay);
}

uint16_t *DAC_ObtenerBufferSirena(void)
{
    return vectorDAC;
}

uint32_t DAC_ObtenerCantidadMuestrasSirena(void)
{
    return 128U;
}
