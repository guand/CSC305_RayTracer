#include "plane.h"

Plane::~Plane() {}

/**
 * @brief Plane::intersectRay
 * @param ray
 * @return Boolean
 * This function checks if a ray intersects the plane, returning a true or false value
 */
bool Plane::intersectRay(ParametrizedLine<float, 3> const &ray)
{
   float denom = ray.direction().dot(this->_mNormal);
   if(abs(denom) > 0.0001f)
   {
       float t = (this->_mPosition - ray.origin()).dot(this->_mNormal) / denom;
       if(t >= 0)
       {
           return true;
       }
   }
   return false;
}

/**
 * @brief Plane::intersectRayValue
 * @param ray
 * @return Float
 * This function checks if a ray intersects the plane, returning the intersecction point as a float value
 */
float Plane::intersectRayValue(ParametrizedLine<float, 3> const &ray)
{
   float denom = ray.direction().dot(this->_mNormal);
   if(abs(denom) > 0.0001f)
   {
       float t = (this->_mPosition - ray.origin()).dot(this->_mNormal) / denom;
       if(t >= 0)
       {
           return t;
       }
   }
   return -1;
}

/**
 * @brief Plane::getNormal
 * @param pt
 * @return ParametrizedLine<float, 3>
 * Returns a ray with the normalized direction
 */
ParametrizedLine<float, 3> Plane::getNormal(vec3 pt)
{
    typedef ParametrizedLine<float, 3> ray3;
    vec3 origin = this->_mNormal;
    vec3 direction = pt - origin;
    direction.normalize();
    return ray3(origin, direction);
}

/**
 * @brief Plane::getIntersectPoint
 * @param ray
 * @param pt
 * @return Vec3f (Eigen)
 * returns a three dimensional vector point of where the intersection happened on the plane
 */
vec3 Plane::getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt)
{
    vec3 hitPoint = ray.pointAt(pt);
    return hitPoint;
}

/**
 * @brief Plane::checkerBoard
 * @param pt
 * @return Vec3f (Opencv)
 * Performs a check to paint the floor in a checkerboard pattern
 */
Coefficient Plane::checkerBoard(vec3 pt)
{
    int square = (int)floor(pt.y()) + (int)floor(pt.z());
    if((square % 2) == 0)
    {
        return Coefficient(0, 0, 0);
    }
    return Coefficient(1, 1, 1);
}



