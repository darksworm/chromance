#pragma once
#include "ESPTelnet.h"

namespace Telnet {

struct MessageHandler {
    String message;
    void (*handle)(String payload);
};

ESPTelnet telnet;
MessageHandler *messageHandlers = NULL;

void onConnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" connected");
    telnet.println("\nWelcome " + telnet.getIP());
    telnet.println("(Use CTRL + ] and q  to disconnect.)");
}

void onDisconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" disconnected");
}

void onReconnect(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" reconnected");
}

void onConnectionAttempt(String ip) {
    Serial.print("- Telnet: ");
    Serial.print(ip);
    Serial.println(" tried to connected");
}

void setup(MessageHandler* handlers = NULL) {
    Serial.begin(9600);
    while (!Serial) { }

    // passing on functions for various telnet events
    telnet.onConnect(onConnect);
    telnet.onConnectionAttempt(onConnectionAttempt);
    telnet.onReconnect(onReconnect);
    telnet.onDisconnect(onDisconnect);

    messageHandlers = handlers;

    // passing a lambda function
    telnet.onInputReceived([](String message) {
        if(messageHandlers != NULL) {
            for (auto handler = messageHandlers; handler->message != ""; handler++) {
                if (message == handler->message) {
                    handler->handle("");
                    return;
                } else if (handler->message.endsWith("*")) {
                    if (message.startsWith(handler->message.substring(0, handler->message.length() - 2))) {
                        handler->handle(message.substring(handler->message.length() - 2));
                        return;
                    }
                }
            }
        }
        telnet.println("I don't know this \"" + message + "\" you're talking of.");
    });

    telnet.begin();
}

void println(String msg) {
    telnet.println(msg);
}

void loop() {
    telnet.loop();

    if (Serial.available()) {
        telnet.print(Serial.read());
    }
}
};
