# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/pedro/esp/esp-idf/components/bootloader/subproject"
  "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader"
  "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix"
  "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix/tmp"
  "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix/src"
  "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/Documentos/UFBA/Curso_online/ESP32/Aciona_Led_Bota_Lcd/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
