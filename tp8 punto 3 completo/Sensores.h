#pragma once

#include <Arduino.h>
#include <math.h>

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
// 4) Subclase Sensor Real (NTC en pin analógico, ej. A1)
class SensorTemperaturaReal : public Sensor {
private:
  uint8_t _pin;
  float _beta;
  float _resistenciaSerie;
  float _resistenciaNominal;

public:
  SensorTemperaturaReal(uint8_t pin,
                        float beta = 3950.0f,
                        float resistenciaSerie = 10000.0f,
                        float resistenciaNominal = 10000.0f)
    : Sensor("°C", -40.0f, 125.0f),
      _pin(pin),
      _beta(beta),
      _resistenciaSerie(resistenciaSerie),
      _resistenciaNominal(resistenciaNominal) {}

  void begin() {
    pinMode(_pin, INPUT);
  }

  void generarMedicion() override {
    int lectura = analogRead(_pin);
    if (lectura <= 0) {
      lectura = 1;
    } else if (lectura >= 1023) {
      lectura = 1022;
    }

    float rNtc = _resistenciaSerie *
                 (static_cast<float>(lectura) / static_cast<float>(1023 - lectura));
    float tempK = 1.0f / ((1.0f / 298.15f) + (log(rNtc / _resistenciaNominal) / _beta));
    _valor = tempK - 273.15f;
  }
};