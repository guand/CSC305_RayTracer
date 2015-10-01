#include "camera.h"

Camera::Camera()
{

}

ParametrizedLine<float, 3> Camera::generateRay(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = _mEye;
    vec3 direction = _mEye - point;
    direction.normalize();
    return ray3(origin, direction);
}

ParametrizedLine<float, 3> Camera::rayToCamera(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = point;
    vec3 direction = _mEye - point;
    direction.normalize();
    return ray3(origin, _mEye);
}
