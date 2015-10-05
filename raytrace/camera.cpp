#include "camera.h"

Camera::~Camera() {}

ParametrizedLine<float, 3> Camera::generateRay(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = this->_mEye;
    vec3 direction = point - origin;
    direction.normalize();
    return ray3(origin, direction);
}

ParametrizedLine<float, 3> Camera::rayToCamera(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = point;
    vec3 direction = origin - this->_mEye;
    direction.normalize();
    return ray3(origin, direction);
}
