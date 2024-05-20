#pragma once

#include <ArduinoJson.h>
#include "utils/types.hpp"
#include "HTSensor.h"

#define PIN_LIGHT_SENSOR 13
#define PIN_SOIL_MOISTURE_SENSOR 12


namespace Sensors {
    struct SensorsData {  // TODO: Add water_level
        uint brightness;
        uint soilMoisture;
        float temperature;
        float humidity;

        String serialize() const;
        JsonDocument serialize_json() const;
    };

    Types::Pair<float, float> getHTAsync();

    uint getSoilMoisture();

    uint getBrightness();

    SensorsData getSensorsData();
}