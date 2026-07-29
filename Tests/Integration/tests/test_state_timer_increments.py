def test_state_timer_increments(sm):
    """
    Integration Test: The state timer should increment on every call
    to sm_tick(), executed inside app_tick().
    """

    sm.app_init()
    assert sm.sm_get_timer() == 0

    # Helper for repeated ticks
    def ticks(n):
        for _ in range(n):
            sm.app_tick()

    ticks(3)

    assert sm.sm_get_timer() == 3
