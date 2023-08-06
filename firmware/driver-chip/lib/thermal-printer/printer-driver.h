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

#ifndef DRIVERCHIP_PRINTER_DRIVER_H
#define DRIVERCHIP_PRINTER_DRIVER_H

#include <Arduino.h>

typedef struct {
    const uint16_t clock_duration_us;
    const uint16_t clock_lead_time_us;
    const uint16_t latch_duration_us;
    const uint16_t latch_lag_time_us;
    const uint16_t strobe_duration_us;
    const uint16_t bold_strobe_duration_us;

    const uint16_t line_width_dots;

    const uint8_t pins_data;
    const uint8_t pins_clock;
    const uint8_t pins_latch;
    const uint8_t pins_strobes_1;
    const uint8_t pins_strobes_2;
    const uint8_t pins_thermistor;
} printer_driver;

void printer_initialise(const printer_driver *printer);

void printer_print_line(const printer_driver *printer, const bool *dots, bool enable_bold_font);

void printer_fire_clock(const printer_driver *printer);
void printer_fire_latch(const printer_driver *printer);
void printer_fire_strobes(const printer_driver *printer, bool enable_bold_font);

#endif //DRIVERCHIP_PRINTER_DRIVER_H
