#include "LPC17xx.h"
#include <stdio.h>
#include "uart.h"
#include "servo.h"

volatile struct mando *mando_data = (volatile struct mando *)MANDO_DATA_ADDR;

uint8_t posicion_actual = 0;

int main(void) {

    mando_data->gatillo = 0;
    mando_data->joystick = 0;
    mando_data->botones = 0;

    conf_uart0();
    dma_uart();
    servo_pines();
    conf_servo();

    while (1) {

        posicion_actual = mando_data->joystick;
        mcpwmCfg.channelPulsewidthValue = calcular_ticks_servo(posicion_actual);
        MCPWM_ConfigChannel(LPC_MCPWM, 0, &mcpwmCfg);
    }
}
