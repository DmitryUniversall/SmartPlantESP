#include "sensors.h"
#include <ArduinoJson.h>

HTSensor htSensor;


namespace Sensors {
    String SensorsData::serialize() const {
        JsonDocument document = serialize_json();

        String output;
        serializeJson(document, output);

        return output;
    }

    JsonDocument SensorsData::serialize_json() const {
        JsonDocument document;

        document["illumination"] = brightness;
        document["soil_moisture"] = soilMoisture;
        document["temperature"] = temperature;
        document["humidity"] = humidity;

        return document;
    }

    void setupSensors() {
        htSensor.begin();
        pinMode(PIN_BUTTON, INPUT_PULLUP);
        pinMode(PIN_LIGHT_SENSOR, INPUT);
        pinMode(PIN_SOIL_MOISTURE_SENSOR, INPUT);
    }

    Types::Pair<float, float> getHTAsync() {
        htSensor.readTemperatureAsync();
        htSensor.readHumidityAsync();

        return {
            htSensor.getLastTemperature(),
            htSensor.getLastHumidity()
        };
    }

    uint getSoilMoisture() {
        return analogRead(PIN_SOIL_MOISTURE_SENSOR);
    }

    uint getBrightness() {
        return 4096 - analogRead(PIN_LIGHT_SENSOR);
    }

    SensorsData getSensorsData() {
        Types::Pair<float, float> HTData = getHTAsync();

        return {
            getBrightness(),
            getSoilMoisture(),
            HTData.first,
            HTData.second
        };
    }
}