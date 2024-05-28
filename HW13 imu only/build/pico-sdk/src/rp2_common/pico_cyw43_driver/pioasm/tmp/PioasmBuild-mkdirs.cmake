# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/shaelmurthy/me433/Pico/pico-sdk/tools/pioasm"
  "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pioasm"
  "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/shaelmurthy/Desktop/ME433git/HW13/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
