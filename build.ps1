#!/usr/bin/env pwsh

# PerlinNoiseCircle_wxCPP Build Script
# This script helps set up dependencies and build the project on Windows

param(
    [string]$Config = "Release",
    [switch]$Clean,
    [switch]$Help
)

if ($Help) {
    Write-Host "Usage: .\build.ps1 [OPTIONS]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Config <Release|Debug>  Build configuration (default: Release)"
    Write-Host "  -Clean                   Clean build directory before building"
    Write-Host "  -Help                    Show this help message"
    Write-Host ""
    Write-Host "Prerequisites:"
    Write-Host "  - Visual Studio 2019 or later with C++ support"
    Write-Host "  - CMake 3.16 or later"
    Write-Host "  - vcpkg (optional but recommended for dependencies)"
    Write-Host ""
    Write-Host "To install dependencies with vcpkg:"
    Write-Host "  vcpkg install wxwidgets nlohmann-json"
    Write-Host ""
    exit 0
}

$ErrorActionPreference = "Stop"

# Check if we're in the correct directory
if (!(Test-Path "CMakeLists.txt")) {
    Write-Error "CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
}

# Check for CMake
if (!(Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Error "CMake not found in PATH. Please install CMake and add it to your PATH."
    exit 1
}

Write-Host "🔧 Building PerlinNoiseCircle_wxCPP..." -ForegroundColor Green

# Clean if requested
if ($Clean -and (Test-Path "build-win")) {
    Write-Host "🧹 Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force "build-win"
}

# Create build directory
if (!(Test-Path "build-win")) {
    New-Item -ItemType Directory -Path "build-win" | Out-Null
}

# Check if vcpkg is available
$vcpkgToolchain = $null
if ($env:VCPKG_ROOT) {
    $vcpkgToolchain = "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
    if (Test-Path $vcpkgToolchain) {
        Write-Host "✅ Found vcpkg at: $env:VCPKG_ROOT" -ForegroundColor Green
    }
}

# Configure
Write-Host "⚙️ Configuring project..." -ForegroundColor Blue
$cmakeArgs = @("-S", ".", "-B", "build-win")

if ($vcpkgToolchain) {
    $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchain"
}

try {
    & cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }
} catch {
    Write-Host ""
    Write-Host "❌ Configuration failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "This is likely due to missing dependencies. Here are some solutions:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Option 1 - Install dependencies with vcpkg (recommended):" -ForegroundColor Cyan
    Write-Host "  1. Install vcpkg: https://vcpkg.io/en/getting-started.html"
    Write-Host "  2. Set VCPKG_ROOT environment variable"
    Write-Host "  3. Run: vcpkg install wxwidgets nlohmann-json"
    Write-Host "  4. Re-run this build script"
    Write-Host ""
    Write-Host "Option 2 - Manual installation:" -ForegroundColor Cyan
    Write-Host "  1. Download and build wxWidgets from https://www.wxwidgets.org/"
    Write-Host "  2. Download nlohmann/json from https://github.com/nlohmann/json"
    Write-Host "  3. Set wxWidgets_ROOT_DIR environment variable"
    Write-Host ""
    exit 1
}

# Build
Write-Host "🔨 Building project ($Config)..." -ForegroundColor Blue
try {
    cmake --build build-win --config $Config
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }
} catch {
    Write-Host "❌ Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "✅ Build completed successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "Executable location:" -ForegroundColor Cyan
if ($Config -eq "Debug") {
    Write-Host "  build-win\Debug\PerlinNoiseCircle_wxCPP.exe" -ForegroundColor White
} else {
    Write-Host "  build-win\Release\PerlinNoiseCircle_wxCPP.exe" -ForegroundColor White
}
Write-Host ""
Write-Host "To run the application:" -ForegroundColor Yellow
Write-Host "  cd build-win\$Config" -ForegroundColor White
Write-Host "  .\PerlinNoiseCircle_wxCPP.exe" -ForegroundColor White
