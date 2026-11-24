#include <Arduino.h>
#include "Sensores.h"
#include "Led.h"



SensorTemperatura tempSim(18.0f, 30.0f, -0.5f);
SensorLuz luzSim(0.0f, 100.0f, 30.0f);
SensorUltrasonido distSim(10.0f, 250.0f, 300.0f);
SensorTemperaturaReal tempReal(A1);
Led ledLuz(2);
Led ledProx(3);
Led ledTemp(4);

unsigned long ultimoEnvio = 0;
const unsigned long intervaloMs = 1500;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  Serial.println("=== TP8 - Ejercicio 1: Simulacion de Sensores ===");
  tempReal.begin();
  ledLuz.begin();
  ledProx.begin();
  ledTemp.begin();
}

void loop() {
  //Así el parpadeo corre fluido en cada ciclo sin esperar los `1500 ms` del Monitor Serial.
  ledLuz.actualizar();
  ledProx.actualizar();
  ledTemp.actualizar();

  if (millis() - ultimoEnvio >= intervaloMs) {
    ultimoEnvio = millis();

    tempSim.generarMedicion();
    luzSim.generarMedicion();
    distSim.generarMedicion();
    tempReal.generarMedicion();

    if (luzSim.estaOscuro()) {
  ledLuz.encender();
} else {
  ledLuz.apagar();
}

if (distSim.hayObjetoCerca(40.0f)) {
  ledProx.parpadear(200);
} else {
  ledProx.apagar();
}

if (tempReal.leerMedicion() > 30.0f) {
  ledTemp.parpadear(700);
} else {
  ledTemp.apagar();
}
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