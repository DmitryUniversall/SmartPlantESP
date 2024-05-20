#include "TransferProcessors.h"

namespace TransferProcessors {
    String ActionResponse::serialize() const {
        JsonDocument document = serialize_json();

        String output;
        serializeJson(document, output);

        return output;
    }

    JsonDocument ActionResponse::serialize_json() const {
        JsonDocument document;

        document["ok"] = ok;
        document["message"] = message;
        document["application_status_code"] = application_status_code;

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

    void ActionProcessor::send_error(const char* message, uint application_status_code) {
        send_response({false, message, application_status_code, JsonDocument()});
    }

    void ActionProcessor::send_error(const String& message, uint application_status_code) {
        send_error(message.c_str(), application_status_code);
    }

    void ActionProcessor::send_response(const ActionResponse& response) {
        transfer_ws.send_request(
            {
                TransferRequestMSGType::SEND_QUEUE_MESSAGE,
                "android",
                response.serialize_json()
            }
        );
    }

    void ActionProcessor::_handle_transfer_response(const TransferResponse& response) {
        Serial.printf("[TransferProcessor]: ok: %s\n", response.ok ? "true" : "false");
        Serial.printf("[TransferProcessor]: message: %s\n", response.message.c_str());
        Serial.printf("[TransferProcessor]: application_status_code: %d\n", response.application_status_code);
    }

    void ActionProcessor::_handle_data_message(const TransferServer::DataMessage& message) {
        uint8_t action = message.data["action"];  // Returns 0 if key does not exist
        if (!action) return send_error(CODE_UNKNOWN_ACTION.message, CODE_UNKNOWN_ACTION.code);

        Serial.printf("[TransferProcessor]: Requested action %d\n", action);
        auto it = _actionHandlers.find(action);
        if (it == _actionHandlers.end()) return send_error(CODE_UNKNOWN_ACTION.message, CODE_UNKNOWN_ACTION.code);

        Serial.printf("[TransferProcessor]: Found handler for action %d\n", action);
        ActionHandler handler = it->second;
        handler(message, message.data);
    }
}

TransferProcessors::ActionProcessor action_processor;
