#include "camera.h"

Camera::~Camera() {}

/**
 * @brief Camera::generateRay
 * @param point
 * @return ParametrizedLine<float, 3>
 * Generates a ray from the camera to the scene where the objects are
 */
ParametrizedLine<float, 3> Camera::generateRay(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = this->_mEye;
    vec3 direction = point - origin;
    direction.normalize();
    return ray3(origin, direction);
}

/**
 * @brief Camera::rayToCamera
 * @param point
 * @return ParametrizedLine<float, 3>
 * Generates a ray from the scene to the camera
 */
ParametrizedLine<float, 3> Camera::rayToCamera(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = point;
    vec3 direction = origin - this->_mEye;
    direction.normalize();
    return ray3(origin, direction);
}
