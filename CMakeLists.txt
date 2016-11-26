cmake_minimum_required (VERSION 3.7)
project (Fake360Driver)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeScripts)

find_library(LIBUSB1_LIBRARY usb-1.0)
if (APPLE)
    include_directories(/usr/local/include)
endif (APPLE)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES src/*.cpp)

add_library(Fake360Library ${SOURCES})
add_executable(Fake360Test extras/fake360test/main.cpp)
target_link_libraries(Fake360Test Fake360Library ${LIBUSB1_LIBRARY})