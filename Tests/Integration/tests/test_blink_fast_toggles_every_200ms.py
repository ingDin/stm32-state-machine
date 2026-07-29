def test_blink_fast_toggles_every_200ms(sm):
    """
    Integration Test: In STATE_BLINK_FAST, the LED should toggle
    every 200 ms according to hal_get_tick().
    """

    sm.app_init()

    # Helper to run debounce ticks
    def ticks(n=3):
        for _ in range(n):
            sm.app_tick()

    # OFF → ON
    sm.button_isr_handler(1)
    ticks()
    assert sm.sm_get_state() == 1

    # RELEASE
    sm.button_isr_handler(0)
    ticks()

    # ON → BLINK_SLOW
    sm.button_isr_handler(1)
    ticks()
    assert sm.sm_get_state() == 2

    # RELEASE
    sm.button_isr_handler(0)
    ticks()

    # BLINK_SLOW → BLINK_FAST
    sm.button_isr_handler(1)
    ticks()
    assert sm.sm_get_state() == 3  # BLINK_FAST

    # Timing checks
    sm.fake_hal_set_tick(0)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 0

    sm.fake_hal_set_tick(200)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 1

    sm.fake_hal_set_tick(400)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 2
