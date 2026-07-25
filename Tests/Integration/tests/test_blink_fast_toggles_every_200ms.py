def test_blink_fast_toggles_every_200ms(sm):
    """
    Integration Test: In STATE_BLINK_FAST, the LED should toggle
    every 200 ms according to hal_get_tick().
    """

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

    # Second press → ON → BLINK_SLOW
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # Release
    sm.button_isr_handler(0)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # Third press → BLINK_SLOW → BLINK_FAST
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()
    assert sm.sm_get_state() == 3  # BLINK_FAST

    sm.fake_hal_set_tick(0)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 0

    sm.fake_hal_set_tick(200)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 1

    sm.fake_hal_set_tick(400)
    sm.app_tick()
    assert sm.fake_hal_get_toggle_count() == 2
