//#include <EEPROM.h>
//#include "wifi/WIFIUtils.hpp"
//#include "settings/Settings.h"
//#include "ESPWebServer/ESPWebServer.h"
//
//Settings::ProjectSettings project_settings;
//ESPWeb::ESPWebServer server;
//
//
//void setup() {
//    Serial.begin(19200);
//    EEPROM.begin(512);
//
//    project_settings.read_from_storage();
//    Settings::AppConfig* config = project_settings.getConfig();
//    WIFIUtils::createSoftAP(config->esp_wifi_ssid, config->esp_wifi_password);
//
//    server.setup(project_settings);
//    server.begin();
//}
//
//
//void loop() {
//    server.serve();
//    // TODO
//}

#include <Arduino.h>
#include "transfer/TransferServer.h"
#include "wifi/WIFIUtils.hpp"
#include "settings/Settings.h"
#include "ESPWebServer/ESPWebServer.h"


using namespace TransferServer;
using namespace ESPWeb;


TransferWS transferWS;
ESPWebServer webServer;


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

    transferWS.send_request(
            {
                    TransferRequestMSGType::SEND_QUEUE_MESSAGE,
                    "android",
                    response.as<JsonObject>()
            }
    );
}


void setup() {
    Serial.begin(115200);
    WIFIUtils::syncWIFIConnect("JSM2", "asde3wsaq1234");

    project_settings.read_from_storage();
    Settings::AppConfig* config = project_settings.getConfig();

    transferWS.set_transfer_response_handler(handle_transfer_response);
    transferWS.set_data_message_handler(handle_data_message);
    transferWS.begin("172.27.11.5", 8000, "/api/storage/smartplant/", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjM0MzIxMTMyMjEsInV1aWQiOiIzZTExYjljZWI1YzI0ODYxOTJiMmMzNzYzM2MyYzkzZiIsInRva2VuX3R5cGUiOiJhY2Nlc3MiLCJ1c2VyX2lkIjoxfQ.GypjsI0tsu8o2Cgsc-NxZJvkcf47Kwu-cljjwameyTE");
}


void loop() {
    transferWS.loop();
}
