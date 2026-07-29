def test_press_triggers_fsm(sm):
    """
    Integration Test: Button press should transition the real FSM
    from STATE_OFF to STATE_ON.

    Pipeline:
        ISR updates raw level
        debounce stabilizes after 3 ticks
        EVENT_BTN_PRESS is emitted
        FSM transitions OFF → ON
    """

    sm.app_init()

    # Helper for debounce ticks
    def ticks(n=3):
        for _ in range(n):
            sm.app_tick()

    # Simulate a button press
    sm.button_isr_handler(1)

    # Debounce
    ticks()

    # FSM should now be in STATE_ON
    assert sm.sm_get_state() == 1
