#include "object.h"

Colour Object::ambient()
{
    Colour ambientComponent = this->_ka * this->_colour;
    return ambientComponent;
}

Colour Object::diffuse(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, Colour lightColour)
{
    float dotCalculation = normalRay.direction().dot(lightRay.direction());
    float dot = dotCalculation < 0 ? 0.0f : dotCalculation;
    Colour diffuseComponent = this->_kd * lightColour * dot;
    return diffuseComponent;
}

 Colour Object::specular(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, ParametrizedLine<float, 3> const &rayToCamera,  Colour lightColour)
 {
    vec3 reflectedRayOfLight = 2 * (lightRay.direction().dot(normalRay.direction())) * normalRay.direction() - lightRay.direction();
    Colour specularComponent = this->_ks * lightColour * pow(reflectedRayOfLight.dot(rayToCamera.direction()), this->_n);
    return specularComponent;
 }


