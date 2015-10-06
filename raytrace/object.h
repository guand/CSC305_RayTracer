#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>
#include <math.h>

using namespace Eigen;
typedef cv::Vec3f Coefficient;
typedef cv::Vec3b Colour;

class Object
{
public:
    virtual ~Object() {}
    virtual bool intersectRay(ParametrizedLine<float, 3> const &ray) = 0;
    virtual float intersectRayValue(ParametrizedLine<float, 3> const &ray) = 0;
    Object(Coefficient coefficient) :
        _colour(coefficient)
    {}

    /// Phong Lighting model
    Colour ambient(Colour lightColour);
    float diffuseDot(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay);
    Colour diffuse(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, Colour lightColour);
    Colour specular(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, ParametrizedLine<float, 3> const &rayToCamera, Colour lightColour);

    /// getters
    Coefficient getKd() { return _kd; }
    Coefficient getKs() { return _ks; }
    float getN() { return _n; }
    Coefficient getCoefficient() { return _colour; }

    /// setters
    void setKd(Coefficient newKd) { _kd = newKd; }
    void setKs(Coefficient newKs) { _ks = newKs; }


protected:
    // Create variables for Parent Class
    Coefficient _colour;
    Coefficient _kd = Coefficient(0.5, 0.5, 0.5);
    Coefficient _ks = Coefficient(0.4, 0.4, 0.4);
    float _pr = 1;
    float _n = 12;
    float _IOR = 1;
};

#endif // OBJECT_H
