#include "TransferProcessors.h"
#include "utils/utils.hpp"

namespace TransferProcessors {
    String StorageAction::serialize() const {
        JsonDocument doc;

        doc["action"] = action;
        doc["data"] = data;

        String output;
        serializeJson(doc, output);

        return output;
    }

    JsonDocument StorageAction::serialize_json() const {
        JsonDocument document;

        document["action"] = action;

        if (data.isNull()) {
            document["data"] = nullptr;
        } else {
            document["data"] = data;
        }

        return document;
    }

    void ActionProcessor::self_register() {
        transfer_ws.set_transfer_response_handler(_handle_transfer_response);

        transfer_ws.set_data_message_handler([this](const DataMessage& message) {
            _handle_data_message(message);
        });
    }

    void ActionProcessor::register_action(uint8_t action, const ActionHandler& handler) {
        _actionHandlers[action] = handler;
    }

    void ActionProcessor::send_error(const String& message, uint application_status_code, const String& message_id) {
        send_response({false, application_status_code, message, JsonDocument()}, message_id);
    }

    void ActionProcessor::send_response(const TransferResponse& response, const String& message_id) {
        transfer_ws.send_request(
            {
                TransferRequestMSGType::ENQUEUE_RESPONSE,
                message_id,
                "SmartPlantAndroid",
                response.serialize_json()
            }
        );
    }

    void ActionProcessor::send_action(const StorageAction& action) {
        transfer_ws.send_request(
            {
                TransferRequestMSGType::ENQUEUE_REQUEST,
                utils::generateRandomString(),
                "SmartPlantAndroid",
                action.serialize_json()
            }
        );
    }

    void ActionProcessor::_handle_transfer_response(const TransferResponse& response) {
        Serial.printf("[TransferProcessor]: ok: %s\n", response.ok ? "true" : "false");
        Serial.printf("[TransferProcessor]: message: %s\n", response.message.c_str());
        Serial.printf("[TransferProcessor]: application_status_code: %d\n", response.application_status_code);
    }

    void ActionProcessor::_handle_data_message(const TransferServer::DataMessage& message) {
        Serial.printf("[TransferProcessor]: Data message sender: %s\n", message.sender_device_id.c_str());

        uint8_t action = message.data["action"];  // Returns 0 if key does not exist
        if (!action) return send_error(CODE_UNKNOWN_ACTION.message, CODE_UNKNOWN_ACTION.code, message.message_id);

        Serial.printf("[TransferProcessor]: Requested action %d\n", action);
        auto it = _actionHandlers.find(action);
        if (it == _actionHandlers.end())
            return send_error(CODE_UNKNOWN_ACTION.message, CODE_UNKNOWN_ACTION.code, message.message_id);

        Serial.printf("[TransferProcessor]: Found handler for action %d\n", action);
        ActionHandler handler = it->second;
        handler(message);
    }
}

TransferProcessors::ActionProcessor action_processor;
