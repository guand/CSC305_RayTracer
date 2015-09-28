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
    Sphere();

    Sphere(vec3 const& c, float r) :
        _mCentre(c),
        _mNormal(c),
        _mRadius(r)
    { }

    inline double intersectRay(ParametrizedLine<float, 3> const& ray)
    {
        // Intersect against the plane first to compute the t value
//        float d = (-_mCentre).dot(_mNormal);
//        float t = -(ray.origin().dot(_mNormal) + d) / (ray.direction().dot(_mNormal));


        double a = ray.direction().dot(ray.direction());
        double b = ray.direction().dot(ray.origin() - _mCentre);
        double c = (ray.origin() - _mCentre).dot(ray.origin() - _mCentre) - (_mRadius * _mRadius);

        double discriminant = (b * b) - (a * c);

        if(discriminant < 0)
        {
            return -1;
        }
        // check this code later, why the divisor is 2 and doesn't need to be 2*a or why it doesn't make a difference
        double t0 = (-b - sqrt(discriminant)) / a;
        double t1 = (-b + sqrt(discriminant)) / a;

        return t0;
    }

private:
    float _mRadius;
    vec3 _mCentre, _mNormal;
};

#endif // SPHERE_H
