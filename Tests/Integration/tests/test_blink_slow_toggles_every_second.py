def test_blink_slow_toggles_every_second(sm):
    """
    Integration Test: In STATE_BLINK_SLOW, the LED should toggle
    every 1000 ms according to hal_get_tick().

    Sequence:
        OFF → ON → BLINK_SLOW
    Timing:
        tick = 1000 ms → 1 toggle
        tick = 2000 ms → 2 toggles
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

    # Reset tick BEFORE entering BLINK_SLOW
    sm.fake_hal_set_tick(0)

    # Second press → ON → BLINK_SLOW
    sm.fake_button_isr(1)
    ticks()
    assert sm.sm_get_state() == 2  # BLINK_SLOW

    # Reset toggle counter
    sm.fake_hal_reset_toggle_count()

    # Now simulate 1000 ms
    sm.fake_hal_set_tick(1000)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 1

    # Next 1000 ms
    sm.fake_hal_set_tick(2000)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 2
