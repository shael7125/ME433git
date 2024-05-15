/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/binary_info.h"
#define PWMPIN 16

void servo_init() {
    gpio_set_function(PWMPIN, GPIO_FUNC_PWM); // Set Pin GP16 to be PWM
    uint slice_num = pwm_gpio_to_slice_num(PWMPIN); // Get PWM slice number
    float div = 40; // gives frq = 50 Hz
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
    // pwm_set_gpio_level(PWMPIN, wrap / 16); // set the duty cycle to 50%
}
volatile char m[100];

void set_angle(float per) {
    uint16_t wrap = 62500;
    // if (ang > 180) {
    //     ang = 180;
    // } else (ang < 0); {
    //     ang = 0;
    // }
    pwm_set_gpio_level(PWMPIN, wrap * (per/100));
}

int main() {
    stdio_init_all();
    
    // initialize PICO LED
    #ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with a regular LED
    #else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    servo_init();

    while (1) {
        // heartbeat at 1 Hz
        gpio_put(LED_PIN, 1);
        set_angle(2.5);
        sleep_ms(2000);
        gpio_put(LED_PIN, 0);
        set_angle(12);
        sleep_ms(2000);
        // heartbeat at 1 Hz
    }
    #endif
}
