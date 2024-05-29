#pragma once

#include <Arduino.h>

namespace utils {
    String generateRandomString() {
        const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        String randomString = "";

        for (int i = 0; i < 6; i++) {
            int index = random(0, sizeof(charset) - 1);
            randomString += charset[index];
        }

        return randomString;
    }
}
