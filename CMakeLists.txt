cmake_minimum_required(VERSION 3.22)
project(control_led C)

enable_testing()

add_subdirectory(App)
add_subdirectory(Tests/Unit)
add_subdirectory(Tests/Integration)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)