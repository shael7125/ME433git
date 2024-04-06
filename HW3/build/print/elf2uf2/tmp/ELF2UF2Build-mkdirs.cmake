# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/shaelmurthy/me433/Pico/pico-sdk/tools/elf2uf2"
  "/Users/shaelmurthy/Desktop/ME433/HW3/build/elf2uf2"
  "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2"
  "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2/tmp"
  "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2/src"
  "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/shaelmurthy/Desktop/ME433/HW3/build/print/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
