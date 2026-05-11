# Taller 7: Comunicación Embebida - Protocolo SPI

Este repositorio contiene el código fuente y la documentación técnica correspondiente a la implementación de un Sistema de Control Industrial Distribuido utilizando el protocolo de comunicación síncrona SPI (Serial Peripheral Interface). 

El proyecto establece una arquitectura de red Maestro-Esclavo operando en modo Full-Duplex, empleando Controladores Lógicos Programables (PLCs) de la familia Controllino y placas de desarrollo de la familia AVR (Arduino).

---

## 1. Descripción del Proyecto

El sistema diseñado simula un proceso de control de temperatura industrial. Un dispositivo configurado como Maestro adquiere los valores analógicos de un sensor simulado (potenciómetro) y transmite esta información a través del bus SPI hacia dos nodos remotos actuadores (Esclavos). Estos nodos procesan la información y ejecutan rutinas de control en tiempo real mediante el uso de interrupciones de hardware.

### Lógica de Control Implementada

* **Esclavo 1 (Controllino Mega - Lógica Directa):** Configurado para la actuación principal. Si el valor térmico recibido supera el umbral crítico (> 150), activa una salida digital (HIGH) conectada a un actuador y envía un código de confirmación de estado `1` (Encendido) de regreso al Maestro. Si el valor es inferior, desactiva la salida y reporta `0`.
* **Esclavo 2 (Arduino UNO - Lógica Inversa):** Configurado para evaluar redundancia y respuestas asíncronas alternativas. Si el valor supera el umbral de `150`, reporta un estado lógico de apagado. Si el valor es inferior, reporta un estado de encendido.

Esta implementación destaca la manipulación de registros a bajo nivel (`SPCR`, `SPDR`) y la configuración de la Rutina de Servicio de Interrupción (`ISR`). Esto garantiza tiempos de respuesta deterministas sin interferir con el tiempo de ciclo (scan time) de los microcontroladores.

---

## 2. Requerimientos del Sistema

### Hardware
* 1x PLC Controllino Mega (Unidad Terminal Remota - Esclavo 1).
* 1x Placa de desarrollo Arduino UNO (Unidad Terminal Remota - Esclavo 2).
* 1x Dispositivo Maestro (Controllino Mega o Arduino).
* 1x Potenciómetro (Actuando como transductor analógico).
* Protoboard y cableado de conexión estándar.

### Software
* Entorno de desarrollo IDE de Arduino (v1.8.x o superior).
* Librería estándar de comunicación `<SPI.h>`.
* Librería de soporte `<Controllino.h>`.

---

## 3. Especificaciones de Conexión (Bus SPI)

La topología de red requiere que las líneas SCLK, MOSI y MISO sean compartidas, mientras que la línea de selección de esclavo (SS/CS) debe ser ruteada de forma independiente desde el Maestro hacia cada dispositivo esclavo.

| Señal SPI | Pin Controllino (Esclavo 1) | Pin Arduino UNO (Esclavo 2) | Conexión en el Maestro |
| :--- | :--- | :--- | :--- |
| **MOSI** | 51 | 11 | Terminal MOSI |
| **MISO** | 50 | 12 | Terminal MISO |
| **SCK** | 52 | 13 | Terminal SCK |
| **SS / CS** | 53 (Hardware por defecto) | 10 (Hardware por defecto) | Pin Digital Asignado |
| **GND** | Terminal GND | Terminal GND | Referencia Común (Obligatorio) |

---

