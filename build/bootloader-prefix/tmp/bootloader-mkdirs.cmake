# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/user/esp/esp-idf/components/bootloader/subproject"
  "D:/Arduino/ESP-C3/build/bootloader"
  "D:/Arduino/ESP-C3/build/bootloader-prefix"
  "D:/Arduino/ESP-C3/build/bootloader-prefix/tmp"
  "D:/Arduino/ESP-C3/build/bootloader-prefix/src/bootloader-stamp"
  "D:/Arduino/ESP-C3/build/bootloader-prefix/src"
  "D:/Arduino/ESP-C3/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Arduino/ESP-C3/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Arduino/ESP-C3/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
