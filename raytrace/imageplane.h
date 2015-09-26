#ifndef IMAGEPLANE_H
#define IMAGEPLANE_H

#pragma once
#include "icg_common.h"

class ImagePlane
{
public:
    ImagePlane();

    ImagePlane(vec3 const& llCorner, // The lower-left corner (l, b, 0)
               vec3 const& urCorner, // The upper-right corner (r, t, 0)
               int  const& xRes, int const& yRes) : // The dimensions of the image
        _l(llCorner.x()),
        _b(llCorner.y()),
        _r(urCorner.x()),
        _t(urCorner.y()),
        _mRows(xRes),
        _mCols(yRes)
    { }

    inline vec3 generatePixelPos(int i, int j)
    {
        float u = _l + (_r - _l) * (i + 0.5f) / _mRows;
        float v = _b + (_t - _b) * (j + 0.5f) / _mCols;
        return vec3(u, v, 0);
    }

private:
    int _mRows, _mCols;
    float _l, _r, _b, _t;
};

#endif // IMAGEPLANE_H
