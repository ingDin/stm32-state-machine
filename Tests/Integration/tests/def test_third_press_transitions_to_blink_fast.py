def test_third_press_transitions_to_blink_fast(sm):
    """
    Integration Test: A third button press should transition the FSM
    from STATE_BLINK_SLOW to STATE_BLINK_FAST.

    Sequence:
        OFF → ON → BLINK_SLOW → BLINK_FAST
    """

    sm.app_init()

    # OFF → ON
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # ON → BLINK_SLOW
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    # BLINK_SLOW → BLINK_FAST
    sm.button_isr_handler(1)
    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    assert sm.sm_get_state() == 3  # STATE_BLINK_FAST
