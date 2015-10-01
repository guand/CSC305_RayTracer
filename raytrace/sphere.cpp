#include "sphere.h"

Sphere::Sphere()
{

}

float Sphere::intersectRay(ParametrizedLine<float, 3> const &ray)
{
    float a = ray.direction().dot(ray.direction());
    float b = ray.direction().dot(ray.origin() - _mCentre);
    float c = (ray.origin() - _mCentre).dot(ray.origin() - _mCentre) - (_mRadius * _mRadius);

    float discriminant = (b * b) - (a * c);

    if(discriminant < 0)
    {
        return -1;
    }

    float t0 = (-b - sqrt(discriminant));
    float t1 = (-b + sqrt(discriminant));

    if(t1 < t0)
        t0 = t1;
    return t0;
}

vec3 Sphere::getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt)
{
    vec3 hitPoint = ray.pointAt(pt);
    return hitPoint;
}

ParametrizedLine<float, 3> Sphere::getNormal(vec3 pt)
{
    typedef ParametrizedLine<float, 3> ray3;
    vec3 origin = _mCentre;
    vec3 direction = origin - pt;
    direction.normalize();
    return ray3(origin, direction);
}

