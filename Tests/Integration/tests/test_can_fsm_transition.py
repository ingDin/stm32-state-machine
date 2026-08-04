import ctypes
from conftest import CanFrame


def test_can_fsm_transition(sm, can):
    """
    Integration Test: A debounced button press must generate a state
    transition in the FSM, and the FSM must transmit a CAN frame
    describing the transition.

    Flow:
        1. app_init() sets up CAN, button, and FSM.
        2. fake_button_isr(1) injects a raw button press in TEST mode.
        3. Three calls to button_tick() satisfy the debounce threshold.
        4. sm_tick() processes the resulting EVENT_BTN_PRESS.
        5. The FSM transitions OFF → ON and sends a CAN frame:
               id = 0x100
               dlc = 2
               data = { previous_state, new_state }

    The test reads the CAN frame BEFORE app_tick() drains the RX buffer.
    """

    sm.app_init()

    # Simulate raw button press
    sm.fake_button_isr.argtypes = [ctypes.c_int]
    sm.fake_button_isr(1)

    # Run debounce ticks
    for _ in range(3):
        sm.button_tick()   # call button_tick() directly

    # FSM transition happens here
    sm.sm_tick()

    # NOW read CAN frame BEFORE app_tick() drains it
    rx = CanFrame()
    assert can.receive(rx), "FSM did not send CAN frame"

    assert rx.id == 0x100
    assert rx.dlc == 2
    assert rx.data[0] == 0
    assert rx.data[1] == 1
