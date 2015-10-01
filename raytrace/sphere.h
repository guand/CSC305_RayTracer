#ifndef SPHERE_H
#define SPHERE_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>
#include <math.h>

using namespace Eigen;

class Sphere
{
public:
    typedef cv::Vec3b Colour;

    Sphere();

    Sphere(vec3 const& c, float r, Colour colour) :
        _mCentre(c),
        _mNormal(c),
        _mRadius(r),
        _mColour(colour)
    { }

    Colour getColour() { return this->_mColour; }

    float intersectRay(ParametrizedLine<float, 3> const& ray);
    vec3 getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt);
    ParametrizedLine<float, 3> getNormal(vec3 sphereHitPt);

    float getKd() { return _kd; }
    float getKa() { return _ka; }
    float getKs() { return _ks; }
    float getN() { return _n; }

private:
    float _mRadius;
    vec3 _mCentre, _mNormal;
    float _kd = 1;
    float _ka = 0;
    float _ks = 0.7;
    float _pr = 1;
    float _n = 5;
    float _IOR = 1;
    Colour _mColour;
};

#endif // SPHERE_H
