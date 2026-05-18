# Universidad Nacional de Córdoba
## Facultad de Ciencias Exactas, Físicas y Naturales
### Electrónica Digital 3

---

# Informe de Trabajo Práctico N° [Número]
**Título:** Implementación de un Vehículo Terrestre Controlado Mediante Microcontrolador LPC1769

**Fecha:** 18 de mayo de 2026
**Integrantes:**
- Juan Ignacio Pineda - DNI 45.591.343
- Juan Ignacio Cleri - DNI 46.452.662
- Aaron Alejandro Espinoza Sutta - DNI 96.009.173

---

## 1. Introducción
El presente trabajo práctico describe el diseño y la implementación de un vehículo a control remoto basado en el microcontrolador LPC1769 (Cortex-M3). El sistema integra diversos periféricos para lograr una plataforma móvil funcional, incluyendo el control de dirección mediante un servomotor, tracción a través de un motor de corriente continua (DC), sensado de condiciones lumínicas ambientales y una interfaz de alerta sonora (bocina). Se pone énfasis en el manejo de registros, configuración de pines y el uso de protocolos de control como PWM para la gestión de actuadores.

## 2. Objetivos
- Desarrollar un sistema embebido robusto utilizando el microcontrolador LPC1769.
- Implementar el control de posición de un servomotor para el sistema de dirección mediante señales PWM.
- Gestionar la velocidad y sentido de giro de un motor DC para la propulsión del vehículo.
- Integrar un sensor de luz para la adquisición de datos ambientales y respuesta automática del sistema.
- Configurar una salida digital/PWM para el accionamiento de una bocina como alerta sonora.

## 3. Materiales y Métodos
### 3.1. Equipamiento y Asignación de Periféricos
Para la interacción con el hardware, se utilizaron los siguientes módulos internos del LPC1769:

- **Comunicación Bluetooth (HC-05):** Módulo **UART** (Universal Asynchronous Receiver-Transmitter). Se utiliza para la recepción de tramas de control provenientes del mando de PC.
- **Control de Dirección (MG90S):** Módulo **PWM** (Pulse Width Modulation). Generación de pulsos de 50Hz con duty cycle variable para el posicionamiento del servo.
- **Tracción (Motor N20):** Módulo **PWM** y **GPIO**. El PWM controla la velocidad mediante un puente H, mientras que los pines GPIO definen el sentido de giro.
- **Lectura de Velocidad (Encoder):** Módulo **GPIO con Interrupciones Externas** (o QEI). Captura de pulsos para el cálculo de RPM y distancia.
- **Sensor de Luz (LDR):** Módulo **ADC** (Analog to Digital Converter). Conversión de la señal analógica de tensión en un valor digital para procesar la intensidad lumínica.
- **Sistema de Iluminación (LEDs):** Módulos **GPIO**. Salidas digitales para el encendido y apagado de las luces frontales/traseras.
- **Bocina (Buzzer 5V):** Módulo **GPIO / PWM**. Accionamiento de la alerta sonora mediante una señal digital o tono de frecuencia fija.

### 3.2. Procedimiento
[Descripción de los pasos realizados durante la práctica.]

## 4. Desarrollo
### 4.1. Diseño / Simulación
[Incluir aquí detalles del diseño, cálculos y resultados de simulaciones si aplica.]

### 4.2. Implementación
[Detalles del montaje y pruebas realizadas.]

## 5. Resultados
[Gráficos, tablas de datos, capturas de pantalla o fotos del funcionamiento.]

## 6. Conclusión
[Análisis de los resultados obtenidos y reflexiones finales.]

## 7. Bibliografía
- [Referencia 1]
- [Referencia 2]
