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

    inline bool intersectRay(ParametrizedLine<float, 3> const& ray)
    {
        // Intersect against the plane first to compute the t value
//        float d = (-_mCentre).dot(_mNormal);
//        float t = -(ray.origin().dot(_mNormal) + d) / (ray.direction().dot(_mNormal));


        float a = ray.direction().dot(ray.direction());
        float b = ray.direction().dot(ray.origin() - _mCentre);
        float c = (ray.origin() - _mCentre).dot(ray.origin() - _mCentre) - (_mRadius * _mRadius);

        float discriminant = (b * b) - (a * c);

        if(discriminant < 0)
        {
            return false;
        }
        // check this code later
        float t = (-b - sqrt(discriminant)) / 2;

        vec3 hitPoint = ray.pointAt(t);
        vec3 dist = hitPoint - _mCentre;


        return (dist.norm() < _mRadius) ? true : false;
    }

private:
    float _mRadius;
    vec3 _mCentre, _mNormal;
};

#endif // SPHERE_H
