@echo off
setlocal EnableDelayedExpansion

echo ==================================================
echo   PerlinNoiseCircle wxWidgets C++ Build Script
echo ==================================================
echo.

:: Check if MinGW is available
where mingw32-make >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: mingw32-make not found in PATH
    echo Please ensure MinGW is installed and in your PATH
    pause
    exit /b 1
)

:: Check if CMake is available  
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: cmake not found in PATH
    echo Please ensure CMake is installed and in your PATH
    pause
    exit /b 1
)

echo Tools check: OK
echo.

:: Create build directory if it doesn't exist
if not exist build-mingw (
    echo Creating build directory...
    mkdir build-mingw
)

:: Enter build directory
cd build-mingw

echo Configuring project with CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed
    cd ..
    pause
    exit /b 1
)

echo.
echo Building project...
mingw32-make -j4
if %errorlevel% neq 0 (
    echo ERROR: Build failed
    cd ..
    pause
    exit /b 1
)

echo.
echo ==================================================
echo   Build completed successfully!
echo ==================================================
echo.
echo Executable: build-mingw\PerlinNoiseCircle_wxCPP.exe

:: Check if executable exists
if exist PerlinNoiseCircle_wxCPP.exe (
    echo Size: 
    for %%f in (PerlinNoiseCircle_wxCPP.exe) do echo   %%~zf bytes
    echo.
    
    :: Automatically run the program after successful build
    echo Starting PerlinNoiseCircle...
    start PerlinNoiseCircle_wxCPP.exe
    echo Program launched!
) else (
    echo ERROR: Executable not found after build
    cd ..
    pause
    exit /b 1
)

cd ..
echo.
echo Build script completed.
