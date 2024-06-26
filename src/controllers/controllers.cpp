#include "controllers.h"
#include "utils/tasks/tasks.h"
#include "settings/Settings.h"


namespace Controllers {
    ControllersState controllers_state;

    void setupControllers() {
        pinMode(PIN_SERVO, OUTPUT);
        pinMode(PIN_R1_WATER_PUMP, OUTPUT);
        pinMode(PIN_R2_LAMP, OUTPUT);
        pinMode(PIN_LED, OUTPUT);
    }

    // ====== LED ======
    void ledDigital(bool value) {
        digitalWrite(PIN_LED, value);
    }

    void ledAnalog(uint8_t value) {
        analogWrite(PIN_LED, value);
    }

    void ledToggle() {
        uint8_t newState = controllers_state.led == LOW ? HIGH : LOW;
        ledDigital(newState);
        controllers_state.led = newState;
    }

    // ====== LAMP ======
    void lampDigital(bool value) {
        digitalWrite(PIN_R2_LAMP, value);
    }

    void lampToggle() {
        lampDigital(!controllers_state.lamp);
        controllers_state.lamp = !controllers_state.lamp;
    }

    // ====== WaterPump ======
    void waterPumpDigital(bool value) {
        digitalWrite(PIN_R1_WATER_PUMP, value);
    }

    void waterPumpToggle() {
        waterPumpDigital(!controllers_state.waterPump);
        controllers_state.waterPump = !controllers_state.waterPump;
    }

    // ====== Utils ======
    void water() {
        Settings::AppConfig* config = project_settings.getConfig();

        Serial.printf("Watering started; Duration: %d\n", config->watering_duration);
        waterPumpDigital(HIGH);
        delay(config->watering_duration);
        waterPumpDigital(LOW);
        Serial.println("Watering done");
    }

    void waterAsync() {
        Settings::AppConfig* config = project_settings.getConfig();

        waterPumpDigital(HIGH);
        Serial.printf("Watering started; Duration: %d\n", config->watering_duration);

        // Is it danger? (task can be called only after all processes is completed, so pump can be enabled longer, then expected)
        taskManager.callAfter(config->watering_duration, []() {
            waterPumpDigital(LOW);
            Serial.println("Watering done");
        });
    }
}
