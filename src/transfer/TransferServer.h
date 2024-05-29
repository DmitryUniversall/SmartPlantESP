#pragma once

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

namespace TransferServer {
    enum TransferResponseMSGType {
        SERVER_RESPONSE = 1,
        DATA = 2
    };

    enum TransferRequestMSGType {
        ENQUEUE_REQUEST = 3,
        ENQUEUE_RESPONSE = 4
    };

    enum DataMessageType {
        REQUEST = 1,
        RESPONSE = 2
    };

    struct TransferRequest {
        TransferRequestMSGType msg_type;
        String message_id;
        String target_device_id;
        JsonDocument data;

        String serialize() const;
    };

    struct TransferResponse {
        bool ok;
        uint application_status_code;
        String message;
        JsonDocument data;

        static TransferResponse deserialize(const JsonObject& json);

        JsonDocument serialize_json() const;

        String serialize() const;
    };

    struct DataMessage {
        DataMessageType data_type;
        String message_id;
        String created_at;
        String sender_device_id;
        JsonDocument data;

        static DataMessage deserialize(const JsonObject& json);
    };

    String getAccessToken(const char* username, const char* userPassword);

    class TransferWS {
    public:
        using DataMessageHandler = std::function<void(const DataMessage&)>;
        using TransferResponseHandler = std::function<void(const TransferResponse&)>;

        void loop();

        void begin(const String& host, uint port, const String& url, const String& access_token);

        void disconnect();

        void send_request(const TransferRequest& request);

        void set_data_message_handler(DataMessageHandler handler);

        void set_transfer_response_handler(TransferResponseHandler handler);

    protected:
        void _handle_transfer_response(const TransferResponse& message);

        void _handle_data_message(const DataMessage& message);

        void _handle_websocket_message(uint8_t* payload);

        static void _handle_websocket_connect(uint8_t* payload);

        static void _handle_websocket_disconnect(uint8_t* payload);

        void _handle_websocket_event(WStype_t type, uint8_t* payload, size_t length);

        WebSocketsClient _webSocket;
        DataMessageHandler _data_message_handler;
        TransferResponseHandler _transfer_response_handler;
    };
}


extern TransferServer::TransferWS transfer_ws;
