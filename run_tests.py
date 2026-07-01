import os
import shutil
import subprocess

# Clean build folder
print("Cleaning build folder...")
if os.path.exists("build"):
    shutil.rmtree("build")

os.makedirs("build")
os.chdir("build")

# Configure CMake (MinGW Makefiles)
print("Configuring CMake (MinGW Makefiles)...")
subprocess.check_call([
    "cmake",
    "-G", "MinGW Makefiles",
    "-DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe",
    "-DCMAKE_C_FLAGS=--coverage",
    "-DCMAKE_EXE_LINKER_FLAGS=--coverage",
    ".."
])

# Build tests
print("Building tests...")
subprocess.check_call(["mingw32-make"])

# Run tests
print("Running tests...")
subprocess.check_call(["ctest", "--output-on-failure"])

# Back to project root
os.chdir("..")

print("Done.")
