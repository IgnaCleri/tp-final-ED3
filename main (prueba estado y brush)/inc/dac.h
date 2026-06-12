#ifndef DAC_H_
#define DAC_H_

#include <stdint.h>

void DAC_ConfigurarSalidaDMA(void);
void DAC_GenerarSenoidal(void);
void DAC_ActualizarDelay(uint16_t delay);

uint16_t *DAC_ObtenerBufferSirena(void);
uint32_t DAC_ObtenerCantidadMuestrasSirena(void);

#endif /* DAC_H_ */
