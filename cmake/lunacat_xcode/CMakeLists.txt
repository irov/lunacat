CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(lunacat_xcode LANGUAGES C)

INCLUDE(../base_template.cmake)
INCLUDE(../xcode_template.cmake)
INCLUDE(../lunacat_template.cmake)
INCLUDE(../macro_template.cmake)

ADD_SUBDIRECTORY(${LUNACAT_SOURCE_DIRECTORY} ${CMAKE_BINARY_DIR}/src)