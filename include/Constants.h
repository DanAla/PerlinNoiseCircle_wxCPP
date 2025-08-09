#pragma once

#include <unordered_map>
#include <string>

// Canvas and UI constants
constexpr int CANVAS_SIZE = 2000;      // logical bitmap
constexpr int VIEWPORT_SIZE = 700;     // visible viewport

// Parameter structure
struct NoiseParams {
    double startRadius = 25.0;
    int maxCircles = 310;
    int resolution = 1;
    double dRadius = 0.5;
    double rdn = 10.0;
    double xOffset = 0.0;
    double yOffset = 0.0;
    int nSeed = 0;
    double penWidth = 0.25;
    double lineDistance = 1.0;
};

// Default parameter values
inline NoiseParams GetDefaultParams() {
    return NoiseParams{
        25.0,   // startRadius
        310,    // maxCircles
        1,      // resolution
        0.5,    // dRadius
        10.0,   // rdn
        0.0,    // xOffset
        0.0,    // yOffset
        0,      // nSeed
        0.25,   // penWidth
        1.0     // lineDistance
    };
}

// File paths
inline std::string GetConfigFile() {
    return "noise_circles_state.json";
}

inline std::string GetDefaultParamsFile() {
    return "default_params.json";
}
