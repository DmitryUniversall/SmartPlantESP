#include "TransferServer.h"

#include <utility>

namespace TransferServer {
    String TransferRequest::serialize() const {
        JsonDocument doc;

        doc["data"] = data;
        doc["msg_type"] = msg_type;
        doc["target_device_id"] = target_device_id;

        String output;
        serializeJson(doc, output);

        return output;
    }

    TransferResponse TransferResponse::deserialize(const JsonObject& json) {
        TransferResponse response;

        response.ok = json["ok"];
        response.application_status_code = json["application_status_code"];
        response.message = json["message"].as<String>();
        response.data = json["data"].as<String>();

        return response;
    }

    DataMessage DataMessage::deserialize(const JsonObject& json) {
        DataMessage message;

        message.created_at = json["created_at"].as<String>();
        message.sender_device_id = json["sender_device_id"].as<String>();
        message.data = json["data"].as<String>();

        return message;
    }

    void TransferWS::loop() {
        _webSocket.loop();
    }

    void TransferWS::begin(const String& host, uint port, const String& url, const String& access_token) {
        _webSocket.begin(host, port, url);
        _webSocket.onEvent(std::bind(&TransferWS::_handle_websocket_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        _webSocket.setExtraHeaders(("Authorization: Bearer " + access_token).c_str());
        _webSocket.setReconnectInterval(5000);
    }

    void TransferWS::disconnect() {
        _webSocket.disconnect();
    }

    void TransferWS::send_request(const TransferRequest& request) {
        _webSocket.sendTXT(request.serialize().c_str());
    }

    void TransferWS::set_data_message_handler(DataMessageHandler handler) {
        _data_message_handler = std::move(handler);
    }

    void TransferWS::set_transfer_response_handler(TransferWS::TransferResponseHandler handler) {
        _transfer_response_handler = std::move(handler);
    }

    void TransferWS::_handle_transfer_response(const TransferResponse& response) {
        Serial.println("[TransferWS]: Got transfer response");
        _transfer_response_handler(response);
    }

    void TransferWS::_handle_data_message(const DataMessage& message) {
        Serial.println("[TransferWS]: Got data message");
        _data_message_handler(message);
    }

    void TransferWS::_handle_websocket_message(uint8_t* payload) {
        Serial.printf("[TransferWS]: Got ws text: %s\n", payload);

        JsonDocument doc;
        deserializeJson(doc, payload);
        uint8_t msg_type = doc["msg_type"];

        if (msg_type == TransferResponseMSGType::RESPONSE) {
            _handle_transfer_response(TransferResponse::deserialize(doc.as<JsonObject>()));
        } else if (msg_type == TransferResponseMSGType::DATA) {
            _handle_data_message(DataMessage::deserialize(doc["data"].as<JsonObject>()));
        } else {
            Serial.printf("[TransferWS]: Ignoring unknown msg_type %d\n", msg_type);
        }
    }

    void TransferWS::_handle_websocket_connect(uint8_t* payload) {
        Serial.printf("[TransferWS]: Connected: %s\n", payload);
    }

    void TransferWS::_handle_websocket_disconnect(uint8_t* payload) {
        Serial.printf("[TransferWS]: Disconnected: %s\n", payload);
    }

    void TransferWS::_handle_websocket_event(WStype_t type, byte* payload, size_t length) {
        if (type == WStype_CONNECTED) {
            _handle_websocket_connect(payload);
        } else if (type == WStype_DISCONNECTED) {
            _handle_websocket_disconnect(payload);
        } else if (type == WStype_TEXT) {
            _handle_websocket_message(payload);
        }
    }
}


TransferServer::TransferWS transfer_ws;
