/*
 * Cátedra: Informática II - Ciclo Lectivo 2026
 * Trabajo Práctico N.º 8: Programación Orientada a Objetos (C++ / Arduino)
 *
 * Resumen de clases y extensiones implementadas:
 * - Sensor (clase base): almacena _valor, _magnitud y rangos de simulación (_minSim, _maxSim),
 *   con métodos generarMedicion() y leerMedicion().
 * - SensorTemperatura (hereda de Sensor): agrega _offset de calibración y leerFahrenheit().
 * - SensorLuz (hereda de Sensor): agrega _umbralOscuridad y estaOscuro().
 * - SensorUltrasonido (hereda de Sensor): agrega _distanciaMaxima y hayObjetoCerca().
 * - SensorTemperaturaReal (hereda de Sensor): lee un sensor NTC real en pin analógico (A1)
 *   convirtiendo la lectura ADC mediante ecuación Beta en lugar de simular con random().
 * - Led: controla un LED en un pin dado con encender(), apagar() y parpadear() no bloqueante.
 * - Timer: cronómetro basado exclusivamente en millis() controlado por Serial ('A', 'D', 'R').
 */

#include <Arduino.h>
#include "Sensores.h"
#include "Led.h"
#include "Timer.h"

// --- Instancias de sensores (Ejercicio 1: simulados y sensor real en A1) ---
SensorTemperatura tempSim(18.0f, 30.0f, -0.5f); // Temperatura simulada (°C) con offset de -0.5 °C
SensorLuz luzSim(0.0f, 100.0f, 30.0f);         // Luz simulada (%) con umbral de oscuridad en 30%
SensorUltrasonido distSim(10.0f, 250.0f, 300.0f); // Distancia simulada (cm) con tope de 300 cm
SensorTemperaturaReal tempReal(A1);            // Sensor real NTC conectado al pin analógico A1

// --- Instancias de LEDs en pines independientes (Ejercicio 2) ---
Led ledLuz(2);   // Pin 2: indica estado de oscuridad según luzSim
Led ledProx(3);  // Pin 3: parpadea si hay objeto cerca según distSim
Led ledTemp(4);  // Pin 4: alerta visual si tempReal supera el umbral de temperatura

// --- Instancia del cronómetro sin librerías externas (Ejercicio 3) ---
Timer cronometro;

// Temporización periódica no bloqueante para muestreo e impresión por Monitor Serial
unsigned long ultimoEnvio = 0;
const unsigned long intervaloMs = 1500;

void setup() {
  Serial.begin(9600);
  // Semilla aleatoria tomada de pin analógico flotante (A0) para los sensores simulados
  randomSeed(analogRead(A0));

  Serial.println("=== TP8 - POO en Arduino (Sensores, LEDs y Timer) ===");

  // Inicialización de hardware (pinMode y estados iniciales)
  tempReal.begin();
  ledLuz.begin();
  ledProx.begin();
  ledTemp.begin();
}

void loop() {
  // Actualización continua en cada ciclo para que el parpadeo con millis() sea fluido
  ledLuz.actualizar();
  ledProx.actualizar();
  ledTemp.actualizar();

  // Ejercicio 3: Control del Timer mediante caracteres recibidos por el Monitor Serial
  // 'A' -> activar/reanudar | 'D' -> detener/pausar | 'R' -> resetear a cero
  if (Serial.available() > 0) {
    char cmd = toupper(Serial.read());
    if (cmd == 'A') {
      cronometro.activar();
      Serial.println("[TIMER] Activado");
    } else if (cmd == 'D') {
      cronometro.detener();
      Serial.println("[TIMER] Detenido");
    } else if (cmd == 'R') {
      cronometro.resetear();
      Serial.println("[TIMER] Reseteado");
    }
  }

  // Muestreo periódico de sensores, actualización de LEDs y reporte por Serial
  if (millis() - ultimoEnvio >= intervaloMs) {
    ultimoEnvio = millis();

    // Ejercicio 1.3 y 1.4: Generar/leer mediciones de sensores simulados y del sensor real
    tempSim.generarMedicion();
    luzSim.generarMedicion();
    distSim.generarMedicion();
    tempReal.generarMedicion();

    // Ejercicio 2.3: Reacción independiente de cada LED a las lecturas de los sensores
    if (luzSim.estaOscuro()) {
      ledLuz.encender();
    } else {
      ledLuz.apagar();
    }

    if (distSim.hayObjetoCerca(40.0f)) {
      ledProx.parpadear(200); // Parpadeo rápido (200 ms) ante objeto cercano
    } else {
      ledProx.apagar();
    }

    if (tempReal.leerMedicion() > 30.0f) {
      ledTemp.parpadear(700); // Parpadeo más lento (700 ms) por temperatura alta en NTC
    } else {
      ledTemp.apagar();
    }

    // Impresión consolidada del estado del sistema en el Monitor Serial
    Serial.print("Temp: ");
    Serial.print(tempSim.leerMedicion(), 2);
    Serial.print(" ");
    Serial.print(tempSim.getMagnitud());
    Serial.print(" (");
    Serial.print(tempSim.leerFahrenheit(), 1);
    Serial.print(" °F) | ");

    Serial.print("TempReal: ");
    Serial.print(tempReal.leerMedicion(), 2);
    Serial.print(" ");
    Serial.print(tempReal.getMagnitud());
    Serial.print(" | ");

    Serial.print("Luz: ");
    Serial.print(luzSim.leerMedicion(), 1);
    Serial.print(" ");
    Serial.print(luzSim.getMagnitud());
    Serial.print(luzSim.estaOscuro() ? " [OSCURO]" : " [CLARO]");
    Serial.print(" | ");

    Serial.print("Dist: ");
    Serial.print(distSim.leerMedicion(), 1);
    Serial.print(" ");
    Serial.print(distSim.getMagnitud());
    Serial.print(distSim.hayObjetoCerca(40.0f) ? " [OBJETO CERCA]" : " [LIBRE]");

    Serial.print(" | Timer: ");
    Serial.print(cronometro.leerSegundos(), 2);
    Serial.print(" s ");
    Serial.println(cronometro.estaActivo() ? "[ACTIVO]" : "[DETENIDO]");
  }
}