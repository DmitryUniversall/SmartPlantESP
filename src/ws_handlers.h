#pragma once

#include "transfer/TransferProcessors.h"

using namespace TransferProcessors;
using namespace TransferWSCodes;
using namespace GenericCodes;


namespace WSHandlers {
    enum ApplicationAction {
        REQUEST_SENSORS_DATA = 1,
        REQUEST_SENSORS_DATA_UPDATE = 2,
        COMMAND_STOP_DATA_UPDATE = 3,
        COMMAND_SETTINGS_UPDATE = 4,
        COMMAND_WATER = 5,
        COMMAND_LAMP = 6
    };

    enum DeviceAction {
        PING = 1,
        SENSORS_DATA_UPDATE = 2
    };

    void send_sensors_data();

    void handle_request_data(const DataMessage&);

    void handle_request_data_update(const DataMessage&);

    void handle_command_settings_update(const DataMessage& message);

    void handle_command_water(const DataMessage&);

    void handle_command_lamp(const DataMessage& message);

    void register_ws_handlers();
}
