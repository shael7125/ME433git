/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "ssd1306.h"



//  // device has default bus address of 0x76
// #define ADDR _u(0b0100000)

// // hardware registers
// #define REG_IODIR _u(0x00)
// #define REG_GPIO _u(0x09)
// #define REG_OLAT _u(0x0A)


// void chip_init() {
//     // use the "handheld device dynamic" optimal setting (see datasheet)
//     uint8_t buf[2];

//     // send register number followed by its corresponding value
//     buf[0] = REG_IODIR;
//     buf[1] = 0b01111111;
//     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); // blocks forever waiting for acknowledge
// }

// void chip_set(char v) {
//     // use the "handheld device dynamic" optimal setting (see datasheet)
//     uint8_t buf[2];

//     // send register number followed by its corresponding value
//     buf[0] = REG_OLAT;
//     buf[1] = v<<7;
//     i2c_write_blocking(i2c_default, ADDR, buf, 2, false); // blocks forever waiting for acknowledge
// }

// int chip_get() {

//     uint8_t buf[1];
//     uint8_t reg = REG_GPIO;
//     i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep master control of bus
//     i2c_read_blocking(i2c_default, ADDR, buf, 1, false);  // false - finished with bus

//     if (buf[0]&0b1==0b1) {
//         return 1;
//     } else {
//         return 0;
//     }
// }


int main() {
    stdio_init_all();

    // I2C is "open drain"
    i2c_init(i2c_default, 100000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    
    // initialize PICO LED
    #ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with a regular LED
    #else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    //initialize the ssd1306
    ssd1306_setup();
    sleep_ms(250); // sleep so that data polling and register update don't collide

    char message[50];
    sprintf(message, "Wow, this was hard");
        

    unsigned int start = to_us_since_boot(get_absolute_time());

    drawMessage(1,1,message);
    ssd1306_update();

    unsigned int stop = to_us_since_boot(get_absolute_time());

    unsigned int t = stop - start;
    sprintf(message, "FPS = %f", 1000000.0/t);
    drawMessage(64,10,message);
    ssd1306_update();
    sleep_ms(250);

    while (1) {
        // heartbeat at 1 Hz
        gpio_put(LED_PIN, 1);
        // ssd1306_drawPixel(1,1,1);
        // ssd1306_update();
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        // ssd1306_drawPixel(1,1,0);
        // ssd1306_update();
        sleep_ms(500);
        // heartbeat at 1 Hz
    }
    #endif
}
