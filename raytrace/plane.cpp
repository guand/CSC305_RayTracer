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

