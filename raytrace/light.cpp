#include "light.h"

Light::~Light() {}

/**
 * @brief Light::generateRay
 * @param point
 * @return ParametrizedLine<float, 3>
 * Generates a ray to the light
 */
ParametrizedLine<float, 3> Light::generateRay(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = point;
    vec3 direction = this->_mPosition - point;
    direction.normalize();
    return ray3(origin, direction);
}

