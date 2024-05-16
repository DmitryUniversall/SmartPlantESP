#include "HTSensor.h"

void HTSensor::begin() {
    Wire.begin();
    _sht.begin();
}

HTSensor::HTSensor() = default;

HTSensor::HTSensor(uint tempRequestInterval, uint humRequestInterval) {
    _tempRequestInterval = tempRequestInterval;
    _humRequestInterval = humRequestInterval;
}


//float HTSensor::readTemperature() {
//    return 0;
//}
//
//float HTSensor::readHumidity() {
//    return 0;
//}

void HTSensor::readTemperatureAsync() {
    uint32_t now = millis();

    if ((now - _lastTempReadTime > _tempRequestInterval) && (_sht.getRequestType() == 0x00)) _sht.requestTemperature();

    if (_sht.reqTempReady()) {
        _lastTempReadTime = now;

        _sht.readTemperature();
        _lastTemp = _sht.getTemperature();
    }
}

void HTSensor::readHumidityAsync() {
    uint32_t now = millis();

    if ((now - _lastHumReadTime > _humRequestInterval) && (_sht.getRequestType() == 0x00)) _sht.requestHumidity();

    if (_sht.reqHumReady()) {
        _lastHumReadTime = now;

        _sht.readHumidity();
        _lastHum = _sht.getHumidity();
    }
}

float HTSensor::getLastTemperature() const {
    return _lastTemp;
}

float HTSensor::getLastHumidity() const {
    return _lastHum;
}
