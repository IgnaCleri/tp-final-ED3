#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "servo.h"
#include "timer0.h"
#include "uart.h"

volatile mando_t *mando_data = (volatile mando_t *)MANDO_DATA_ADDR;
uint8_t posicion_actual = 0;
uint8_t posicion_anterior = 0;

int main(void)
{
    mando_data->gatillo = 0;
    mando_data->joystick = 0;
    mando_data->botones = 0;

    DAC_GenerarSenoidal();
    conf_uart2();
    dma_uart();
    servo_pines();
    conf_servo();
    DAC_ConfigurarSalidaDMA();
    DMA_ConfigurarSirenaDAC();
    TIMER0_ConfigurarSirena();
    ADC_Config();

    while (1)
    {
        posicion_actual = mando_data->joystick;
        if (posicion_actual != posicion_anterior)
        {
            posicion_anterior = posicion_actual;
            servo_actualizar(posicion_actual);
        }
    }
}
