#include "ws_handlers.h"
#include "settings/Settings.h"
#include "utils/tasks/tasks.h"
#include "controllers/controllers.h"
#include "sensors/sensors.h"


namespace WSHandlers {
    Tasks::LoopedTask* sendDataTask = nullptr;

    void send_sensors_data() {
        Sensors::SensorsData sensors_data = Sensors::getSensorsData();
        ActionProcessor::send_response(
            {
                true,
                CODE_OK.message,
                CODE_OK.code,
                sensors_data.serialize_json()
            }
        );
    }

    void handle_request_data(const DataMessage&) {
        Serial.println("[REQUEST]: DATA");

        send_sensors_data();
    }

    void handle_request_data_update(const DataMessage&) {
        Serial.println("[REQUEST]: DATA_UPDATE");

        if (sendDataTask == nullptr) {
            Settings::AppConfig* config = project_settings.getConfig();

            sendDataTask = new Tasks::LoopedTask(config->data_update_interval, send_sensors_data);
            sendDataTask->callSoon();
        }
    }

    void handle_command_stop_data_update(const DataMessage&) {
        Serial.println("[COMMAND]: STOP_DATA_UPDATE");

        if (sendDataTask != nullptr) sendDataTask->cancel(); sendDataTask = nullptr;
    }

    void handle_command_settings_update(const DataMessage& message) {
        Serial.println("[COMMAND]: UPDATE_SETTINGS");

        Settings::AppConfig* config = project_settings.getConfig();

        JsonDocument documentCopy;
        documentCopy.set(message.data);

        config->updateSettingsFromJson(documentCopy.as<JsonObject>());
    }

    void handle_command_water(const DataMessage&) {
        Serial.println("[COMMAND]: WATER");
//        uint8_t duration = message.data["duration"];

        Controllers::water();
    }

    void handle_command_lamp(const DataMessage& message) {
        Serial.println("[COMMAND]: LAMP");

        uint8_t mode = message.data["mode"];

        if (mode == 2) {
            Controllers::lampToggle();
        } else {
            Controllers::lampDigital(mode);
        }
    }

    void register_ws_handlers() {
        action_processor.register_action(RequestAction::REQUEST_SENSORS_DATA, handle_request_data);
        action_processor.register_action(RequestAction::REQUEST_SENSORS_DATA_UPDATE, handle_request_data_update);
        action_processor.register_action(RequestAction::COMMAND_STOP_DATA_UPDATE, handle_command_stop_data_update);
        action_processor.register_action(RequestAction::COMMAND_SETTINGS_UPDATE, handle_command_settings_update);
        action_processor.register_action(RequestAction::COMMAND_WATER, handle_command_water);
        action_processor.register_action(RequestAction::COMMAND_LAMP, handle_command_lamp);
    }
}
