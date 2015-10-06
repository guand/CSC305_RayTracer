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
    typedef cv::Vec3f Coefficient;

    Plane(vec3 const& position, vec3 const& normal, Coefficient const& coefficient, int special) :
        _mPosition(position),
        _mNormal(normal),
        _mSpecial(special),
        Object(coefficient)
    { }

    ~Plane();

    /// ray intersects
    bool intersectRay(ParametrizedLine<float, 3> const &ray);
    float intersectRayValue(ParametrizedLine<float, 3> const &ray);
    ParametrizedLine<float, 3> getNormal(vec3 pt);
    vec3 getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt);
    Coefficient checkerBoard(vec3 pt);
    /// getter
    vec3 getPosition() { return _mPosition; }
    int getSpecial() { return _mSpecial; }


private:
    vec3 _mPosition;
    vec3 _mNormal;
    int _mSpecial;
};

#endif // PLANE_H
