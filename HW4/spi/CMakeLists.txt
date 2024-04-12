add_executable(bme280_spi
        bme280_spi.c
        )

# pull in common dependencies and additional spi hardware support
target_link_libraries(bme280_spi pico_stdlib hardware_spi)

pico_enable_stdio_usb(bme280_spi 1)
pico_enable_stdio_uart(bme280_spi 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(bme280_spi)
