#include <Arduino.h>

class Sensor {
protected:
  float _valor;
  String _magnitud;
  float _minSim;
  float _maxSim;

public:
  Sensor(String magnitud, float minSim, float maxSim)
    : _valor(0.0f), _magnitud(magnitud), _minSim(minSim), _maxSim(maxSim) {}

  virtual ~Sensor() {}

  // Simula una medición dentro del rango configurado
  virtual void generarMedicion() {
    long escala = 1000L;
    long minEntero = static_cast<long>(_minSim * escala);
    long maxEntero = static_cast<long>(_maxSim * escala);
    long aleatorio = random(minEntero, maxEntero + 1);
    _valor = static_cast<float>(aleatorio) / static_cast<float>(escala);
  }

  // Devuelve la última medición almacenada
  virtual float leerMedicion() const {
    return _valor;
  }

  String getMagnitud() const {
    return _magnitud;
  }
};

// 1) Subclase Temperatura
class SensorTemperatura : public Sensor {
private:
  float _offset;

public:
  SensorTemperatura(float minC = 15.0f, float maxC = 35.0f, float offset = 0.0f)
    : Sensor("°C", minC, maxC), _offset(offset) {}

  void generarMedicion() override {
    Sensor::generarMedicion();
    _valor += _offset;
  }

  void setOffset(float offset) {
    _offset = offset;
  }

  float leerFahrenheit() const {
    return (_valor * 1.8f) + 32.0f;
  }
};

// 2) Subclase Luz
class SensorLuz : public Sensor {
private:
  float _umbralOscuridad;

public:
  SensorLuz(float minPct = 0.0f, float maxPct = 100.0f, float umbralOscuridad = 25.0f)
    : Sensor("%", minPct, maxPct), _umbralOscuridad(umbralOscuridad) {}

  bool estaOscuro() const {
    return _valor < _umbralOscuridad;
  }

  void setUmbralOscuridad(float umbral) {
    _umbralOscuridad = umbral;
  }
};

// 3) Subclase Ultrasonido
class SensorUltrasonido : public Sensor {
private:
  float _distanciaMaxima;

public:
  SensorUltrasonido(float minCm = 5.0f, float maxCm = 300.0f, float distMax = 400.0f)
    : Sensor("cm", minCm, maxCm), _distanciaMaxima(distMax) {}

  void generarMedicion() override {
    Sensor::generarMedicion();
    if (_valor > _distanciaMaxima) {
      _valor = _distanciaMaxima;
    }
  }

  bool hayObjetoCerca(float umbralCm) const {
    return (_valor > 0.0f) && (_valor <= umbralCm);
  }
};

SensorTemperatura tempSim(18.0f, 30.0f, -0.5f);
SensorLuz luzSim(0.0f, 100.0f, 30.0f);
SensorUltrasonido distSim(10.0f, 250.0f, 300.0f);

unsigned long ultimoEnvio = 0;
const unsigned long intervaloMs = 1500;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  Serial.println("=== TP8 - Ejercicio 1: Simulacion de Sensores ===");
}

void loop() {
  if (millis() - ultimoEnvio >= intervaloMs) {
    ultimoEnvio = millis();

    tempSim.generarMedicion();
    luzSim.generarMedicion();
    distSim.generarMedicion();

    Serial.print("Temp: ");
    Serial.print(tempSim.leerMedicion(), 2);
    Serial.print(" ");
    Serial.print(tempSim.getMagnitud());
    Serial.print(" (");
    Serial.print(tempSim.leerFahrenheit(), 1);
    Serial.print(" °F) | ");

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