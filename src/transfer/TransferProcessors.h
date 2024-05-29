#pragma once

#include "transfer/TransferServer.h"
#include <ArduinoJson.h>
#include <map>
#include <application_status_codes.h>

using namespace TransferServer;
using namespace TransferWSCodes;


namespace TransferProcessors {
    using ActionHandler = std::function<void(const DataMessage&)>;

    struct StorageAction {
        uint action;
        JsonDocument data;

        String serialize() const;
        JsonDocument serialize_json() const;
    };

    class ActionProcessor {
    public:
        void self_register();

        void register_action(uint8_t action, const ActionHandler& handler);

        static void send_error(const String& message, uint application_status_code, const String& message_id);

        static void send_response(const TransferResponse& response, const String& message_id);

        static void send_action(const StorageAction& action);

    private:
        static void _handle_transfer_response(const TransferResponse& response);

        void _handle_data_message(const DataMessage& message);

        std::map<uint8_t, ActionHandler> _actionHandlers;
    };
}


extern TransferProcessors::ActionProcessor action_processor;
