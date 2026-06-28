Write-Host "Cleaning build folder..."
if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}

New-Item -ItemType Directory -Path "build" | Out-Null
Set-Location "build"

Write-Host "Configuring CMake (MinGW Makefiles)..."
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe ..

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed."
    exit 1
}

Write-Host "Building tests..."
mingw32-make

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed."
    exit 1
}

Write-Host "Running tests..."
ctest --output-on-failure

Set-Location ".."
Write-Host "Done!"
