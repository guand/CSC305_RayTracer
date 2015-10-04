#ifndef SPHERE_H
#define SPHERE_H

#pragma once
#include "icg_common.h"
#include "object.h"
#include <Eigen/Geometry>
#include <math.h>

using namespace Eigen;

class Sphere: public Object
{
public:
    typedef cv::Vec3b Colour;

    Sphere(vec3 const& c, float const& r, Colour const& colour) :
        _mCentre(c),
        _mNormal(c),
        _mRadius(r),
        Object(colour)
    { }

    ~Sphere();

    bool intersectRay(ParametrizedLine<float, 3> const& ray);
    float intersectRayValue(ParametrizedLine<float, 3> const& ray);
    vec3 getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt);
    ParametrizedLine<float, 3> getNormal(vec3 sphereHitPt);

    // getters
    vec3 getCentre() { return _mCentre; }
    float getRadius() { return _mRadius; }

private:
    float _mRadius;
    vec3 _mCentre, _mNormal;
};

#endif // SPHERE_H
