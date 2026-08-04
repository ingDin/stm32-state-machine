import ctypes
import pytest
import os
from pathlib import Path


LIB_NAME = "libstm32_sm.dll"


def get_library_path() -> Path:
    override = os.getenv("LIB_STM32_PATH")
    if override:
        return Path(override).resolve()

    base_dir = Path(__file__).resolve().parent / ".." / "lib"
    lib_path = base_dir / LIB_NAME

    if not lib_path.exists():
        raise FileNotFoundError(
            f"Library not found: {lib_path}\n"
            f"Override with LIB_STM32_PATH if needed."
        )

    return lib_path


@pytest.fixture(scope="session")
def sm():
    lib_path = get_library_path()
    lib = ctypes.CDLL(str(lib_path))
    return lib


# ------------------------------------------------------------
# CAN FIXTURE 
# ------------------------------------------------------------

class CanFrame(ctypes.Structure):
    _fields_ = [
        ("id", ctypes.c_uint32),
        ("dlc", ctypes.c_uint8),
        ("data", ctypes.c_uint8 * 8),
    ]


class CanDriver(ctypes.Structure):
    _fields_ = [
        ("init", ctypes.CFUNCTYPE(ctypes.c_bool)),
        ("send", ctypes.CFUNCTYPE(ctypes.c_bool, ctypes.POINTER(CanFrame))),
        ("receive", ctypes.CFUNCTYPE(ctypes.c_bool, ctypes.POINTER(CanFrame))),
    ]


@pytest.fixture(scope="session")
def can(sm):
    """CAN loopback driver using exported CAN_LOOPBACK struct."""
    driver = CanDriver.in_dll(sm, "CAN_LOOPBACK")

    assert driver.init(), "CAN init failed"

    return driver
