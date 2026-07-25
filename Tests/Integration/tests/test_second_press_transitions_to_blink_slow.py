def test_second_press_transitions_to_blink_slow(sm):
    """
    Integration Test: A second button press should transition the FSM
    from STATE_ON to STATE_BLINK_SLOW.

    A valid press sequence requires:
        release → press → release → press
    """

    sm.app_init()

    # First press → OFF → ON
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()
    assert sm.sm_get_state() == 1  # STATE_ON

    # Release before second press
    sm.button_isr_handler(0)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # Second press → ON → BLINK_SLOW
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    assert sm.sm_get_state() == 2  # STATE_BLINK_SLOW
