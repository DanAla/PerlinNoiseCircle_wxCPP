#include "PerlinNoise.h"
#include <cmath>
#include <random>
#include <algorithm>

const PerlinNoise::Vec2 PerlinNoise::gradients2D[8] = {
    Vec2(1, 0), Vec2(-1, 0), Vec2(0, 1), Vec2(0, -1),
    Vec2(0.707107, 0.707107), Vec2(-0.707107, 0.707107),
    Vec2(0.707107, -0.707107), Vec2(-0.707107, -0.707107)
};

PerlinNoise::PerlinNoise(unsigned int seed) {
    // Initialize permutation table with values 0-255
    for (int i = 0; i < GRAD_TABLE_SIZE; i++) {
        permutation[i] = i;
    }
    
    // Shuffle the permutation table
    std::mt19937 rng(seed);
    std::shuffle(permutation, permutation + GRAD_TABLE_SIZE, rng);
    
    // Duplicate the permutation table for easier indexing
    for (int i = 0; i < GRAD_TABLE_SIZE; i++) {
        permutation[GRAD_TABLE_SIZE + i] = permutation[i];
    }
}

double PerlinNoise::noise2D(double x, double y, int octaves, double persistence) const {
    double value = 0.0;
    double amplitude = 1.0;
    double frequency = 1.0;
    double maxValue = 0.0;
    
    for (int i = 0; i < octaves; i++) {
        // Calculate noise for this octave
        double xi = x * frequency;
        double yi = y * frequency;
        
        // Get integer and fractional parts
        int X = static_cast<int>(std::floor(xi)) & GRAD_TABLE_MASK;
        int Y = static_cast<int>(std::floor(yi)) & GRAD_TABLE_MASK;
        
        double xf = xi - std::floor(xi);
        double yf = yi - std::floor(yi);
        
        // Fade curves
        double u = fade(xf);
        double v = fade(yf);
        
        // Hash coordinates of 4 square corners
        int aa = permutation[permutation[X] + Y];
        int ab = permutation[permutation[X] + Y + 1];
        int ba = permutation[permutation[X + 1] + Y];
        int bb = permutation[permutation[X + 1] + Y + 1];
        
        // Calculate gradients
        double x1 = lerp(grad2D(aa, xf, yf), grad2D(ba, xf - 1, yf), u);
        double x2 = lerp(grad2D(ab, xf, yf - 1), grad2D(bb, xf - 1, yf - 1), u);
        
        double octaveValue = lerp(x1, x2, v);
        value += octaveValue * amplitude;
        
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }
    
    // Normalize to [0, 1]
    return (value / maxValue + 1.0) / 2.0;
}

double PerlinNoise::fade(double t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double a, double b, double t) const {
    return a + t * (b - a);
}

double PerlinNoise::grad2D(int hash, double x, double y) const {
    const Vec2& grad = gradients2D[hash & 7];
    return grad.x * x + grad.y * y;
}
