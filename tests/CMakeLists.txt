cmake_minimum_required(VERSION 3.16)

project(UnitTests C)

enable_testing()

# # Add debug symbols for VSCode breakpoints
add_compile_options(-g)

include_directories(
    Unity/Inc
    Unit/Inc
    Fakes/Inc
    ../App/Inc
)

# Coverage flags
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Helper macro: creates one executable per test
function(add_unity_test TEST_NAME)
    add_executable(${TEST_NAME}
        Unity/Src/unity.c
        test_runner.c
        Unit/Src/test_state_machine.c
        Fakes/Src/fake_hal.c
        Fakes/Src/fake_tguard.c  
        ../App/Src/state_machine.c
    )

    target_include_directories(${TEST_NAME} PUBLIC
        Unity/Inc
        Unit/Inc
        Fakes/Inc
        ../App/Inc
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Four separate executables, one per test
add_unity_test(test_timer_resets)
add_unity_test(test_state_transitions)
add_unity_test(test_blink_timeout)
add_unity_test(test_next_state_returns_current_when_no_transition)
add_unity_test(test_guard_always_true)