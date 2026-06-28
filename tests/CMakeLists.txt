enable_testing()

add_executable(unit_tests
    Unity/Src/unity.c
    run_tests.c
    Unit/Src/test_state_machine.c
    Fakes/Src/fake_hal.c
    ../App/Src/state_machine.c
)

target_include_directories(unit_tests PUBLIC
    Unity/Inc
    Unit/Inc
    Fakes/Inc
    ../App/Inc
)

add_test(NAME unit_tests COMMAND unit_tests)
