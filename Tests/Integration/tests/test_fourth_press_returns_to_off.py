def test_fourth_press_returns_to_off(sm):
    """
    Integration Test: A fourth button press should return the FSM
    from STATE_BLINK_FAST back to STATE_OFF.

    Full cycle:
        OFF → ON → BLINK_SLOW → BLINK_FAST → OFF
    """

    sm.app_init()

    # Helper for debounce ticks
    def ticks(n=3):
        for _ in range(n):
            sm.app_tick()

    # First press → OFF → ON
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 1

    # Release
    sm.fake_button_isr(0)
    ticks()

    # Second press → ON → BLINK_SLOW
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 2

    # Release
    sm.fake_button_isr(0)
    ticks()

    # Third press → BLINK_SLOW → BLINK_FAST
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 3

    # Release
    sm.fake_button_isr(0)
    ticks()

    # Fourth press → BLINK_FAST → OFF
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 0  # STATE_OFF
