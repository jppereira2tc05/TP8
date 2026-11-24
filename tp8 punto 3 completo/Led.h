#pragma once
#include <Arduino.h>

class Led {
private:
  uint8_t _pin;
  bool _estado;
  bool _parpadeando;
  unsigned long _intervalo;
  unsigned long _ultimoCambio;

  void escribirPin(bool encenderLed) {
    _estado = encenderLed;
    digitalWrite(_pin, _estado ? HIGH : LOW);
  }

public:
  explicit Led(uint8_t pin)
    : _pin(pin),
      _estado(false),
      _parpadeando(false),
      _intervalo(500),
      _ultimoCambio(0) {}

  void begin() {
    pinMode(_pin, OUTPUT);
    apagar();
  }

  void encender() {
    _parpadeando = false;
    escribirPin(true);
  }

  void apagar() {
    _parpadeando = false;
    escribirPin(false);
  }

  void parpadear(unsigned long intervaloMs) {
    _intervalo = intervaloMs;
    if (!_parpadeando) {
      _parpadeando = true;
      _ultimoCambio = millis();
    }
  }

  void actualizar() {
    if (!_parpadeando) {
      return;
    }

    unsigned long ahora = millis();
    if (ahora - _ultimoCambio >= _intervalo) {
      _ultimoCambio = ahora;
      escribirPin(!_estado);
    }
  }

  bool estaEncendido() const {
    return _estado;
  }
};