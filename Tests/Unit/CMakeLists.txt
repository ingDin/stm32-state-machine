cmake_minimum_required(VERSION 3.16)

project(UnitTests C)

enable_testing()

add_compile_options(-g)

option(DEVELOP "Enable development mode" ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Unity/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Fakes/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../App/Inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

execute_process(
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/extract_tests.py
            ${CMAKE_CURRENT_SOURCE_DIR}/Src
    OUTPUT_VARIABLE UNITY_TEST_LIST
)

string(REPLACE "\n" ";" UNITY_TEST_LIST "${UNITY_TEST_LIST}")

file(GLOB UNITY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Unity/Src/*.c"
)

file(GLOB UNIT_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c"
)

file(GLOB FAKES_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/../Fakes/Src/*.c"
)

foreach(EXE_NAME ${UNITY_TEST_LIST})

    add_executable(${EXE_NAME}
        ${UNITY_SRC}
        ${UNIT_SRC}
        ${FAKES_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_runner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../App/Src/state_machine.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../App/Src/button.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../App/Src/led.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../../App/Src/app.c
    )

    target_include_directories(${EXE_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../Unity/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../Fakes/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../../App/Inc
    )

    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})

endforeach()

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Test_results/TEST)

string(REPLACE "\r\n" "\n" UNITY_TEST_LIST "${UNITY_TEST_LIST}")
string(REPLACE "\r" "\n" UNITY_TEST_LIST "${UNITY_TEST_LIST}")

file(WRITE ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt "")
file(APPEND ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt
     "Unit tests found:\n")
foreach(TEST_NAME ${UNITY_TEST_LIST})
    file(APPEND ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt "- ${TEST_NAME}\n")
endforeach()

find_program(GCOVR gcovr)
if(NOT GCOVR)
    message(WARNING "gcovr not found, coverage targets will be disabled.")
    return()
endif()

set(COVERAGE_DIR ${CMAKE_SOURCE_DIR}/Test_results/COVERAGE)
set(COVERAGE_HTML ${COVERAGE_DIR}/html)

add_custom_target(coverage_clean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${COVERAGE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_HTML}
)

add_custom_target(coverage_xml
    DEPENDS coverage_clean
    COMMAND ${GCOVR}
        -r ${CMAKE_SOURCE_DIR}
        --object-directory ${CMAKE_BINARY_DIR}
        --exclude "${CMAKE_BINARY_DIR}/Tests/Integration"
        --exclude "${CMAKE_BINARY_DIR}/Tests/Integration/*"
        --exclude "${CMAKE_SOURCE_DIR}/Tests/Integration"
        --exclude "${CMAKE_SOURCE_DIR}/Tests/Integration/*"
        --gcov-ignore-errors=no_working_dir_found
        --xml-pretty
        -o ${COVERAGE_DIR}/coverage.xml
)

add_custom_target(coverage_html
    DEPENDS coverage_clean
    COMMAND ${GCOVR}
        -r ${CMAKE_SOURCE_DIR}
        --object-directory ${CMAKE_BINARY_DIR}
        --exclude "${CMAKE_BINARY_DIR}/Tests/Integration"
        --exclude "${CMAKE_BINARY_DIR}/Tests/Integration/*"
        --exclude "${CMAKE_SOURCE_DIR}/Tests/Integration"
        --exclude "${CMAKE_SOURCE_DIR}/Tests/Integration/*"
        --gcov-ignore-errors=no_working_dir_found
        --html --html-details
        -o ${COVERAGE_HTML}/index.html
)

add_custom_target(coverage
    DEPENDS coverage_xml coverage_html
)
