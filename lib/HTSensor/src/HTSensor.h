#pragma once

#include <Arduino.h>
#include "SHT2x.h"


class HTSensor {
public:
    HTSensor();

    HTSensor(uint tempRequestInterval, uint humRequestInterval);

    // Setup
    void begin();

    // For sync interface
//    float readTemperature();
//
//    float readHumidity();

    // For async interface
    void readTemperatureAsync();

    void readHumidityAsync();

    float getLastTemperature() const;

    float getLastHumidity() const;

protected:
    uint _tempRequestInterval = 1000;
    uint _humRequestInterval = 1000;

    uint32_t _lastTempReadTime = 0;
    uint32_t _lastHumReadTime = 0;

    float _lastTemp = -1;
    float _lastHum = -1;

    SHT2x _sht;
};
