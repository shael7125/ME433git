/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define SINE_NUM 100
#define TRI_NUM 100

/* Example code to talk to a bme280 humidity/temperature/pressure sensor.

   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefore SPI) cannot be used at 5v.

   You will need to use a level shifter on the SPI lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board and a generic bme280 board, other
   boards may vary.

   GPIO 16 (pin 21) MISO/spi0_rx-> SDO/SDO on bme280 board
   GPIO 17 (pin 22) Chip select -> CSB/!CS on bme280 board
   GPIO 18 (pin 24) SCK/spi0_sclk -> SCL/SCK on bme280 board
   GPIO 19 (pin 25) MOSI/spi0_tx -> SDA/SDI on bme280 board
   3.3v (pin 36) -> VCC on bme280 board
   GND (pin 38)  -> GND on bme280 board

   Note: SPI devices can have a number of different naming schemes for pins. See
   the Wikipedia page at https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
   for variations.

   This code uses a bunch of register definitions, and some compensation code derived
   from the Bosch datasheet which can be found here.
   https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme280-ds002.pdf
*/

#define READ_BIT 0x80



#ifdef PICO_DEFAULT_SPI_CSN_PIN
static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}
#endif

#if defined(spi_default) && defined(PICO_DEFAULT_SPI_CSN_PIN)
static void write_register(uint8_t reg, uint16_t data) {
    uint8_t buf[2];

    buf[0] = reg;

    // set first four bits of buf based on channel, buffer, gain, and shutdown
    buf[0] = (buf[0]<<7) | 0b1110000;

    // set next four bits based on 10-bit voltage input
    buf[0] = buf[0] | (data >> 6);

    // set equal to last 6 bits of data
    buf[1] = data & 0b0000111111;
    buf[1] = buf[1] << 2;
    
    cs_select();
    spi_write_blocking(spi_default, buf, 2);
    cs_deselect();
    sleep_ms(10);
}


#endif

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
#warning spi/bme280_spi example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else
    printf("Reading raw data from registers via SPI...\n");

    // This example will use SPI0 at 0.5MHz.
    spi_init(spi_default, 12000);
    // gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));

    // create sine wave at 2 Hz
    uint16_t sine[SINE_NUM];
    float step = 2* M_PI / SINE_NUM;
    // float step = 2 * M_PI / (2000);
    for (int i = 0; i < SINE_NUM; i++) {
        sine[i] = (int) 512* sin(2* i * step) + 511;
    }

    // create triangle wave at 1 Hz

    uint16_t tri[TRI_NUM];
    // step = 1 / TRI_NUM;
    // for (int i=0; i < TRI_NUM; i++) {
    //     if (i < TRI_NUM/4){
    //         tri[i] = 511 + (i * step)*511*4;
    //     } else if (i < TRI_NUM/2 && i >= TRI_NUM/4) {
    //         tri[i] = (511*3) - (i * step)*511*4;
    //     } else {
    //         -(511*3) + (i * step)*511*4;
    //     }
    // }

    float period = 0.5; // Period of the triangle wave in seconds
    float frequency = 1.0 / period; // Frequency of the triangle wave in Hz
    float sampling_interval = period / TRI_NUM; // Sampling interval
    float half_period = period / 2.0; // Half of the period

    // Generate triangle wave points
    for (int i = 0; i < TRI_NUM; i++) {
        float t = i * sampling_interval; // Time
        float value = (2.0 / period) * (t - floor(t / period) * period); // Triangle wave function
        if (t > half_period) {
            value = 2.0 - value; // Mirror the triangle wave after half of the period
        }
        tri[i] = (int)(512 * value); // Scale and shift the values to fit in uint16_t range
    }

    while (1) {
        
        for (int j = 0; j < 100; j++) {
            write_register(0,sine[j]);
            write_register(1,tri[j]);
        }

        // write_register(0,0b0100000000);

        // uint8_t buf[2];
        // buf[0] = 0b11110111;  // remove read bit as this is a write
        // buf[1] = 0b11111100;
        // cs_select();
        // spi_write_blocking(spi_default, buf, 2);
        // cs_deselect();
        // sleep_ms(10);

        // sleep_ms(500);
    }
#endif
}
