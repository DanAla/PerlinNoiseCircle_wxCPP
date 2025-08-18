cmake_minimum_required(VERSION 3.16)
project(PerlinNoiseCircle_wxCPP VERSION 1.0.0)

# Enable resource compilation for Windows
if(WIN32)
    enable_language(RC)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Manual wxWidgets configuration for MinGW
set(wxWidgets_ROOT_DIR "C:/wxWidgets-3.3.1")
set(wxWidgets_LIB_DIR "C:/wxWidgets-3.3.1/build-mingw/lib/gcc_x64_lib")
set(wxWidgets_INCLUDE_DIR "${wxWidgets_ROOT_DIR}/include" "${wxWidgets_LIB_DIR}/mswu")

# wxWidgets libraries
set(wxWidgets_LIBRARIES
    ${wxWidgets_LIB_DIR}/libwxmsw33u_core.a
    ${wxWidgets_LIB_DIR}/libwxbase33u.a
    ${wxWidgets_LIB_DIR}/libwxpng.a
    ${wxWidgets_LIB_DIR}/libwxtiff.a
    ${wxWidgets_LIB_DIR}/libwxjpeg.a
    ${wxWidgets_LIB_DIR}/libwxzlib.a
    ${wxWidgets_LIB_DIR}/libwxregexu.a
    ${wxWidgets_LIB_DIR}/libwxexpat.a
)

# Add wxWidgets include directories
include_directories(${wxWidgets_INCLUDE_DIR})

# Add wxWidgets definitions
add_definitions(-D__WXMSW__ -DwxUSE_UNICODE=1)

# Source files
set(SOURCES
    src/main.cpp
    src/MainFrame.cpp
    src/Canvas.cpp
    src/ParamPanel.cpp
    src/PerlinNoise.cpp
)

# Windows resource file
if(WIN32)
    set(SOURCES ${SOURCES} app.rc)
endif()

set(HEADERS
    include/MainFrame.h
    include/Canvas.h
    include/ParamPanel.h
    include/PerlinNoise.h
    include/Constants.h
)

# Include directories
include_directories(include)

# Add external directory for single-header libraries
if(EXISTS "${CMAKE_SOURCE_DIR}/external/json.hpp")
    include_directories(external)
    add_definitions(-DUSE_SINGLE_HEADER_JSON)
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

# Link Windows system libraries required by wxWidgets
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        kernel32 user32 gdi32 comdlg32 winspool winmm shell32 comctl32 ole32
        oleaut32 uuid rpcrt4 advapi32 wsock32 wininet gdiplus msimg32
        uxtheme dwmapi shlwapi version oleacc
    )
endif()

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
