cmake_minimum_required(VERSION 3.26.4)
project(mqttled VERSION 1.0.0 LANGUAGES C)

find_package(eclipse-paho-mqtt-c REQUIRED)

add_executable(mqttled mqttled.c)
target_link_libraries(mqttled eclipse-paho-mqtt-c::paho-mqtt3c)