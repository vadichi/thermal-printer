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
#include <SimpleKeypad.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

#include <motor-driver.h>
#include <printer-driver.h>

printer_driver printer = {
        .clock_duration_us = 0,
        .clock_lead_time_us = 1,
        .latch_duration_us = 16,
        .latch_lag_time_us = 120,
        .strobe_duration_us = 1180 * 4,
        .bold_strobe_duration_us = 1180 * 8,

        .line_width_dots = 288,

        .pins_data = 4,
        .pins_clock = 5,
        .pins_latch = 13,
        .pins_strobes_1 = 7,
        .pins_strobes_2 = 6,
        .pins_thermistor = A0,
};

motor_driver motor = {
        .current_phase = 0,

        .tick_duration_us = 5000,
        .ticks_per_line = 10,

        .pins_enabled_1 = 12,
        .pins_enabled_2 = 11,
        .pins_in_1 = 9,
        .pins_in_2 = 10,
};

LiquidCrystal_I2C display(0x27, 16, 2);

char keypad_setup[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'},
};

uint8_t row_pins[4] = {A2, A3, A4, A5};
uint8_t column_pins[4] = {A1, 3, 2, 8};

SimpleKeypad keypad((char*) keypad_setup, row_pins, column_pins, 4, 4);

void setup() {
    Serial.begin(115200);

    printer_initialise(&printer);
    motor_initialise(&motor);

    display.init();
    display.backlight();
    display.noCursor();
}

void handle_scroll_command();
void handle_print_command();
void handle_display_clear_command();
void handle_display_set_cursor_command();
void handle_display_print_text_command();
void handle_display_print_number_command();
void handle_keypad_terminated_input_command();
void handle_keypad_fixed_length_input_command();

void loop() {
    if (Serial.available() > 0) {
        uint8_t opcode = (uint8_t) Serial.read();

        switch(opcode) {
            case 'L': {
                handle_scroll_command();
                break;
            }

            case 'P': {
                handle_print_command();
                break;
            }

            case 'C': {
                handle_display_clear_command();
                break;
            }

            case 'M': {
                handle_display_set_cursor_command();
                break;
            }

            case 'W': {
                handle_display_print_text_command();
                break;
            }

            case 'N': {
                handle_display_print_number_command();
                break;
            }

            case 'T': {
                handle_keypad_terminated_input_command();
                break;
            }

            case 'F': {
                handle_keypad_fixed_length_input_command();
                break;
            }

            default:
                break;
        }
    }
}

void handle_scroll_command() {
    while (Serial.available() < 1);
    uint8_t lines = Serial.read();
    motor_scroll_lines(&motor, lines);

    Serial.write('S');
}

void handle_print_command() {
    while (Serial.available() < 1);
    bool enable_bold_font = (Serial.read() == 'B');

    uint8_t line_width_bytes = printer.line_width_dots / 8;
    if (printer.line_width_dots % 8 != 0) line_width_bytes += 1;

    uint8_t line_bytes[line_width_bytes];
    for (uint8_t i = 0; i < line_width_bytes; i++) {
        while (Serial.available() < 1);
        line_bytes[i] = Serial.read();
    }

    bool line_bits[printer.line_width_dots];
    for (uint16_t bit_number = 0; bit_number < printer.line_width_dots; bit_number++) {
        uint8_t byte_number = bit_number / 8;
        uint8_t sub_bit_number = bit_number % 8;

        uint8_t byte = line_bytes[byte_number];
        uint8_t mask = 0b00000001 << sub_bit_number;

        line_bits[bit_number] = ((byte & mask) != 0);
    }

    printer_print_line(&printer, line_bits, enable_bold_font);

    motor_scroll_lines(&motor,1);
    Serial.write('S');
}

void handle_display_clear_command() {
    display.clear();

    Serial.write('S');
}

void handle_display_set_cursor_command() {
    while (Serial.available() < 1);
    uint8_t column = (uint8_t) Serial.read();

    while (Serial.available() < 1);
    uint8_t row = (uint8_t) Serial.read();

    display.setCursor(column, row);

    Serial.write('S');
}

void handle_display_print_text_command() {
    while (Serial.available() < 1);
    uint8_t length = (uint8_t) Serial.read();

    for (uint8_t i = 0; i < length; i++) {
        while (Serial.available() < 1);
        uint8_t character = (uint8_t) Serial.read();

        display.write(character);
    }

    Serial.write('S');
}

void handle_display_print_number_command() {
    while (Serial.available() < 1);
    uint8_t base = (uint8_t) Serial.read();

    int32_t number = 0;
    for (uint8_t i = 0; i < 4; i++) {
        while (Serial.available() < 1);

        uint8_t byte = (uint8_t) Serial.read();
        number = (number << 8) + byte;
    }

    display.print(number, base);

    Serial.write('S');
}

void handle_keypad_terminated_input_command() {
    while (Serial.available() < 1);
    char terminator = (char) Serial.read();

    char character;
    do {
        character = keypad.getKey();

        if (character != 0) {
            Serial.write(character);
        }
    } while (character != terminator);

    Serial.write('S');
}

void handle_keypad_fixed_length_input_command() {
    while (Serial.available() < 1);
    uint8_t length = (uint8_t) Serial.read();

    for (uint8_t i = 0; i < length; i++) {
        char character;
        do { character = keypad.getKey(); } while (character == 0);

        Serial.write(character);
    }

    Serial.write('S');
}
