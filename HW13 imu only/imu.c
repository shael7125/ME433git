/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#define ADDR _u(0b1101000)
// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75

int who_am_i() {
    uint8_t buf[1];
    uint8_t reg = WHO_AM_I;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    int val = i2c_read_blocking(i2c_default, ADDR, buf, 1, false);
    return val;
}

void imu_init() {
    uint8_t buf[2];
    buf[0] = PWR_MGMT_1;
    buf[1] = 0x00;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    buf[0] = ACCEL_CONFIG;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    buf[0] = GYRO_CONFIG;
    buf[1] = 0b00011000;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void chip_get(uint8_t *buf) {
    uint8_t reg = ACCEL_XOUT_H;
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, buf, 14, false);
}

void process_data(uint8_t *buf, signed short *data) {
    data[0] = buf[0]<<8;
    data[0] = data[0] | buf[1];
    data[1] = buf[2]<<8;
    data[1] = data[1] | buf[3];
    data[2] = buf[4]<<8;
    data[2] = data[2] | buf[5];
    data[3] = buf[6]<<8;
    data[3] = data[3] | buf[7];
    data[4] = buf[8]<<8;
    data[4] = data[4] | buf[9];
    data[5] = buf[10]<<8;
    data[5] = data[5] | buf[11];
    data[6] = buf[12]<<8;
    data[6] = data[6] | buf[13];
}

int main() {
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    // useful information for picotool
    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 100000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN); // accounted for in hardware
    // gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN); // accounted for in hardware

    // initialize PICO LED
    #ifndef PICO_DEFAULT_LED_PIN
    #warning blink example requires a board with a regular LED
    #else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    who_am_i(); // blocking function, check that the chip is actually giving an address back
    imu_init();

    while (1) {
        uint8_t arr[14];
        chip_get(arr);
        
        // for (int i = 0; i < 14; i++) {
        //     if (i == 13) {
        //         printf("%d\n", arr[i]);
        //     } else {
        //         printf("%d\t", arr[i]);
        //     }
        // }
  
        signed short data[7];
        process_data(arr, data);
        float dataf[7];

        // for (int i = 0; i < 7; i++) {
        //     // printf("%d\n", sizeof(data));
        //     printf("%d\n", data[2]);
        // }
    
        dataf[0] = data[0] * 0.000061;
        dataf[1] = data[1] * 0.000061;
        dataf[2] = data[2] * 0.000061;
        dataf[3] = (data[3]/340.00) + 36.53;
        dataf[4] = data[4] * 0.007630;
        dataf[5] = data[5] * 0.007630;
        dataf[6] = data[6] * 0.007630;

        for (int i = 0; i < 7; i++) {
            if (i == 6) {
                printf("%.3f\n", dataf[i]);
            } else {
                printf("%.3f\t", dataf[i]);
            }
        }

        gpio_put(LED_PIN, 1);
        sleep_ms(5); // read at 100 Hz
        gpio_put(LED_PIN, 0);
        sleep_ms(5);
    }
    #endif
}
