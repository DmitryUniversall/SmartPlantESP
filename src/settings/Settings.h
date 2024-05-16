#pragma once

#include <Arduino.h>

#define EEPROM_CONFIG_ADDR 2

#define EEPROM_CONFIG_INIT_ADDR 1
#define EEPROM_CONFIG_INIT_KEY 55

namespace Settings {
    struct AppConfig {
        uint transfer_server_port = 8000;
        String transfer_server_host = "172.27.11.5";
        String transfer_server_login = "master";
        String transfer_server_password = "master";

        String esp_wifi_ssid = "SmartPlant";
        String esp_wifi_password = "SmartPlant";

        String user_wifi_ssid = "SmartPlant";
        String user_wifi_password = "SmartPlant";
    };

    class ProjectSettings {
    public:
        ProjectSettings();

        explicit ProjectSettings(AppConfig& config);

        void read_from_storage();

        void save();

        AppConfig* getConfig();

        void setConfig(AppConfig& config);

    protected:
        static bool _is_initialized();

        static void _set_initialized(bool initialized);

        static AppConfig _get_default_config();

        static void _get_config(AppConfig& config);

        static void _put_config(const AppConfig& config);

        static void _read_config(AppConfig& config);

        AppConfig _config;
    };
}
