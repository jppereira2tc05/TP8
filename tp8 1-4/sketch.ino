#include <Arduino.h>
#include "Sensores.h"



SensorTemperatura tempSim(18.0f, 30.0f, -0.5f);
SensorLuz luzSim(0.0f, 100.0f, 30.0f);
SensorUltrasonido distSim(10.0f, 250.0f, 300.0f);
SensorTemperaturaReal tempReal(A1);

unsigned long ultimoEnvio = 0;
const unsigned long intervaloMs = 1500;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  Serial.println("=== TP8 - Ejercicio 1: Simulacion de Sensores ===");
  tempReal.begin();
}

void loop() {
  if (millis() - ultimoEnvio >= intervaloMs) {
    ultimoEnvio = millis();

    tempSim.generarMedicion();
    luzSim.generarMedicion();
    distSim.generarMedicion();
    tempReal.generarMedicion();

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
    /*Serial.print(" (");
    Serial.print(tempSim.leerFahrenheit(), 1);
    Serial.print(" °F) | ");*/
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
    Serial.println(distSim.hayObjetoCerca(40.0f) ? " [OBJETO CERCA]" : " [LIBRE]");
  }
}