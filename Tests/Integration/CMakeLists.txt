cmake_minimum_required(VERSION 3.16)
project(IntegrationTests C)

enable_testing()

# ---------------------------------------------------------
# Enable TEST mode (for DLL)
# ---------------------------------------------------------
# IMPORTANT: define TEST=1 on the target, not globally
# so that button.c, fake_button_isr.c, fake_hal.c etc. all see it.
# ---------------------------------------------------------

# ---------------------------------------------------------
# Build shared library for Python integration tests
# ---------------------------------------------------------
add_library(stm32_sm SHARED
    ../../App/Src/state_machine.c
    ../../App/Src/button.c
    ../../App/Src/led.c
    ../../App/Src/app.c
)

# TEST=1 must be applied to the target, not globally
target_compile_definitions(stm32_sm PUBLIC TEST=1)

# ---------------------------------------------------------
# In TEST mode, use ALL FAKE SOURCES automatically
# ---------------------------------------------------------
file(GLOB FAKE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../Fakes/Src/*.c"
)

target_sources(stm32_sm PRIVATE ${FAKE_SOURCES})

# ---------------------------------------------------------
# Include directories
# ---------------------------------------------------------
target_include_directories(stm32_sm PUBLIC
    ../../App/Inc
    ../Fakes/Inc
)

# ---------------------------------------------------------
# Output DLL into Integration/lib
# ---------------------------------------------------------
set_target_properties(stm32_sm PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# ---------------------------------------------------------
# Collect pytest tests
# ---------------------------------------------------------
execute_process(
    COMMAND pytest --collect-only -q
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE PYTEST_RAW
)

# Normalize line endings
string(REPLACE "\r" "" PYTEST_RAW "${PYTEST_RAW}")
string(REPLACE "\n" ";" PYTEST_LIST "${PYTEST_RAW}")

# Filter only valid pytest test names
set(PYTEST_FILTERED "")

foreach(LINE ${PYTEST_LIST})
    if(LINE MATCHES ".*\\.py::.*")
        list(APPEND PYTEST_FILTERED "${LINE}")
    endif()
endforeach()

# ---------------------------------------------------------
# Write separation line + header to test_list.txt
# ---------------------------------------------------------
file(APPEND ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt "\n")
file(APPEND ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt
    "Integration tests found:\n")

# ---------------------------------------------------------
# Add each pytest test individually
# ---------------------------------------------------------
foreach(PYTEST_NAME ${PYTEST_FILTERED})
    # Extract only the test name after ::
    string(REGEX REPLACE ".*::" "" PYTEST_SHORT "${PYTEST_NAME}")

    # Add test to CTest
    add_test(NAME ${PYTEST_SHORT}
        COMMAND pytest -q ${PYTEST_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # Write to test_list.txt in same format as unit tests
    file(APPEND ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt
        "- ${PYTEST_SHORT}\n")
endforeach()
