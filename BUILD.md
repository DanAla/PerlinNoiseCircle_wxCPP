# Build Instructions

## Prerequisites

1. **MinGW-w64 GCC 15.1.0** installed at: `C:\mingw64-standalone\mingw64`
2. **wxWidgets 3.3.1** built with the same compiler at: `C:\wxWidgets-3.3.1`
3. **CMake 3.16+**

## Building

```bash
# Build the project
build.bat

# Clean and build
build.bat clean

# Build and run
build.bat start

# Run existing build
build.bat run

# Kill running app
build.bat kill

# Show help
build.bat help
```

## Output

- Executable: `build_mingw\PerlinNoiseCircle_wxCPP.exe`
- Required DLLs are automatically copied to the build directory
- Default parameters loaded from `default_params.json`

## Build System

- **CMake** with MinGW Makefiles generator
- **Static linking** of wxWidgets libraries
- **Windows manifest** embedded for modern Windows compatibility
- **C++17** standard required
