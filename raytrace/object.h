#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

using namespace Eigen;

class Object
{
public:
    virtual ~Object(){}
    virtual bool intersectRay(ParametrizedLine<float, 3> const &ray) = 0;

    float getKd() { return _kd; }
    float getKa() { return _ka; }
    float getKs() { return _ks; }
    float getN() { return _n; }

protected:
    float _kd = 0.5;
    float _ka = 1;
    float _ks = 0.7;
    float _pr = 1;
    float _n = 5;
    float _IOR = 1;
};

#endif // OBJECT_H
