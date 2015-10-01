#include "imageplane.h"

ImagePlane::ImagePlane()
{

}

vec3 ImagePlane::generatePixelPos(int i, int j)
{
    float u = _l + (_r - _l) * (i + 0.5f) / _mRows;
    float v = _b + (_t - _b) * (j + 0.5f) / _mCols;
    return vec3(u, v, 0);
}


