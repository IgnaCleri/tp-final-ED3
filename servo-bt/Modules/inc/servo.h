#ifndef SERVO_H_
#define SERVO_H_

#include "LPC17xx.h"
#include "lpc17xx_mcpwm.h"
#include <stdint.h>

#define PERIOD_TICKS 500000

extern MCPWM_CHANNEL_CFG_Type mcpwmCfg;

/**
 * @brief Configura los pines necesarios para el MCPWM del servo
 */
void servo_pines(void);

/**
 * @brief Configura el periférico MCPWM para controlar el servo
 */
void conf_servo(void);

/**
 * @brief Calcula los ticks necesarios para el ancho de pulso según el valor de entrada
 * @param valor_entrada Valor de 0 a 255
 * @return Valor en ticks para el registro de ancho de pulso
 */
uint32_t calcular_ticks_servo(uint8_t valor_entrada);

#endif /* SERVO_H_ */
