// Copyright 2023 Vadim Chichikalyuk
//
// This file is part of Thermal Printer
//
// Thermal Printer is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later version.
//
// Thermal Printer is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with Thermal Printer. If not, see <https://www.gnu.org/licenses/>.

#include <Arduino.h>

// Do not remove — this is required for a clean build to pass
#include <Hash.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

WebSocketsServer server(8080);

void web_socket_handle_event(uint8_t client_number, WStype_t event_type, const uint8_t *message_buffer, size_t message_length);
void web_socket_handle_message(uint8_t client_number, const uint8_t *message_buffer, uint8_t message_length);

void setup() {
    Serial.begin(115200);

    WiFi.softAP("GreenFrost Setup", nullptr, 1, false, 1);

    MDNS.begin("green-frost-printer");
    MDNS.addService("ws", "tcp", 8080);

    server.onEvent(web_socket_handle_event);
    server.begin();
}

void loop() {
    server.loop();
    MDNS.update();
}

void web_socket_handle_event(uint8_t client_number, WStype_t event_type, const uint8_t *message_buffer, size_t message_length) {
    switch(event_type) {
        case WStype_CONNECTED: {
            uint8_t response = 'C';
            server.sendBIN(client_number, &response, 1);

            break;
        }

        case WStype_BIN: {
            if (message_length == 0) return;
            web_socket_handle_message(client_number, message_buffer, (uint8_t) message_length);

            break;
        }

        default:
            break;
    }
}

void web_socket_handle_message(uint8_t client_number, const uint8_t *message_buffer, uint8_t message_length) {
    for (uint8_t i = 0; i < message_length; i++) {
        Serial.write(message_buffer[i]);
    }

    uint8_t response = 0;
    do {
        while(Serial.available() < 1);

        response = (uint8_t) Serial.read();
        server.sendBIN(client_number, &response, 1);
    } while (response != 'S');
}
