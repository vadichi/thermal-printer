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

#ifndef DRIVERCHIP_MOTOR_DRIVER_H
#define DRIVERCHIP_MOTOR_DRIVER_H

#include <Arduino.h>

typedef struct {
    uint8_t current_phase;

    const uint16_t tick_duration_us;
    const uint8_t ticks_per_line;

    const uint8_t pins_enabled_1;
    const uint8_t pins_enabled_2;
    const uint8_t pins_in_1;
    const uint8_t pins_in_2;
} motor_driver;

void motor_initialise(const motor_driver *motor);

void motor_scroll_lines(motor_driver *motor, uint8_t lines);

void motor_rotate_forwards(motor_driver *motor, uint16_t ticks);
void motor_rotate_backwards(motor_driver *motor, uint16_t ticks);

void motor_rotate(motor_driver *motor, uint16_t ticks, int8_t step_direction);

#endif //DRIVERCHIP_MOTOR_DRIVER_H
