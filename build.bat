@echo off
REM PerlinNoiseCircle_wxCPP Build Script

set MINGW_ROOT=C:\mingw64-standalone\mingw64
set WXWIDGETS_ROOT=C:\wxWidgets-3.3.1
set WXWIDGETS_BUILD=%WXWIDGETS_ROOT%\build-mingw
set APP_NAME=PerlinNoiseCircle_wxCPP

if "%1"=="help" goto :help
if "%1"=="kill" goto :kill
if "%1"=="start" goto :start
if "%1"=="run" goto :run
goto :build

:help
echo Usage: build.bat [command]
echo Commands:
echo   (none)  - Build the project
echo   clean   - Clean and build
echo   kill    - Kill running app
echo   start   - Build and run app
echo   run     - Run existing app
echo   help    - Show this help
exit /b 0

:kill
echo Killing %APP_NAME%...
taskkill /f /im %APP_NAME%.exe >nul 2>&1
if "%2"=="" exit /b 0
shift
goto :%2

:start
call :kill silent
call :build %2
if %ERRORLEVEL% equ 0 call :run
exit /b %ERRORLEVEL%

:run
if not exist "build_mingw\%APP_NAME%.exe" (
    echo [ERROR] Executable not found. Build first.
    exit /b 1
)
echo Starting %APP_NAME%...
start "" "build_mingw\%APP_NAME%.exe"
exit /b 0

:build
if not exist "%MINGW_ROOT%\bin\gcc.exe" (
    echo [ERROR] MinGW not found at %MINGW_ROOT%
    exit /b 1
)

if not exist "%WXWIDGETS_BUILD%\lib\gcc_x64_lib" (
    echo [ERROR] wxWidgets not found at %WXWIDGETS_BUILD%
    exit /b 1
)

set PATH=%MINGW_ROOT%\bin;%PATH%

if "%1"=="clean" (
    echo Cleaning...
    if exist build_mingw rmdir /s /q build_mingw
)

if not exist build_mingw mkdir build_mingw
cd build_mingw

echo Configuring...
cmake -G "MinGW Makefiles" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER="%MINGW_ROOT%\bin\gcc.exe" ^
    -DCMAKE_CXX_COMPILER="%MINGW_ROOT%\bin\g++.exe" ^
    -DCMAKE_MAKE_PROGRAM="%MINGW_ROOT%\bin\mingw32-make.exe" ^
    -S .. -B . >nul

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Configuration failed
    cd ..
    exit /b 1
)

echo Building...
%MINGW_ROOT%\bin\mingw32-make.exe -j4 >nul

if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed
    cd ..
    exit /b 1
)

REM Copy runtime DLLs
copy "%MINGW_ROOT%\bin\libgcc_s_seh-1.dll" . >nul 2>&1
copy "%MINGW_ROOT%\bin\libstdc++-6.dll" . >nul 2>&1
copy "%MINGW_ROOT%\bin\libwinpthread-1.dll" . >nul 2>&1
if exist "..\default_params.json" copy "..\default_params.json" . >nul

cd ..
echo Build complete: build_mingw\%APP_NAME%.exe
exit /b 0
