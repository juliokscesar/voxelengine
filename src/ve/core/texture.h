#pragma once

#include "base.h"

struct Texture2DProperties {
    uint32_t width;
    uint32_t height;
    
    GLenum storeFmt;
    GLenum pixelFmt;
};

class Texture2D {
public:
    Texture2D();
};
