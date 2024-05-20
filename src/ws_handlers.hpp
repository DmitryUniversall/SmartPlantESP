#pragma once

#include "sensors/sensors.h"
#include "transfer/TransferProcessors.h"
#include "controllers/controllers.h"

using namespace Sensors;
using namespace TransferProcessors;
using namespace GenericCodes;
using namespace TransferWSCodes;


enum RequestAction {
    REQUEST_DATA = 1,
    REQUEST_DATA_UPDATE = 2,
    COMMAND_SETTINGS_UPDATE = 3,
    COMMAND_WATER = 4,
    COMMAND_LAMP = 5
};


void handle_request_data(const DataMessage&, const JsonDocument&) {
    SensorsData sensors_data = getSensorsData();
    ActionProcessor::send_response(
        {
            true,
            CODE_OK.message,
            CODE_OK.code,
            sensors_data.serialize_json()
        }
    );
}

void handle_request_data_update(const DataMessage&, const JsonDocument&) {}

void handle_command_settings_update(const DataMessage&, const JsonDocument&) {}

void handle_command_water(const DataMessage&, const JsonDocument&) {}

void handle_command_lamp(const DataMessage&, const JsonDocument&) {}


void register_ws_handlers() {
    action_processor.register_action(RequestAction::REQUEST_DATA, handle_request_data);
    action_processor.register_action(RequestAction::REQUEST_DATA_UPDATE, handle_request_data_update);
    action_processor.register_action(RequestAction::COMMAND_SETTINGS_UPDATE, handle_command_settings_update);
    action_processor.register_action(RequestAction::COMMAND_WATER, handle_command_water);
    action_processor.register_action(RequestAction::COMMAND_LAMP, handle_command_lamp);
}
