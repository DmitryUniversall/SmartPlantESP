#include "Settings.h"

#include "EEPROM.h"


namespace Settings {
    ProjectSettings::ProjectSettings() = default;

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

Settings::ProjectSettings project_settings = Settings::ProjectSettings();
