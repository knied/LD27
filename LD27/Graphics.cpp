//
//  Graphics.cpp
//  LD27
//
//  Created by Kristof Niederholtmeyer on 17.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Graphics.h"

////////////////////////////////////////////////////////////////////////////////

Color alpha_blend(const Color& dest, const Color& src) {
    float a = (float)src.a / 255.0f;
    int r = dest.r * (1.0f - a) + src.r * a;
    int g = dest.g * (1.0f - a) + src.g * a;
    int b = dest.b * (1.0f - a) + src.b * a;
    if (r > 255) {
        r = 255;
    }
    if (g > 255) {
        g = 255;
    }
    if (b > 255) {
        b = 255;
    }
    return Color(r,g,b);
}

////////////////////////////////////////////////////////////////////////////////