#pragma once

#include <WebServer.h>

#include <utility>
#include "settings/Settings.h"


namespace ESPWeb {
    struct StatusResponse {
        bool ok;
        int http_status;
        int code;
        String message;

        StatusResponse(
            const bool& ok,
            const int& http_status,
            const int& code,
            String message
        ) : ok(ok), http_status(http_status), code(code), message(std::move(message)) {}
    };

    class ESPWebServer {
    public:
        void begin();

        void serve();

    protected:
        static StatusResponse _update_config(String& json_string);

        void _setup_routes();

        void _handler_main_get();

        void _handler_settings_get();

        void _handler_update_settings_port();

        WebServer _server;
    };
}


extern ESPWeb::ESPWebServer web_server;
