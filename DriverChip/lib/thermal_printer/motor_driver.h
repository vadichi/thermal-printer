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
