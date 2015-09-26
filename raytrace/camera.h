#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include "icg_common.h"
#include <Eigen/Geometry>

using namespace Eigen;

class Camera
{
public:
    Camera();
    // we only need the position of the camera.
    Camera(vec3 const& eye) : _mEye(eye) { }

    // Given coordinates of a pixel in the view plane, we generate its primary ray
    ParametrizedLine<float, 3> generateRay(vec3 const& point){
        typedef ParametrizedLine<float, 3> ray3;

        vec3 origin = _mEye;
        vec3 direction = _mEye - point;
        direction.normalize();
        return ray3(origin, direction);
    }

private:
    vec3 _mEye; // The position of the camera.
};

#endif // CAMERA_H
