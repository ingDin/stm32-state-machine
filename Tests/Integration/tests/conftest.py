import ctypes
import pytest
import os
import sys

def get_library_path():
    base = os.path.join(os.path.dirname(__file__), "..", "lib")

    if sys.platform.startswith("win"):
        return os.path.join(base, "libstm32_sm.dll")
    elif sys.platform.startswith("linux"):
        return os.path.join(base, "libstm32_sm.so")
    elif sys.platform.startswith("darwin"):
        return os.path.join(base, "libstm32_sm.dylib")
    else:
        raise RuntimeError(f"Unsupported OS: {sys.platform}")

@pytest.fixture(scope="session")
def sm():
    lib_path = get_library_path()
    lib = ctypes.CDLL(lib_path)

    lib.app_init.restype = None
    lib.app_tick.restype = None

    lib.fake_hal_set_tick.argtypes = [ctypes.c_uint32]
    lib.fake_hal_get_toggle_count.restype = ctypes.c_int

    lib.fake_fsm_last_event.restype = ctypes.c_int

    return lib
