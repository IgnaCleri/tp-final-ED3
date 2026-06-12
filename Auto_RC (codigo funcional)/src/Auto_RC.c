#include "lpc17xx_gpio.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "motores.h"
#include "timer0.h"
#include "uart.h"
#include <stdio.h>

uint8_t posicion_actual = 0;
uint8_t gatillo_actual = 0;
uint8_t posicion_anterior = 0;
uint8_t gatillo_anterior = 0;
uint8_t boton_actual = 0;
uint8_t boton_anterior = 0;

volatile uint32_t *ADC_VALUE = (volatile uint32_t *)ADC_VALUE_ADDR;

int main(void)
{

    DAC_GenerarSenoidal();
    conf_uart0();
    conf_dma();
    pines_motores();
    conf_motores();
    DAC_ConfigurarSalidaDMA();
    DMA_ConfigurarSirenaDAC();
    TIMER0_ConfigurarSirena();
    ADC_Config();
    ADC_dma();

    while (1)
    {
        posicion_actual = mando_data->joystick;
        gatillo_actual = mando_data->gatillo;
        boton_actual = mando_data->botones;

        if (posicion_actual != posicion_anterior)
        {
            mcpwmCfg.channelPulsewidthValue = calcular_ticks_motores(posicion_actual);
            MCPWM_WriteToShadow(LPC_MCPWM, 0, &mcpwmCfg);
            posicion_anterior = posicion_actual;
        }

        if (gatillo_actual != gatillo_anterior)
        {
            mcpwmCfg.channelPulsewidthValue = calcular_ticks_brush(gatillo_actual);
            MCPWM_WriteToShadow(LPC_MCPWM, 1, &mcpwmCfg);
            gatillo_anterior = gatillo_actual;
        }

        if ((boton_actual & 0b01) == 1)
        {

            toggle_dac();
        }

        if (((*ADC_VALUE >> 4) & 0xFFF) > 400)
        {
            GPIO_SetPins(0, (1 << 22));
        }
        else
        {
            GPIO_ClearPins(0, (1 << 22));
        }
    }
}
