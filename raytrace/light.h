#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

class Light
{
public:
    typedef cv::Vec3b Colour;

    Light();

    Light(vec3 const& position, Colour const& colour) :
        _mPosition(position),
        _mColour(colour)
    { }

private:
    Colour _mColour;
    vec3 _mPosition;
};

#endif // LIGHT_H
