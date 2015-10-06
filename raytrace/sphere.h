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
    typedef cv::Vec3f Coefficient;
    typedef cv::Mat Image;

    Sphere(vec3 const& c, float const& r, Coefficient const& coefficient, int special) :
        _mCentre(c),
        _mNormal(c),
        _mRadius(r),
        _mSpecial(special),
        Object(coefficient)
    { }

    ~Sphere();

    bool intersectRay(ParametrizedLine<float, 3> const& ray);
    bool intersectRayForShadow(ParametrizedLine<float, 3> const &ray);
    float intersectRayValue(ParametrizedLine<float, 3> const& ray);
    vec3 getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt);
    ParametrizedLine<float, 3> getNormal(vec3 sphereHitPt);
    Colour textureValue(vec3 pt);

    // getters
    vec3 getCentre() { return _mCentre; }
    float getRadius() { return _mRadius; }
    int getSpecial() { return _mSpecial; }

private:
    float _mRadius;
    vec3 _mCentre, _mNormal;
    int _mSpecial;
    Image image = cv::imread("/Users/danny/Development/CSC 305/icg/raytrace/earthMap.jpg", 1);
};

#endif // SPHERE_H
