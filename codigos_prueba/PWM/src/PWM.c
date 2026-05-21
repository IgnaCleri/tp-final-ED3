#include "lpc17xx.h"
#include "lpc17xx_mcpwm.h"
#include "lpc17xx_pinsel.h"

/* =============================================================
 * CONFIGURACIÓN DE PARÁMETROS
 * ============================================================= */
#define PWM_FREQ_HZ    20000   // Frecuencia deseada en Hz
#define PWM_DUTY_PERC  30      // Duty cycle deseado (0 a 100)
#define PCLK_MCPWM     25000000 // Frecuencia del PCLK (estándar 25MHz)

/* =============================================================
 * MACROS DE CÁLCULO
 * ============================================================= */
#define GET_PERIOD(freq)      (PCLK_MCPWM / freq)
#define GET_PULSE(duty, freq) ((GET_PERIOD(freq) * duty) / 100)

/**
 * @brief Configura el pin P1.19 para la función MCOA0
 */
void PinConfig_MCOA0(void) {
    PINSEL_CFG_T pinCfg;
    pinCfg.port = PORT_1;
    pinCfg.pin = PIN_19;
    pinCfg.func = PINSEL_FUNC_01; // Función MCPWM
    pinCfg.mode = PINSEL_TRISTATE;
    pinCfg.openDrain = DISABLE;
    PINSEL_ConfigPin(&pinCfg);
}

int main(void) {
    MCPWM_CHANNEL_CFG_Type mcpwmCfg;

    // 1. Configurar el pin físico
    PinConfig_MCOA0();

    // 2. Inicializar el periférico
    MCPWM_Init(LPC_MCPWM);

    // 3. Hard Reset de registros de control para evitar modos AC/DC/Deadtime
    // Usamos el registro MCCON_CLR de la estructura LPC_MCPWM
    LPC_MCPWM->MCCON_CLR = 0xFFFFFFFF;

    // 4. Configuración del Canal 0
    mcpwmCfg.channelType = MCPWM_CHANNEL_EDGE_MODE;      // Alineado al flanco
    mcpwmCfg.channelPolarity = MCPWM_CHANNEL_PASSIVE_HI; // Activo en ALTO

    // Usamos las macros para calcular los valores de los registros
    mcpwmCfg.channelPeriodValue = GET_PERIOD(PWM_FREQ_HZ);
    mcpwmCfg.channelPulsewidthValue = GET_PULSE(PWM_DUTY_PERC, PWM_FREQ_HZ);

    mcpwmCfg.channelTimercounterValue = 0;
    mcpwmCfg.channelDeadtimeEnable = DISABLE;
    mcpwmCfg.channelDeadtimeValue = 0;
    mcpwmCfg.channelUpdateEnable = ENABLE;

    // 5. Cargar configuración en Canal 0
    MCPWM_ConfigChannel(LPC_MCPWM, 0, &mcpwmCfg);

    // 6. Iniciar el Canal 0
    MCPWM_Start(LPC_MCPWM, 1, 0, 0);

    while(1) {
        // El PWM corre por hardware sin intervención de la CPU
    }
}
