#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#define PERIOD_TICKS 20000U

void servo_pines(void);
void conf_servo(void);
uint32_t calcular_ticks_servo(uint8_t valor_entrada);
void servo_actualizar(uint8_t valor_entrada);

#endif /* SERVO_H_ */
