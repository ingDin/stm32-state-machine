cmake_minimum_required(VERSION 3.16)

project(UnitTests C)

enable_testing()

# Include paths
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Unity/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Unit/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Fakes/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../App/Inc
)

# Macro to create separate Unity test executables
# All tests come from the SAME file: test_state_machine.c
function(add_unity_test TEST_NAME TEST_ARG)
    add_executable(${TEST_NAME}
        Unity/Src/unity.c
        run_tests.c
        Unit/Src/test_state_machine.c
        Fakes/Src/fake_hal.c
        ../App/Src/state_machine.c
    )

    target_include_directories(${TEST_NAME} PUBLIC
        Unity/Inc
        Unit/Inc
        Fakes/Inc
        ../App/Inc
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} ${TEST_ARG})
endfunction()

# Three separate tests (ALL using test_state_machine.c)
add_unity_test(test_timer_resets      timer)
add_unity_test(test_state_transitions transitions)
add_unity_test(test_blink_timeout     blink)
