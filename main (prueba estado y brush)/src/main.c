#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "motores.h"
#include "timer0.h"
#include "uart.h"
#include <stdio.h>

typedef enum
{
    STATE_INICIO,
    STATE_FAILSAFE_MOTOR,
    STATE_MAIN
} estados_t;

volatile uint32_t ms_ticks = 0;
uint8_t gatillo_actual = 0;
uint8_t gatillo_anterior = 0;

void SysTick_Handler(void) { ms_ticks++; }

int main(void)
{
    pines_motores();
    conf_motores();

    mando_data->gatillo = 0;
    mando_data->joystick = 0;
    mando_data->botones = 0;

    DAC_GenerarSenoidal();
    conf_uart0();
    conf_dma();
    DAC_ConfigurarSalidaDMA();
    DMA_ConfigurarSirenaDAC();
    TIMER0_ConfigurarSirena();

    // Inicializar SysTick para interrupciones cada 1ms
    SYSTICK_InternalInit(1);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);

    estados_t estado = STATE_BOOT;
    uint32_t start_failsafe_tick = 0;

    printf("aca empieza el codigo - Esperando activacion (Boton Bit 2)...\n");

    while (1)
    {
        switch (estado)
        {
        case STATE_INICIO:
            // Mantener PWM al minimo (Failsafe)
            mcpwmCfg.channelPulsewidthValue = calcular_ticks_brush(0);
            MCPWM_WriteToShadow(LPC_MCPWM, 1, &mcpwmCfg);

            // Esperar a que se presione el boton del bit 2 (valor 4)
            // se necesita encender el motor en esta etapa
            if (mando_data->botones & (1 << 2))
            {
                estado = STATE_WAIT_FAILSAFE;
                start_failsafe_tick = ms_ticks;
            }
            break;
        case STATE_FAILSAFE_MOTOR:

            // Failsafe para que el motor no se desincronice
            mcpwmCfg.channelPulsewidthValue = calcular_ticks_brush(0);
            MCPWM_WriteToShadow(LPC_MCPWM, 1, &mcpwmCfg);

            // timer como si fuese el milis de esp32
            if ((ms_ticks - start_failsafe_tick) >= 2000)
            {

                estado = STATE_MAIN;
                SYSTICK_Cmd(DISABLE);
                SYSTICK_IntCmd(DISABLE);
            }

            break;
        case STATE_MAIN:

            // ciclo general, por ahora solo para probar el motor
            gatillo_actual = mando_data->gatillo;

            if (gatillo_actual != gatillo_anterior)
            {
                mcpwmCfg.channelPulsewidthValue = calcular_ticks_brush(gatillo_actual);
                MCPWM_WriteToShadow(LPC_MCPWM, 1, &mcpwmCfg);
                gatillo_anterior = gatillo_actual;
            }
            break;
        }
    }
}
