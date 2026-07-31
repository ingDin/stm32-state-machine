def test_third_press_transitions_to_blink_fast(sm):
    """
    Integration Test: A third valid button press should transition the FSM
    from BLINK_SLOW to BLINK_FAST.

    Sequence:
        OFF → ON → BLINK_SLOW → BLINK_FAST
    Pattern:
        release → press → release → press → release → press
    """

    sm.app_init()

    # Helper for debounce ticks
    def ticks(n=3):
        for _ in range(n):
            sm.app_tick()

    # OFF → ON
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 1

    # RELEASE
    sm.fake_button_isr(0)
    ticks()

    # ON → BLINK_SLOW
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 2

    # RELEASE
    sm.fake_button_isr(0)
    ticks()

    # BLINK_SLOW → BLINK_FAST
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 3
