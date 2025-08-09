# wxWidgets C++ Build Toolchain Setup Guide

## Overview
This document explains the complete setup for building wxWidgets C++ applications on Windows using MinGW, specifically for the user's development environment. This setup was successfully tested with the PerlinNoiseCircle_wxCPP project.

## Environment Details
- **Operating System**: Windows
- **Compiler**: MinGW-w64 13.1.0 (from Qt Tools)
- **Build System**: CMake 3.x
- **wxWidgets Version**: 3.3.1
- **Shell**: PowerShell 7.5.2

## Tool Locations
```
MinGW: C:/Qt/Tools/mingw1310_64/
CMake: C:/Qt/Tools/CMake_64/
wxWidgets Source: C:/wxWidgets-3.3.1/
wxWidgets Build: C:/wxWidgets-3.3.1/build-mingw/
```

## Step-by-Step Setup

### 1. wxWidgets Installation & Build

#### Download and Extract wxWidgets
```bash
# Download wxWidgets 3.3.1 source
# Extract to: C:/wxWidgets-3.3.1/
```

#### Build wxWidgets with MinGW
```bash
cd C:/wxWidgets-3.3.1
mkdir build-mingw
cd build-mingw

# Configure wxWidgets for MinGW static build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DwxBUILD_SHARED=OFF

# Build (this takes 15-30 minutes)
mingw32-make -j4
```

#### Verify wxWidgets Build
The successful build should create:
- `C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib/` (library directory)
- Static libraries like:
  - `libwxmsw33u_core.a`
  - `libwxbase33u.a` 
  - `libwxpng.a`, `libwxzlib.a`, `libwxregexu.a`, `libwxexpat.a`
- Include directory: `C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib/mswu/`

### 2. CMakeLists.txt Configuration

The key to successful linking is **manually specifying** wxWidgets libraries rather than relying on `include(${wxWidgets_USE_FILE})`.

#### Critical CMakeLists.txt Template
```cmake
cmake_minimum_required(VERSION 3.16)
project(YourProjectName VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets - point to our specific build
set(wxWidgets_ROOT_DIR "C:/wxWidgets-3.3.1")
set(wxWidgets_LIB_DIR "C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib")
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED COMPONENTS core base)

# CRITICAL: Manually set wxWidgets libraries and includes for MinGW
set(wxWidgets_LIBRARIES 
    "${wxWidgets_LIB_DIR}/libwxmsw33u_core.a"
    "${wxWidgets_LIB_DIR}/libwxbase33u.a"
    "${wxWidgets_LIB_DIR}/libwxpng.a"
    "${wxWidgets_LIB_DIR}/libwxzlib.a"
    "${wxWidgets_LIB_DIR}/libwxregexu.a"
    "${wxWidgets_LIB_DIR}/libwxexpat.a"
    winmm comctl32 uuid oleacc uxtheme rpcrt4 shlwapi version wsock32
    gdi32 gdiplus msimg32  # Essential for graphics/GDI+ support
)
set(wxWidgets_INCLUDE_DIRS 
    "C:/wxWidgets-3.3.1/include"
    "C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib/mswu"
)
set(wxWidgets_DEFINITIONS -D__WXMSW__)

include_directories(${wxWidgets_INCLUDE_DIRS})
add_definitions(${wxWidgets_DEFINITIONS})

# Your source files
set(SOURCES
    src/main.cpp
    # ... other source files
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

# Windows specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    if(MINGW)
        target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
    endif()
endif()
```

### 3. Build Process

#### Standard Build Commands
```bash
# From project root
mkdir build-mingw
cd build-mingw
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4
```

#### Automated Build Script (build.bat)
```batch
@echo off
setlocal EnableDelayedExpansion

# Tool checks, directory creation, build, and optional run
# (See build.bat in project for full implementation)
```

### 4. Common Issues & Solutions

#### Issue: Undefined reference to wxWidgets symbols
**Cause**: CMake's automatic wxWidgets detection doesn't work properly with MinGW
**Solution**: Manually specify libraries in CMakeLists.txt as shown above

#### Issue: GDI+/Graphics related undefined references  
**Cause**: Missing Windows graphics libraries
**Solution**: Add `gdi32 gdiplus msimg32` to library list

#### Issue: Static linking problems
**Cause**: Wrong wxWidgets configuration or missing static libraries
**Solution**: Ensure wxWidgets built with `-DwxBUILD_SHARED=OFF`

#### Issue: Wrong include paths
**Cause**: CMake not finding correct setup.h
**Solution**: Explicitly set include paths to both:
- `C:/wxWidgets-3.3.1/include`  
- `C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib/mswu`

### 5. Project Structure Template

```
YourProject/
├── src/
│   ├── main.cpp
│   └── ... (other .cpp files)
├── include/
│   └── ... (header files)
├── external/          # Optional: single-header libraries
│   └── json.hpp
├── CMakeLists.txt     # Using template above
├── build.bat          # Build automation script
├── default_params.json # Optional: config files
└── build-mingw/       # Build directory (created by build script)
    └── YourProject.exe
```

### 6. Verification Steps

After setup, verify with:

1. **Tools available**:
   ```bash
   where mingw32-make  # Should find Qt Tools MinGW
   where cmake         # Should find CMake
   ```

2. **wxWidgets libraries exist**:
   ```bash
   ls C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib/libwxmsw33u_core.a
   ```

3. **Test build**:
   ```bash
   cd your-project
   build.bat  # Should complete without errors
   ```

4. **Executable runs**:
   - Should launch GUI application
   - No console errors
   - Reasonable file size (~14MB for static linking)

### 7. Key Success Indicators

- ✅ Build completes in under 2 minutes (after wxWidgets is built)
- ✅ Executable size ~10-20MB (indicates proper static linking)
- ✅ No DLL dependencies (self-contained executable)
- ✅ Application launches immediately without missing DLL errors
- ✅ All GUI controls work properly

### 8. Performance Notes

- **First wxWidgets build**: 15-30 minutes (one-time)
- **Subsequent project builds**: 30 seconds to 2 minutes
- **Final executable**: Self-contained, no external dependencies
- **Parallel builds**: Use `-j4` or `-j8` based on CPU cores

## Summary

The critical insight is that **manual library specification** in CMakeLists.txt is required for MinGW + wxWidgets on Windows. The automatic CMake wxWidgets detection doesn't work reliably, so explicit paths and library lists are essential for successful builds.

This setup produces fully static, portable executables that run without requiring wxWidgets or MinGW to be installed on target machines.
