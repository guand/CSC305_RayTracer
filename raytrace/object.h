#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>
#include <math.h>

using namespace Eigen;
typedef cv::Vec3b Colour;

class Object
{
public:
    virtual ~Object() {}
    virtual bool intersectRay(ParametrizedLine<float, 3> const &ray) = 0;
    Object(Colour colour) :
        _colour(colour)
    {}

    /// Phong Lighting model
    Colour ambient();
    Colour diffuse(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, Colour lightColour);
    Colour specular(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, ParametrizedLine<float, 3> const &rayToCamera, Colour lightColour);

    /// getters
    float getKd() { return _kd; }
    float getKa() { return _ka; }
    float getKs() { return _ks; }
    float getN() { return _n; }
    Colour getColour() { return _colour; }


protected:
    Colour _colour;
    float _kd = 0.5;
    float _ka = 1;
    float _ks = 0.7;
    float _pr = 1;
    float _n = 5;
    float _IOR = 1;
};

#endif // OBJECT_H
