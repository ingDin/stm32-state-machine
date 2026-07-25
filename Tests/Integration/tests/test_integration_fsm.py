def test_press_triggers_fsm(sm):
    """
    Integration Test: Button press should transition the real FSM
    from STATE_OFF to STATE_ON.

    This test validates the full application pipeline:
        - the button ISR updates the raw input level
        - app_tick() performs debounce processing
        - after three consecutive ticks, the stable state changes
        - the registered callback (sm_handle_event) receives EVENT_BTN_PRESS
        - the real FSM transitions from OFF → ON

    Note:
        Integration tests use the real FSM, not the fake_fsm stub.
        Therefore, the correct assertion is sm_get_state(), not fake_fsm_last_event().
    """

    # Initialize the application (sets callback to sm_handle_event)
    sm.app_init()

    # Simulate a button press (raw_level = 1)
    sm.button_isr_handler(1)

    # Debounce requires three consecutive ticks
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # After debounce completes, the FSM should be in STATE_ON (value 1)
    assert sm.sm_get_state() == 1
