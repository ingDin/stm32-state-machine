import ctypes
import pytest
import os
from pathlib import Path


LIB_NAME = "libstm32_sm.dll"


def get_library_path() -> Path:
    """
    Return the path to the Windows DLL.
    Allows override via LIB_STM32_PATH.
    """
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
