#include "plane.h"

Plane::~Plane() {}

bool Plane::intersectRay(ParametrizedLine<float, 3> const &ray)
{
   float denom = ray.direction().dot(_mNormal);
   if(abs(denom) > 0.0001f)
   {
       float t = (_mPosition - ray.origin()).dot(_mNormal) / denom;
       if(t >= 0)
       {
           return true;
       }
   }
   return false;
}

