
#include "object.h"

/**
 * @brief Object::ambient
 * @param lightColour
 * @return Ambient Component
 * This component returns the ambient component of the phong model
 */
Colour Object::ambient(Colour lightColour)
{
    Colour ambientComponent = (this->_colour).mul(lightColour);
    return ambientComponent;
}

/**
 * @brief Object::diffuse
 * @param normalRay
 * @param lightRay
 * @param lightColour
 * @return Diffuse Component
 * This function performs the diffuse component of the phong model
 */
Colour Object::diffuse(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, Colour lightColour)
{
    float dotCalculation = normalRay.direction().dot(lightRay.direction());
    float dot = dotCalculation < 0 ? 0.0f : dotCalculation;
    Colour diffuseComponent = (this->_kd).mul(lightColour) * dot;
    return diffuseComponent;
}

/**
 * @brief Object::specular
 * @param normalRay
 * @param lightRay
 * @param rayToCamera
 * @param lightColour
 * @return Specular Component
 * Tis component returns the specular component of the phong model
 */
Colour Object::specular(ParametrizedLine<float, 3> const &normalRay, ParametrizedLine<float, 3> const &lightRay, ParametrizedLine<float, 3> const &rayToCamera, Colour lightColour)
{
    vec3 reflectedRayOfLight = 2.0f * (lightRay.direction().dot(normalRay.direction())) * normalRay.direction() - lightRay.direction();
    Colour specularComponent = (this->_ks).mul(lightColour) * pow(max(0.0f, reflectedRayOfLight.dot(rayToCamera.direction())), this->_n);
    return specularComponent;
}


