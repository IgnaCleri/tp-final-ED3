# Universidad Nacional de Córdoba
## Facultad de Ciencias Exactas, Físicas y Naturales
### Electrónica Digital 3

---

# Informe de Trabajo Práctico Final
**Título:** Implementación de un Vehículo Terrestre Controlado Mediante Microcontrolador LPC1769

**Fecha:** 18 de mayo de 2026
**Integrantes:**
- Juan Ignacio Pineda - DNI 45.591.343
- Juan Ignacio Cleri - DNI 46.452.662
- Aaron Alejandro Espinoza Sutta - DNI 96.009.173

---

## 1. Introducción
El presente trabajo práctico describe el diseño y la implementación de un vehículo a control remoto basado en el microcontrolador LPC1769 (Cortex-M3). El sistema integra diversos periféricos para lograr una plataforma móvil funcional, incluyendo el control de dirección mediante un servomotor, tracción a través de un motor brushless (sin escobillas), sensado de condiciones lumínicas ambientales y una interfaz de alerta sonora (bocina). Se pone énfasis en el manejo de registros, configuración de pines y el uso de protocolos de control como PWM para la gestión de actuadores y controladores de velocidad (ESC).

## 2. Objetivos
- Desarrollar un sistema embebido robusto utilizando el microcontrolador LPC1769.
- Implementar el control de posición de un servomotor para el sistema de dirección mediante señales PWM.
- Gestionar la velocidad de un motor brushless mediante un controlador electrónico de velocidad (ESC) utilizando señales PWM.
- Integrar un sensor de luz para la adquisición de datos ambientales y respuesta automática del sistema.
- Configurar una salida digital/PWM para el accionamiento de una bocina como alerta sonora.

## 3. Materiales y Métodos
### 3.1. Equipamiento y Asignación de Periféricos
Para la interacción con el hardware, se utilizaran los siguientes módulos internos del LPC1769:

- **Comunicación Bluetooth (HC-05):** Módulo **UART** (Universal Asynchronous Receiver-Transmitter). Se utiliza para la recepción de tramas de control provenientes del mando de PC.
- **Control de Dirección (MG90S):** Módulo **PWM** y **GPIO**. Se emplea un servomotor para el posicionamiento de la dirección, el cual es gestionado a través de un controlador para motores de corriente continua (Driver), permitiendo una etapa de potencia adecuada para el actuador.
- **Tracción (Motor Brushless A2212 1400KV):** Módulo **PWM**. El control se realiza mediante un controlador electrónico de velocidad (ESC), el cual interpreta señales PWM de 50Hz para gestionar la potencia del motor trifásico.
- **Sensor de Luz (LDR):** Módulo **ADC** (Analog to Digital Converter). Conversión de la señal analógica de tensión en un valor digital para procesar la intensidad lumínica.
- **Sistema de Iluminación (LEDs):** Módulos **GPIO**. Salidas digitales para el encendido y apagado de las luces frontales/traseras.
- **Bocina (Buzzer 5V):** Módulo **GPIO / PWM**. Accionamiento de la alerta sonora mediante una señal digital o tono de frecuencia fija.
- **Sistema de Alimentación:** Baterías de Polímero de Litio (LiPo) conectadas en serie (celdas de 3,7V nominales), proporcionando la tensión necesaria para la etapa de potencia y lógica del sistema.
- **Diseño Mecánico:** Chasis de escala 1/10 obtenido del proyecto de hardware abierto ["Chassis 1/10 Adaptable DKS Basic"](https://www.printables.com/model/447947-chassis-110-adaptable-dks-basic), adaptado para alojar los componentes electrónicos y sistemas de propulsión descritos.

### 3.2. Procedimiento
El desarrollo del proyecto se dividira en las siguientes etapas consecutivas:

#### 3.2.1. Fabricación del Chasis y Montaje Mecánico
La estructura física del vehículo se realizara mediante impresión 3D, siguiendo estos pasos:
1. **Obtención de Archivos:** Se descargaron los modelos STL del proyecto ["Chassis 1/10 Adaptable DKS Basic"](https://www.printables.com/model/447947-chassis-110-adaptable-dks-basic) desde la sección de archivos de la plataforma Printables.
2. **Laminado e Impresión:** Se procesaron las piezas en un software de laminado (Slicer) ajustando parámetros de relleno (infill) y soportes para garantizar la resistencia mecánica necesaria para un modelo escala 1/10.
3. **Ensamblaje:** Se procedió al montaje de la estructura principal, instalando los ejes, el sistema de dirección y los alojamientos para el motor brushless y el servomotor MG90S.

#### 3.2.2. Integración Electrónica y Cableado
Una vez consolidada la estructura mecánica, se realizaron las conexiones siguiendo el esquema de periféricos:
1. **Etapa de Potencia:** Conexión de las baterías LiPo en serie al ESC para el motor brushless y al controlador de motores DC para el servomotor de dirección.
2. **Control Central:** Vinculación de las salidas PWM de la LPC1769 a los controladores de los motores y los pines UART al módulo HC-05.
3. **Sensores y Alertas:** Montaje del sensor LDR en la parte frontal y conexión del buzzer y LEDs indicadores.

#### 3.2.3. Programación y Calibración
(Pendiente de completar con detalles del firmware)

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
