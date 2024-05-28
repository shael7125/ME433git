/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"

#include "hardware/pwm.h"

#define PWM1 16
#define OUT1 17
#define PWM2 18
#define OUT2 19
#define CUTOFF 30
#define FLOOR 25


void motor_init() {
    // Set Pin GP16 to be PWM, goes to IN1
    gpio_set_function(PWM1, GPIO_FUNC_PWM); 
    uint slice_num = pwm_gpio_to_slice_num(PWM1); // Get PWM slice number
    float div = 1000; // 
    pwm_set_clkdiv(slice_num, div); // divider
    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM

    // Set Pin GP17 to be a digital output, goes to IN2
    gpio_init(OUT1);
    gpio_set_dir(OUT1, GPIO_OUT);

    // Set Pin GP18 to be PWM, goes to IN3
    gpio_set_function(PWM2, GPIO_FUNC_PWM); 
    slice_num = pwm_gpio_to_slice_num(PWM2); // Get PWM slice number
    div = 1000; // gives frq = 1250 Hz
    pwm_set_clkdiv(slice_num, div); // divider
    wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM

    // Set Pin GP19 to be a digital output, goes to IN4
    gpio_init(OUT2);
    gpio_set_dir(OUT2, GPIO_OUT);
    
    // set both output pins low, no reverse direction needed!
    gpio_put(OUT1, 0);
    gpio_put(OUT2, 0);
}

void set_duty(float *buf, float *buf2, int num) {
    pwm_set_gpio_level(PWM1, 62500 * buf[num]);
    pwm_set_gpio_level(PWM2, 62500 * buf2[num]);
}

int main() {
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");

    motor_init();

    // initialize PICO LED
    #ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with a regular LED
    #else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    float left_duty[101];
    float right_duty[101];
    for (int i = 0; i < 101; i++) {
        if (i <= CUTOFF) {
            left_duty[i] = 0 + (float) i/CUTOFF;
            right_duty[i] = 1;
        } else if (i > CUTOFF && i < (100-CUTOFF)) {
            left_duty[i] = 1;
            right_duty[i] = 1;
        } else {
            left_duty[i] = 1;
            right_duty[i] = ((100-CUTOFF) * ((float) 1/CUTOFF) + 1) - ((float) i/CUTOFF);
            // printf("%.2f\r\n",right_duty[i]);
        }
    }

    // printf("%.2f\r\n",left_duty[20]);

    while (1) {
        int num;
        printf("What number? 1-100\n");
        scanf("%d", &num); // recieve number 1-100
        printf("Number: %d\r\n",num); // check with user by printing # to screen
        set_duty(left_duty, right_duty, num);

        gpio_put(LED_PIN, 1);
        sleep_ms(1500); // wait three seconds before asking for another input
        gpio_put(LED_PIN, 0);
        sleep_ms(1500);
    }
    #endif
}
