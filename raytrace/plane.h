#ifndef PLANE_H
#define PLANE_H

#pragma once
#include "icg_common.h"
#include "object.h"
#include <Eigen/Geometry>
#include <math.h>

using namespace Eigen;

class Plane: public Object
{
public:
    typedef cv::Vec3b Colour;

    Plane(vec3 const& position, vec3 const& normal, Colour const& colour) :
        _mPosition(position),
        _mNormal(normal),
        Object(colour)
    { }

    ~Plane();

    bool intersectRay(ParametrizedLine<float, 3> const &ray);

    // getter
    vec3 getPosition() { return _mPosition; }


private:
    vec3 _mPosition;
    vec3 _mNormal;
};

#endif // PLANE_H
