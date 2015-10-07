
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
Colour Object::diffuse(ParametrizedLine<float, 3> const &normalRay, vec3 sphereHitPt, vector<Light> lightScene)
{
    ParametrizedLine<float, 3> rayOfLight;
    Colour totalDiffuseComponent;
    for(int i; i < lightScene.size(); ++i)
    {
        rayOfLight = lightScene.at(i).generateRay(sphereHitPt);
        float dotCalculation = normalRay.direction().dot(rayOfLight.direction());
        float dot = dotCalculation < 0 ? 0.0f : dotCalculation;
        Colour diffuseComponent = (this->_kd).mul(lightScene.at(i).getColour()) * dot;
        totalDiffuseComponent += diffuseComponent;
    }

    return totalDiffuseComponent;
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
Colour Object::specular(ParametrizedLine<float, 3> const &normalRay, vec3 sphereHitPt, ParametrizedLine<float, 3> const &rayToCamera, vector<Light> lightScene)
{
    ParametrizedLine<float, 3> rayOfLight;
    Colour totalSpecularComponent;
    for(int i = 0; i < lightScene.size(); ++i)
    {
        rayOfLight = lightScene.at(i).generateRay(sphereHitPt);
        vec3 reflectedRayOfLight = 2.0f * (rayOfLight.direction().dot(normalRay.direction())) * normalRay.direction() - rayOfLight.direction();
        Colour specularComponent = (this->_ks).mul(lightScene.at(i).getColour()) * pow(max(0.0f, reflectedRayOfLight.dot(rayToCamera.direction())), this->_n);
        totalSpecularComponent += specularComponent;
    }

    return totalSpecularComponent;
}


