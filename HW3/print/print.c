#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");
    
    // initialize GP13 as the LED output pin. LED on with output is high
    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);

    // GP15 is the button input pin. normally lo, when button is pressed goes high
    gpio_init(15);
    gpio_set_dir(15, GPIO_IN);

    adc_init(); // init the adc module
    adc_gpio_init(26); // set ADC0 pin to be adc input instead of GPIO
    adc_select_input(0); // select to read from ADC0

    while (1) {
        gpio_put(13, 1); // turn light on
        while (gpio_get(15) == 0) {} // wait forever until button is pushed
        gpio_put(13, 0); // turn light off
        printf("What number? 1-100\n");
        int num;
        scanf("%d", &num); // recieve number 1-100
        printf("Times: %d\r\n",num); // check with user by printing # to screen
        sleep_ms(1000); // quick delay for user use
        int i;
        for (i = 0; i < num; i++) { // will execute for loop num number of times
            uint16_t result = adc_read(); // reads ADC in counts
            float resultf = result*0.000806; // convert counts to volts (12-bit adc) 0.000806 = 3.3/4096
            printf("ADC= %f\r\n",resultf); // print to screen
            sleep_ms(10); // wait 10 ms = 0.01s = 1/100 = 100 Hz
        }
        
        

    }
}