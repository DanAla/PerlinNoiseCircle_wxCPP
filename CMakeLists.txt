cmake_minimum_required(VERSION 3.16)
project(PerlinNoiseCircle_wxCPP VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Find nlohmann/json
find_package(nlohmann_json REQUIRED)

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

# Include directories
include_directories(include)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} nlohmann_json::nlohmann_json)

# Copy default files to build directory
configure_file(default_params.json ${CMAKE_BINARY_DIR}/default_params.json COPYONLY)

# Set output directory
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Windows specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
