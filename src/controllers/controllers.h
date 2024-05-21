#pragma once

#include <Arduino.h>

#define PIN_SERVO 17
#define PIN_R1_WATER_PUMP 5
#define PIN_R2_LAMP 4
#define PIN_LED 14


namespace Controllers {
    struct ControllersState {
        uint led;
        bool lamp;
        bool waterPump;
    };

    void setupControllers();

    void ledDigital(bool value);

    void ledAnalog();

    void ledToggle();

    void lampDigital(bool value);

    void lampToggle();

    void waterPumpDigital(bool value);

    void waterPumpToggle();

    void water();

    void waterAsync();
}
