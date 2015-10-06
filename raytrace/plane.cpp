#include "plane.h"

Plane::~Plane() {}

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

ParametrizedLine<float, 3> Plane::getNormal(vec3 pt)
{
    typedef ParametrizedLine<float, 3> ray3;
    vec3 origin = this->_mNormal;
    vec3 direction = pt - origin;
    direction.normalize();
    return ray3(origin, direction);
}

vec3 Plane::getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt)
{
    vec3 hitPoint = ray.pointAt(pt);
    return hitPoint;
}

Coefficient Plane::checkerBoard(vec3 pt)
{
    int square = (int)floor(pt.y()) + (int)floor(pt.z());
    if((square % 2) == 0)
    {
        return Coefficient(0, 0, 0);
    }
    return Coefficient(1, 1, 1);
}



