#ifndef MOTORES_H_
#define MOTORES_H_

#include <stdint.h>
#include "lpc17xx_mcpwm.h"

#define PERIOD_TICKS 500000
#define MOTOR_DEADZONE 5

extern MCPWM_CHANNEL_CFG_Type mcpwmCfg;

uint32_t calcular_ticks_motores(uint8_t valor_entrada);
uint32_t calcular_ticks_brush(uint8_t valor_entrada);
void conf_motores(void);
void pines_motores(void);

#endif /* MOTORES_H_ */
