// Copyright 2023 Vadim Chichikalyuk
//
// This file is part of Arduino-Printer
//
// Arduino-Printer is free software: you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Arduino-Printer is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Arduino-Printer. If not, see <https://www.gnu.org/licenses/>.

#include <Arduino.h>
#include <HardwareSerial.h>

#include <motor_driver.h>
#include <printer_driver.h>

printer_driver printer = {
        .clock_duration_us = 0,
        .clock_lead_time_us = 1,
        .latch_duration_us = 16,
        .latch_lag_time_us = 120,
        .strobe_duration_us = 1180 * 2,
        .bold_strobe_duration_us = 1180 * 4,

        .line_width_dots = 288,

        .pins_data = 4,
        .pins_clock = 5,
        .pins_latch = 8,
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

void setup() {
    Serial.begin(115200);

    printer_initialise(&printer);
    motor_initialise(&motor);
}

void handle_scroll_command();
void handle_print_command();

void loop() {
    if (Serial.available() > 0) {
        uint8_t opcode = Serial.read();

        switch(opcode) {
            case 'L': {
                handle_scroll_command();
                break;
            }

            case 'P': {
                handle_print_command();
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
    Serial.write('P');
}
