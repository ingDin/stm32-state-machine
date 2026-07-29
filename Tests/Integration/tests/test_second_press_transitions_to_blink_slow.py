def test_second_press_transitions_to_blink_slow(sm):
    """
    Integration Test: A second button press should transition the FSM
    from STATE_ON to STATE_BLINK_SLOW.

    Required pattern:
        release → press → release → press
    """

    sm.app_init()

    # Helper for debounce ticks
    def ticks(n=3):
        for _ in range(n):
            sm.app_tick()

    # First press → OFF → ON
    sm.button_isr_handler(1)
    ticks()
    assert sm.sm_get_state() == 1  # STATE_ON

    # Release before second press
    sm.button_isr_handler(0)
    ticks()

    # Second press → ON → BLINK_SLOW
    sm.button_isr_handler(1)
    ticks()
    assert sm.sm_get_state() == 2  # STATE_BLINK_SLOW
