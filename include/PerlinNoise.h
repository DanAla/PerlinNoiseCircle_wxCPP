#pragma once

class PerlinNoise {
public:
    PerlinNoise(unsigned int seed = 1);
    
    // 2D Perlin noise with multiple octaves
    double noise2D(double x, double y, int octaves = 4, double persistence = 0.35) const;
    
private:
    static constexpr int GRAD_TABLE_SIZE = 256;
    static constexpr int GRAD_TABLE_MASK = 255;
    
    // Permutation table
    int permutation[512];
    
    // Gradient vectors for 2D noise
    struct Vec2 {
        double x, y;
        Vec2(double x, double y) : x(x), y(y) {}
    };
    
    static const Vec2 gradients2D[8];
    
    // Helper functions
    double fade(double t) const;
    double lerp(double a, double b, double t) const;
    double grad2D(int hash, double x, double y) const;
};
