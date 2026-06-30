Write-Host "Cleaning build folder..."
if (Test-Path "build") {
    cmd /c rmdir /s /q build
}

New-Item -ItemType Directory -Path "build" | Out-Null
Set-Location "build"

Write-Host "Configuring CMake (MinGW Makefiles)..."
cmake -G "MinGW Makefiles" `
      -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe `
      -DCMAKE_C_FLAGS="--coverage" `
      -DCMAKE_EXE_LINKER_FLAGS="--coverage" ..

Write-Host "Building tests..."
mingw32-make

Write-Host "Running tests..."
ctest --output-on-failure

# back to project root
Set-Location ..

Write-Host "Preparing test_results folder..."

$testResults = "test_results"
if (Test-Path $testResults) {
    cmd /c rmdir /s /q $testResults
}
New-Item -ItemType Directory -Path $testResults | Out-Null

$coverageRoot = "$testResults/coverage"
New-Item -ItemType Directory -Path $coverageRoot | Out-Null

$htmlDir = "$coverageRoot/html"
New-Item -ItemType Directory -Path $htmlDir | Out-Null

Write-Host "Generating LCOV coverage.info..."

gcovr -r . `
       --object-directory build `
       --lcov `
       -o "$coverageRoot/coverage.info"

Write-Host "Generating HTML coverage report..."

gcovr -r . `
       --object-directory build `
       --html --html-details `
       -o "$htmlDir/index.html"

Write-Host "`nCoverage generated!"
Write-Host "LCOV file: test_results/coverage/coverage.info"
Write-Host "HTML report: test_results/coverage/html/index.html"
