#include "light.h"

Light::Light()
{

}

ParametrizedLine<float, 3> Light::generateRay(vec3 const& point)
{
    typedef ParametrizedLine<float, 3> ray3;

    vec3 origin = point;
    vec3 direction = _mPosition - point;
    direction.normalize();
    return ray3(origin, direction);
}

