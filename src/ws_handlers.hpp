#include <Arduino.h>
#include "transfer/TransferServer.h"
#include "settings/Settings.h"
#include "utils/wifi.hpp"


using namespace TransferServer;


void handle_transfer_response(const TransferResponse& response) {
    Serial.printf("[TransferResponse] ok: %s\n", response.ok ? "true" : "false");
    Serial.printf("[TransferResponse] message: %s\n", response.message.c_str());
    Serial.printf("[TransferResponse] application_status_code: %u\n", response.application_status_code);
}


void handle_data_message(const DataMessage& message) {
    JsonDocument document;
    deserializeJson(document, message.data);

    int random_int = document["random_int"];
    Serial.printf("Random int: %d\n", random_int);

    JsonDocument response;
    response["ok"] = true;

    transfer_ws.send_request(
            {
                    TransferRequestMSGType::SEND_QUEUE_MESSAGE,
                    "android",
                    response.as<JsonObject>()
            }
    );
}
