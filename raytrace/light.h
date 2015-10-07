#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

using namespace Eigen;
class Light
{
public:
    typedef cv::Vec3b Colour;

    Light(vec3 const& position, Colour const& colour) :
        _mPosition(position),
        _mColour(colour)
    { }

    ~Light();

    ParametrizedLine<float, 3> generateRay(vec3 const& point);

    /// getter
    Colour getColour() { return _mColour; }
    vec3 getPosition() { return _mPosition; }

private:
    Colour _mColour;
    vec3 _mPosition;
};

#endif // LIGHT_H
