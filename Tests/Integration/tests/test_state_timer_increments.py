def test_state_timer_increments(sm):
    """
    Integration Test: The state timer should increment on every call
    to sm_tick(), which is invoked inside app_tick().

    This test ensures the internal timer behaves correctly.
    """

    sm.app_init()
    assert sm.sm_get_timer() == 0

    sm.app_tick()
    sm.app_tick()
    sm.app_tick()

    assert sm.sm_get_timer() == 3
