#pragma once

inline constexpr double initialHeight = 4.0;

struct Properties
{
    int maxIter = 50;
    double centerReal = -0.75;
    double centerImag = 0.0;
    double height = initialHeight;

    int rmin = 100;
    int rmax = 255;
    int gmin = 0;
    int gmax = 255;
    int bmin = 0;
    int bmax = 0;
};

extern Properties g_props;
