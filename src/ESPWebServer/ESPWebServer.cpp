#include <ArduinoJson.h>
#include "ESPWebServer.h"
#include "MainPageHTML.h"
#include "settings/Settings.h"


namespace ESPWeb {
    void ESPWebServer::begin() {
        _setup_routes();
        _server.begin();
    }

    void ESPWebServer::serve() {
        _server.handleClient();
    }

    void ESPWebServer::_setup_routes() {
        _server.on("/", HTTPMethod::HTTP_GET, std::bind(&ESPWebServer::_handler_main_get, this));
        _server.on("/settings/", HTTPMethod::HTTP_GET, std::bind(&ESPWebServer::_handler_settings_get, this));
        _server.on("/settings/", HTTPMethod::HTTP_POST, std::bind(&ESPWebServer::_handler_update_settings_port, this));
    }

    StatusResponse ESPWebServer::_update_config(String& json_string) {
        JsonDocument document;
        DeserializationError error = ArduinoJson::deserializeJson(document, json_string);

        if (error) {
            Serial.println("Error while parsing json");
            Serial.println(error.f_str());
            return {false, 400, 100, "Failed to parse Json"};
        }

        Settings::AppConfig* config = project_settings.getConfig();
        config->transfer_server_port = document["transfer_server_port"];
        config->transfer_server_host = document["transfer_server_host"].as<String>();

        config->esp_wifi_ssid = document["esp_wifi_ssid"].as<String>();
        config->esp_wifi_password = document["esp_wifi_password"].as<String>();

        config->user_wifi_ssid = document["user_wifi_ssid"].as<String>();
        config->user_wifi_password = document["user_wifi_password"].as<String>();

        config->transfer_server_login = document["transfer_server_login"].as<String>();
        config->transfer_server_password = document["transfer_server_password"].as<String>();

        Serial.printf("Updated config field transfer_server_port: %d\n", config->transfer_server_port);
        Serial.printf("Updated config field transfer_server_host: %s\n", config->transfer_server_host.c_str());
        Serial.printf("Updated config field esp_wifi_name: %s\n", config->esp_wifi_ssid.c_str());
        Serial.printf("Updated config field esp_wifi_password: %s\n", config->esp_wifi_password.c_str());
        Serial.printf("Updated config field user_wifi_name: %s\n", config->user_wifi_ssid.c_str());
        Serial.printf("Updated config field user_wifi_password: %s\n", config->user_wifi_password.c_str());
        Serial.printf("Updated config field transfer_server_login: %s\n", config->transfer_server_login.c_str());
        Serial.printf("Updated config field transfer_server_password: %s\n", config->transfer_server_password.c_str());

        project_settings.save();

        return {true, 200, 200, "Updated"};
    }

    void ESPWebServer::_handler_main_get() {
        Serial.println("Requested main");
        _server.send_P(200, "text/html", MAIN_PAGE_HTML);
    }

    void ESPWebServer::_handler_settings_get() {
        Serial.println("Requested for current settings");

        Settings::AppConfig* config = project_settings.getConfig();

        JsonDocument document;

        document["ok"] = true;
        document["code"] = 200;
        document["message"] = "OK";

        JsonObject detail = document["detail"].to<JsonObject>();
        detail["transfer_server_port"] = config->transfer_server_port;
        detail["transfer_server_host"] = config->transfer_server_host;
        detail["esp_wifi_ssid"] = config->esp_wifi_ssid;
        detail["esp_wifi_password"] = config->esp_wifi_password;
        detail["user_wifi_ssid"] = config->user_wifi_ssid;
        detail["user_wifi_password"] = config->user_wifi_password;
        detail["transfer_server_login"] = config->transfer_server_login;
        detail["transfer_server_password"] = config->transfer_server_password;

        String jsonString;
        serializeJson(document, jsonString);

        _server.send(200, "application/json", jsonString);
    }

    void ESPWebServer::_handler_update_settings_port() {
        Serial.println("Requested for settings update");

        String data = _server.arg("plain");
        StatusResponse response = _update_config(data);

        JsonDocument doc;
        doc["ok"] = response.ok;
        doc["code"] = response.code;
        doc["message"] = response.message;

        String jsonString;
        serializeJson(doc, jsonString);

        _server.send(response.http_status, "application/json", jsonString);
    }
}


ESPWeb::ESPWebServer web_server;
