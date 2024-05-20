#pragma once
#include <Arduino.h>


struct ErrorDetail {
    uint code;
    const char* message;
};


namespace GenericCodes {
    const ErrorDetail CODE_OK = {1000, "Success"};
    const ErrorDetail CODE_CREATED = {1001, "Created"};
    const ErrorDetail CODE_UPDATED = {2003, "Updated"};
}


namespace GenericErrorCodes {
    const ErrorDetail CODE_INTERNAL_SERVER_ERROR = {2000, "Internal server error"};
    const ErrorDetail CODE_ALREADY_EXISTS = {2001, "Already exists"};
    const ErrorDetail CODE_NOT_FOUND = {2002, "Not found"};
    const ErrorDetail CODE_UNPROCESSABLE_ENTITY = {2003, "Unprocessable entity"};
}

namespace TransferWSCodes {
    const ErrorDetail CODE_UNKNOWN_ACTION = {4001, "Unknown action"};
}
