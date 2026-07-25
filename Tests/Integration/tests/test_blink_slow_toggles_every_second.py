def test_blink_slow_toggles_every_second(sm):
    sm.app_init()

    # First press → OFF → ON
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # Release
    sm.button_isr_handler(0)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # Reset tick BEFORE entering BLINK_SLOW
    sm.fake_hal_set_tick(0)

    # Second press → ON → BLINK_SLOW
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()
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
