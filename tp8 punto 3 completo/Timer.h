#pragma once
#include <Arduino.h>

class Timer {
private:
  bool _activo;
  unsigned long _inicioMs;
  unsigned long _acumuladoMs;

public:
  Timer() : _activo(false), _inicioMs(0), _acumuladoMs(0) {}

  void activar() {
    if (!_activo) {
      _inicioMs = millis();
      _activo = true;
    }
  }

  void detener() {
    if (_activo) {
      _acumuladoMs += (millis() - _inicioMs);
      _activo = false;
    }
  }

  void resetear() {
    _acumuladoMs = 0;
    if (_activo) {
      _inicioMs = millis();
    }
  }

  unsigned long leerMs() const {
    if (_activo) {
      return _acumuladoMs + (millis() - _inicioMs);
    }
    return _acumuladoMs;
  }

  float leerSegundos() const {
    return static_cast<float>(leerMs()) / 1000.0f;
  }

  bool estaActivo() const {
    return _activo;
  }
};