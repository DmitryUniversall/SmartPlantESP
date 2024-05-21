#include "Settings.h"

#include "EEPROM.h"


namespace Settings {
    void AppConfig::updateSettingsFromJson(const JsonObject& data) {
        if (data.containsKey("user_wifi_ssid")) {
            user_wifi_ssid = data["user_wifi_ssid"].as<String>();
            Serial.printf("Updated config field user_wifi_ssid: %s\n", user_wifi_ssid.c_str());
        }

        if (data.containsKey("user_wifi_password")) {
            user_wifi_password = data["user_wifi_password"].as<String>();
            Serial.printf("Updated config field user_wifi_password: %s\n", user_wifi_password.c_str());
        }

        if (data.containsKey("esp_wifi_ssid")) {
            esp_wifi_ssid = data["esp_wifi_ssid"].as<String>();
            Serial.printf("Updated config field esp_wifi_ssid: %s\n", esp_wifi_ssid.c_str());
        }

        if (data.containsKey("esp_wifi_password")) {
            esp_wifi_password = data["esp_wifi_password"].as<String>();
            Serial.printf("Updated config field esp_wifi_password: %s\n", esp_wifi_password.c_str());
        }

        if (data.containsKey("transfer_server_port")) {
            transfer_server_port = data["transfer_server_port"];
            Serial.printf("Updated config field transfer_server_port: %d\n", transfer_server_port);
        }

        if (data.containsKey("transfer_server_host")) {
            transfer_server_host = data["transfer_server_host"].as<String>();
            Serial.printf("Updated config field transfer_server_host: %s\n", transfer_server_host.c_str());
        }

        if (data.containsKey("transfer_server_login")) {
            transfer_server_login = data["transfer_server_login"].as<String>();
            Serial.printf("Updated config field transfer_server_login: %s\n", transfer_server_login.c_str());
        }

        if (data.containsKey("transfer_server_password")) {
            transfer_server_password = data["transfer_server_password"].as<String>();
            Serial.printf("Updated config field transfer_server_password: %s\n", transfer_server_password.c_str());
        }

        project_settings.save();
    }

    void AppConfig::show() const {
        Serial.printf("Core settings\n");
        Serial.printf("Config field transfer_server_port: %d\n", transfer_server_port);
        Serial.printf("Config field transfer_server_host: %s\n", transfer_server_host.c_str());
        Serial.printf("Config field esp_wifi_name: %s\n", esp_wifi_ssid.c_str());
        Serial.printf("Config field esp_wifi_password: %s\n", esp_wifi_password.c_str());
        Serial.printf("Config field user_wifi_name: %s\n", user_wifi_ssid.c_str());
        Serial.printf("Config field user_wifi_password: %s\n", user_wifi_password.c_str());
        Serial.printf("Config field transfer_server_login: %s\n", transfer_server_login.c_str());
        Serial.printf("Config field transfer_server_password: %s\n", transfer_server_password.c_str());
        Serial.printf("Growing settings\n");
        Serial.printf("Config field watering_duration: %d\n", watering_duration);
    }

    bool ProjectSettings::_is_initialized() {
        return EEPROM.read(EEPROM_CONFIG_INIT_ADDR) == EEPROM_CONFIG_INIT_KEY;
    }

    void ProjectSettings::_set_initialized(bool initialized) {
        EEPROM.write(EEPROM_CONFIG_INIT_ADDR, initialized ? EEPROM_CONFIG_INIT_KEY : 0);
        EEPROM.commit();
    }

    void ProjectSettings::_put_config(const AppConfig& config) {
        EEPROM.put(EEPROM_CONFIG_ADDR, config);
        EEPROM.commit();
    }

    void ProjectSettings::_read_config(AppConfig& config) {
        EEPROM.get(EEPROM_CONFIG_ADDR, config);
    }

    AppConfig ProjectSettings::_get_default_config() {
        return AppConfig{};
    }

    void ProjectSettings::_get_config(AppConfig& config) {
        if (!_is_initialized()) {
            Serial.println("Initializing new config");
            config = _get_default_config();
            _set_initialized(true);
            _put_config(config);
        } else {
            Serial.println("Reading config from EEPROM");
            _read_config(config);
        }
    }

    void ProjectSettings::read_from_storage() {
        _get_config(_config);
    }

    void ProjectSettings::save() {
        _put_config(_config);
    }

    AppConfig* ProjectSettings::getConfig() {
        return &_config;
    }

    void ProjectSettings::setConfig(AppConfig& config) {
        _config = std::move(config);
    }
}

Settings::ProjectSettings project_settings;
