def test_fourth_press_returns_to_off(sm):
    """
    Integration Test: A fourth button press should return the FSM
    from STATE_BLINK_FAST back to STATE_OFF.

    Full cycle:
        OFF → ON → BLINK_SLOW → BLINK_FAST → OFF
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

    # Release
    sm.button_isr_handler(0)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # Fourth press → BLINK_FAST → OFF
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    assert sm.sm_get_state() == 0  # STATE_OFF
