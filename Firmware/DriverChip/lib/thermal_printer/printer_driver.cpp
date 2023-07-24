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

#include "printer_driver.h"

void printer_initialise(const printer_driver *printer) {
    pinMode(printer->pins_data, OUTPUT);
    digitalWrite(printer->pins_data, LOW);

    pinMode(printer->pins_clock, OUTPUT);
    digitalWrite(printer->pins_clock, LOW);

    pinMode(printer->pins_latch, OUTPUT);
    digitalWrite(printer->pins_latch, HIGH);

    pinMode(printer->pins_strobes_1, OUTPUT);
    digitalWrite(printer->pins_strobes_1, HIGH);

    pinMode(printer->pins_strobes_2, OUTPUT);
    digitalWrite(printer->pins_strobes_2, HIGH);

    pinMode(printer->pins_thermistor, INPUT);
}

void printer_print_line(const printer_driver *printer, const bool *dots, bool enable_bold_font) {
    for (uint16_t i = 0; i < printer->line_width_dots; i++) {
        digitalWrite(printer->pins_data, dots[i]);
        printer_fire_clock(printer);
    }

    printer_fire_latch(printer);
    printer_fire_strobes(printer, enable_bold_font);
}

void printer_fire_clock(const printer_driver *printer) {
    delayMicroseconds(printer->clock_lead_time_us);

    digitalWrite(printer->pins_clock, HIGH);
    delayMicroseconds(printer->clock_duration_us);
    digitalWrite(printer->pins_clock, LOW);
}

void printer_fire_latch(const printer_driver *printer) {
    digitalWrite(printer->pins_latch, LOW);
    delayMicroseconds(printer->latch_duration_us);
    digitalWrite(printer->pins_latch, HIGH);

    delayMicroseconds(printer->latch_lag_time_us);
}

void printer_fire_strobes(const printer_driver *printer, bool enable_bold_font) {
    digitalWrite(printer->pins_strobes_1, LOW);
    digitalWrite(printer->pins_strobes_2, LOW);

    if (!enable_bold_font) {
        delayMicroseconds(printer->strobe_duration_us);
    } else {
        delayMicroseconds(printer->bold_strobe_duration_us);
    }

    digitalWrite(printer->pins_strobes_1, HIGH);
    digitalWrite(printer->pins_strobes_2, HIGH);
}
