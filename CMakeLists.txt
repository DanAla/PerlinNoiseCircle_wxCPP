cmake_minimum_required(VERSION 3.16)
project(PerlinNoiseCircle_wxCPP VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets - point to our specific build
set(wxWidgets_ROOT_DIR "C:/wxWidgets-3.3.1")
set(wxWidgets_LIB_DIR "C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib")
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Manually set wxWidgets libraries and includes for MinGW
set(wxWidgets_LIBRARIES 
    "${wxWidgets_LIB_DIR}/libwxmsw33u_core.a"
    "${wxWidgets_LIB_DIR}/libwxbase33u.a"
    "${wxWidgets_LIB_DIR}/libwxpng.a"
    "${wxWidgets_LIB_DIR}/libwxzlib.a"
    "${wxWidgets_LIB_DIR}/libwxregexu.a"
    "${wxWidgets_LIB_DIR}/libwxexpat.a"
    winmm comctl32 uuid oleacc uxtheme rpcrt4 shlwapi version wsock32
    gdi32 gdiplus msimg32
)
set(wxWidgets_INCLUDE_DIRS 
    "C:/wxWidgets-3.3.1/include"
    "C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib/mswu"
)
set(wxWidgets_DEFINITIONS -D__WXMSW__)

include_directories(${wxWidgets_INCLUDE_DIRS})
add_definitions(${wxWidgets_DEFINITIONS})

# Source files
set(SOURCES
    src/main.cpp
    src/MainFrame.cpp
    src/Canvas.cpp
    src/ParamPanel.cpp
    src/PerlinNoise.cpp
)

set(HEADERS
    include/MainFrame.h
    include/Canvas.h
    include/ParamPanel.h
    include/PerlinNoise.h
    include/Constants.h
)

# Windows resource file for manifest and version info
if(WIN32)
    set(RESOURCE_FILES src/app.rc)
endif()

# Include directories
include_directories(include)

# Add external directory for single-header libraries
if(EXISTS "${CMAKE_SOURCE_DIR}/external/json.hpp")
    include_directories(external)
    add_definitions(-DUSE_SINGLE_HEADER_JSON)
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

# Copy default files to build directory
if(EXISTS "${CMAKE_SOURCE_DIR}/default_params.json")
    configure_file(default_params.json ${CMAKE_CURRENT_BINARY_DIR}/default_params.json COPYONLY)
endif()

# Windows specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # MinGW specific: ensure proper linking
    if(MINGW)
        target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)
    endif()
endif()
