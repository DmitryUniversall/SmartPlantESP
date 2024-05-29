#include "TransferServer.h"
#include "settings/Settings.h"

#include <utility>
#include <HTTPClient.h>

namespace TransferServer {
    String getAccessToken(const char* username, const char* userPassword) {  // TODO: Refactor this
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;

            Settings::AppConfig* config = project_settings.getConfig();

            http.begin(
                "http://" + config->transfer_server_host + ":" + config->transfer_server_port + "/api/auth/login/"
            );
            http.addHeader("Content-Type", "application/json");

            // Create JSON payload
            String payload =
                "{\"username\":\"" + String(username) + "\", \"password\":\"" + String(userPassword) + "\"}";

            int httpResponseCode = http.POST(payload);

            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.println(httpResponseCode);
                Serial.println(response);

                // Parse JSON response
                JsonDocument doc;
                DeserializationError error = deserializeJson(doc, response);

                if (!error) {
                    const char* accessToken = doc["data"]["tokens"]["access_token"];
                    http.end();

                    return {accessToken};
                } else {
                    Serial.print("JSON deserialization failed: ");
                    Serial.println(error.c_str());
                    http.end();

                    return {};
                }
            } else {
                Serial.print("Error on sending POST: ");
                Serial.println(httpResponseCode);
                http.end();

                return {};
            }

        } else {
            Serial.println("WiFi not connected");
            return {};
        }
    }

    TransferResponse TransferResponse::deserialize(const JsonObject& json) {
        TransferResponse response;

        response.ok = json["ok"];
        response.application_status_code = json["application_status_code"];
        response.message = json["message"].as<String>();
        response.data.set(json["data"]);

        return response;
    }

    String TransferResponse::serialize() const {
        JsonDocument document = serialize_json();

        String output;
        serializeJson(document, output);

        return output;
    }

    JsonDocument TransferResponse::serialize_json() const {
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

    String TransferRequest::serialize() const {
        JsonDocument doc;

        doc["msg_type"] = msg_type;
        doc["message_id"] = message_id;
        doc["target_device_id"] = target_device_id;
        doc["data"] = data;

        String output;
        serializeJson(doc, output);

        return output;
    }

    DataMessage DataMessage::deserialize(const JsonObject& json) {
        DataMessage message;

        message.data_type = json["data_type"] == 1 ? DataMessageType::REQUEST : DataMessageType::RESPONSE;  // TODO: REFACTOR THIS
        message.message_id = json["message_id"].as<String>();
        message.created_at = json["created_at"].as<String>();
        message.sender_device_id = json["sender_device_id"].as<String>();
        message.data.set(json["data"]);

        return message;
    }

    void TransferWS::loop() {
        _webSocket.loop();
    }

    void TransferWS::begin(const String& host, uint port, const String& url, const String& access_token) {
        Serial.println("[TransferWS]: Initializing WS connection");

        _webSocket.onEvent(std::bind(&TransferWS::_handle_websocket_event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        _webSocket.setExtraHeaders(("Authorization: Bearer " + access_token).c_str());
        _webSocket.setReconnectInterval(5000);
        _webSocket.begin(host, port, url);
    }

    void TransferWS::disconnect() {
        _webSocket.disconnect();
    }

    void TransferWS::send_request(const TransferRequest& request) {
        Serial.println(request.serialize());
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

        if (msg_type == TransferResponseMSGType::SERVER_RESPONSE) {
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

    void TransferWS::_handle_websocket_event(WStype_t type, byte* payload, size_t) {
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
