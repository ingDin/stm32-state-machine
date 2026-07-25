import sys
from pathlib import Path

# Directory containing test .c files
test_dir = Path(sys.argv[1])
test_files = test_dir.glob("*.c")

tests = []

for file in test_files:
    lines = file.read_text().splitlines()

    for line in lines:
        stripped = line.strip()

        # Skip comments
        if stripped.startswith("//") or stripped.startswith("/*"):
            continue

        # Detect Unity test functions: void test_xxx(...)
        if stripped.startswith("void test_") and "(" in stripped:
            func_name = stripped.split("(")[0].replace("void", "").strip()
            exe_name = func_name.replace("_parameterized", "")
            tests.append(exe_name)

# Output ONLY the executable names, one per line
for exe_name in tests:
    print(exe_name)
