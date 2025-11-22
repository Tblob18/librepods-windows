# LibrePods Windows - System Requirements Test Runner
# This script builds and runs the system requirements test suite

param(
    [string]$VcpkgPath = "",
    [string]$QtPath = "",
    [switch]$SkipBuild = $false,
    [switch]$Verbose = $false
)

Write-Host "=== LibrePods Windows - System Requirements Test ===" -ForegroundColor Cyan
Write-Host ""

# Determine if we're already in a build directory
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$testsDir = Join-Path $scriptDir "tests"
$buildDir = Join-Path $testsDir "build"

# Check if test directory exists
if (-not (Test-Path $testsDir)) {
    Write-Host "Error: Tests directory not found at $testsDir" -ForegroundColor Red
    Write-Host "Please run this script from the windows directory" -ForegroundColor Red
    exit 1
}

# Create build directory if it doesn't exist
if (-not (Test-Path $buildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Set-Location $buildDir

if (-not $SkipBuild) {
    Write-Host "Building tests..." -ForegroundColor Yellow
    Write-Host ""
    
    # Prepare CMake command
    $cmakeConfigArgs = @("..")
    
    if ($VcpkgPath -ne "") {
        $toolchainFile = Join-Path $VcpkgPath "scripts\buildsystems\vcpkg.cmake"
        if (Test-Path $toolchainFile) {
            Write-Host "Using vcpkg toolchain: $toolchainFile" -ForegroundColor Green
            $cmakeConfigArgs += "-DCMAKE_TOOLCHAIN_FILE=$toolchainFile"
        } else {
            Write-Host "Warning: vcpkg toolchain file not found at $toolchainFile" -ForegroundColor Yellow
        }
    } elseif ($QtPath -ne "") {
        if (Test-Path $QtPath) {
            Write-Host "Using Qt path: $QtPath" -ForegroundColor Green
            $cmakeConfigArgs += "-DCMAKE_PREFIX_PATH=$QtPath"
        } else {
            Write-Host "Warning: Qt path not found at $QtPath" -ForegroundColor Yellow
        }
    } else {
        Write-Host "No vcpkg or Qt path specified, using system defaults" -ForegroundColor Yellow
        Write-Host "If configuration fails, provide -VcpkgPath or -QtPath" -ForegroundColor Yellow
    }
    
    # Configure
    Write-Host ""
    Write-Host "Configuring CMake..." -ForegroundColor Yellow
    & cmake @cmakeConfigArgs
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "Error: CMake configuration failed!" -ForegroundColor Red
        Write-Host ""
        Write-Host "Try running with vcpkg:" -ForegroundColor Yellow
        Write-Host "  .\run_tests.ps1 -VcpkgPath 'C:\path\to\vcpkg'" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "Or with Qt:" -ForegroundColor Yellow
        Write-Host "  .\run_tests.ps1 -QtPath 'C:\Qt\6.x.x\msvc2019_64'" -ForegroundColor Cyan
        exit 1
    }
    
    # Build
    Write-Host ""
    Write-Host "Building..." -ForegroundColor Yellow
    & cmake --build . --config Release
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host ""
        Write-Host "Error: Build failed!" -ForegroundColor Red
        exit 1
    }
    
    Write-Host ""
    Write-Host "Build completed successfully!" -ForegroundColor Green
}

# Run tests
Write-Host ""
Write-Host "=== Running System Requirements Tests ===" -ForegroundColor Cyan
Write-Host ""

$testExe = Join-Path $buildDir "Release\test_system_requirements.exe"

if (-not (Test-Path $testExe)) {
    # Try Debug build
    $testExe = Join-Path $buildDir "Debug\test_system_requirements.exe"
    if (-not (Test-Path $testExe)) {
        Write-Host "Error: Test executable not found!" -ForegroundColor Red
        Write-Host "Expected location: $testExe" -ForegroundColor Red
        exit 1
    }
}

Write-Host "Running: $testExe" -ForegroundColor Yellow
Write-Host ""

# Run the test executable
& $testExe

$testResult = $LASTEXITCODE

Write-Host ""
Write-Host "================================================" -ForegroundColor Cyan

if ($testResult -eq 0) {
    Write-Host "✓ All tests passed!" -ForegroundColor Green
    Write-Host "Your system meets all requirements for LibrePods Windows" -ForegroundColor Green
} else {
    Write-Host "✗ Some tests failed" -ForegroundColor Red
    Write-Host "Please review the output above to see which requirements are not met" -ForegroundColor Yellow
}

Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

exit $testResult
