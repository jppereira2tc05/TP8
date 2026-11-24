# Trabajo Práctico N.º 8 — Programación Orientada a Objetos (C++ / Arduino)

* **Cátedra:** Informática II  
* **Carrera:** Ingeniería Electrónica  
* **Ciclo Lectivo:** 2026  
* **Plataforma:** Arduino Nano (Simulación en Wokwi)

---

## 📋 Descripción General

Este repositorio contiene el desarrollo completo del **Trabajo Práctico N.º 8**, enfocado en aplicar los pilares de la **Programación Orientada a Objetos (POO)** (encapsulamiento, herencia y polimorfismo) en C++ sobre la plataforma Arduino.

El sistema implementa una arquitectura modular dividida en archivos de cabecera (`.h`) y un programa principal (`sketch.ino`), evitando el uso de funciones bloqueantes (`delay()`) mediante temporización no bloqueante con `millis()`.

---

## 📁 Estructura del Repositorio

```text
├── sketch.ino      # Programa principal: instanciación, muestreo, lógica de control y Serial
├── Sensores.h      # Clase base Sensor y subclases (simuladas y sensor real NTC)
├── Led.h           # Clase Led para control de estado y parpadeo independiente con millis()
├── Timer.h         # Clase Timer (cronómetro) sin librerías externas controlado por Serial
├── diagram.json    # Esquema de conexiones y componentes para Wokwi
└── README.md       # Documentación del proyecto
```

---

## 🏗️ Arquitectura de Clases

### 1. Jerarquía de Sensores (`Sensores.h`)
* **`Sensor` (Clase Base):** Encapsula los atributos protegidos `_valor`, `_magnitud`, `_minSim` y `_maxSim`. Define los métodos virtuales `generarMedicion()` y `leerMedicion()`, permitiendo trabajar de manera uniforme con distintos tipos de sensores.
* **`SensorTemperatura` (Hereda de `Sensor`):** Simula temperatura en `°C`. Agrega el atributo privado `_offset` para calibración y el método `leerFahrenheit()` para conversión de escala.
* **`SensorLuz` (Hereda de `Sensor`):** Simula intensidad lumínica en porcentaje (`%`). Agrega `_umbralOscuridad` y el método booleano `estaOscuro()`.
* **`SensorUltrasonido` (Hereda de `Sensor`):** Simula medición de distancia en `cm`. Agrega `_distanciaMaxima` como cota superior y el método `hayObjetoCerca(float umbralCm)`.
* **`SensorTemperaturaReal` (Hereda de `Sensor`):** Implementa la lectura de un termistor **NTC de $10\text{ k}\Omega$** conectado al pin analógico `A1`. Sobrescribe `generarMedicion()` leyendo el conversor ADC (`analogRead`) y calculando la temperatura en grados Celsius mediante la ecuación Beta:
  $$T_K = \frac{1}{\frac{1}{T_0} + \frac{1}{\beta} \ln\left(\frac{R_{NTC}}{R_0}\right)}$$

> **¿Qué cambia al pasar de un sensor simulado a uno real? (Ejercicio 1.4)**  
> A nivel de software, se reemplaza la generación pseudoaleatoria (`random()`) por la lectura directa de puertos físicos (`analogRead()` o buses digitales) y su posterior conversión matemática a la magnitud física real, incorporando el pin de conexión y su inicialización (`pinMode`). A nivel de hardware, requiere el cableado de alimentación ($5\text{ V}$ / $\text{GND}$), señal y acondicionamiento (como divisores resistivos).

### 2. Control de Actuadores (`Led.h`)
* **`Led`:** Abstrae el manejo de diodos LED conectados a pines digitales. Permite instanciar múltiples objetos independientes con métodos `encender()`, `apagar()`, `parpadear(intervaloMs)` y `actualizar()` (ejecutado en cada iteración del `loop()`).
* **Reacción a Sensores:**
  * `ledLuz` (Pin `D2`): Se enciende fijo cuando `luzSim.estaOscuro()` es verdadero.
  * `ledProx` (Pin `D3`): Parpadea rápidamente ($200\text{ ms}$) cuando `distSim.hayObjetoCerca(40.0)` detecta proximidad.
  * `ledTemp` (Pin `D4`): Parpadea ($700\text{ ms}$) cuando la temperatura del sensor real `tempReal` supera los $30.0\text{ }^\circ\text{C}$.

### 3. Control de Tiempo (`Timer.h`)
* **`Timer`:** Implementa un cronómetro de precisión en milisegundos utilizando exclusivamente `millis()`, sin dependencias externas.
* **Comandos por Monitor Serial (`9600 baudios`):**
  * `'A'` / `'a'`: Activa o reanuda el conteo sin perder el tiempo acumulado.
  * `'D'` / `'d'`: Detiene (pausa) el cronómetro conservando el registro actual.
  * `'R'` / `'r'`: Resetea el contador a `0.00 s`.

---

## 🔌 Esquema de Conexiones (Arduino Nano)

| Componente | Pin / Terminal | Pin Arduino Nano | Observaciones |
| :--- | :--- | :--- | :--- |
| **Sensor NTC (Módulo)** | `VCC` / `GND` / `OUT` | `5V` / `GND` / `A1` | Lectura analógica real |
| **Semilla Aleatoria** | — | `A0` | Pin flotante para `randomSeed()` |
| **LED Luz** | Ánodo (vía $R = 220\ \Omega$) | `D2` | Cátodo a `GND` |
| **LED Proximidad** | Ánodo (vía $R = 220\ \Omega$) | `D3` | Cátodo a `GND` |
| **LED Temperatura** | Ánodo (vía $R = 220\ \Omega$) | `D4` | Cátodo a `GND` |

*(El archivo `diagram.json` incluido en el repositorio permite reproducir el circuito directamente en Wokwi).*

---

## 🚀 Aplicación al Trabajo Final

La arquitectura modular desarrollada en este práctico se integra directamente en el **Trabajo Final de Domótica** basado en Arduino Nano:
1. **Reutilización de `Led` y `Timer`:** Permiten gestionar luminarias, indicadores de estado y temporizaciones de apagado automático de ambientes sin bloquear la recepción de comandos ni el muestreo de entradas.
2. **Extensibilidad de `Sensor`:** La clase base `Sensor` estandariza la interfaz de lectura (`generarMedicion()` / `leerMedicion()`), facilitando la incorporación de los sensores físicos definitivos del proyecto (presencia/movimiento, temperatura ambiente y luminosidad) manteniendo el código principal limpio y desacoplado.

---

## ▶️ Instrucciones de Uso y Simulación

1. Abrir el proyecto en **Wokwi** (cargando `sketch.ino`, los archivos `.h` y `diagram.json`) o en el **Arduino IDE**.
2. Iniciar la simulación y abrir el **Monitor Serial** a `9600 baudios`.
3. Observar el reporte periódico (cada $1500\text{ ms}$) con las mediciones simuladas, la lectura del NTC y el estado del temporizador.
4. Modificar el deslizador del sensor NTC en Wokwi por encima de $30\text{ }^\circ\text{C}$ para verificar la alerta visual en el LED conectado a `D4`.
5. Enviar los caracteres `A`, `D` o `R` en la consola Serial para operar el cronómetro en tiempo real.