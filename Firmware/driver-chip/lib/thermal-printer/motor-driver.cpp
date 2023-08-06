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

#include "motor-driver.h"

void motor_initialise(const motor_driver *motor) {
    pinMode(motor->pins_enabled_1, OUTPUT);
    digitalWrite(motor->pins_enabled_1, LOW);

    pinMode(motor->pins_enabled_2, OUTPUT);
    digitalWrite(motor->pins_enabled_2, LOW);

    pinMode(motor->pins_in_1, OUTPUT);
    digitalWrite(motor->pins_in_1, LOW);

    pinMode(motor->pins_in_2, OUTPUT);
    digitalWrite(motor->pins_in_2, LOW);
}

void motor_scroll_lines(motor_driver *motor, uint8_t lines) {
    uint16_t ticks = lines * motor->ticks_per_line;
    motor_rotate_forwards(motor, ticks);
}

void motor_rotate_forwards(motor_driver *motor, uint16_t ticks) {
    motor_rotate(motor, ticks, +1);
}

void motor_rotate_backwards(motor_driver *motor, uint16_t ticks) {
    motor_rotate(motor, ticks, -1);
}

void motor_rotate(motor_driver *motor, uint16_t ticks, int8_t step_direction) {
    digitalWrite(motor->pins_enabled_1, HIGH);
    digitalWrite(motor->pins_enabled_2, HIGH);

    for (uint16_t i = 0; i < ticks; i++) {
        switch(motor->current_phase) {
            case 0: {
                digitalWrite(motor->pins_in_1, HIGH);
                digitalWrite(motor->pins_in_2, HIGH);
                break;
            }

            case 1: {
                digitalWrite(motor->pins_in_1, LOW);
                digitalWrite(motor->pins_in_2, HIGH);
                break;
            }

            case 2: {
                digitalWrite(motor->pins_in_1, LOW);
                digitalWrite(motor->pins_in_2, LOW);
                break;
            }

            case 3 : {
                digitalWrite(motor->pins_in_1, HIGH);
                digitalWrite(motor->pins_in_2, LOW);
                break;
            }

            default:
                break;
        }

        motor->current_phase = (motor->current_phase + step_direction) % 4;

        delayMicroseconds(motor->tick_duration_us);
    }

    digitalWrite(motor->pins_enabled_1, LOW);
    digitalWrite(motor->pins_enabled_2, LOW);
}
