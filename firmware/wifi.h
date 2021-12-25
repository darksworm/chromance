#pragma once
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include "credentials.h"
#include "animations/wifi_loading.h"

#ifndef WIFI_SSID
#pragma message "Please define WIFI_SSID"
exit(1);
#endif

#ifndef WIFI_PASS
#pragma message "Please define WIFI_PASS"
exit(1);
#endif

namespace Wifi {
    void connect() {
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
            WifiLoading::step();
            LEDs::delay(16);
        }
    }
}
