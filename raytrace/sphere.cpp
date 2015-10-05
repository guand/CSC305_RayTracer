#include "sphere.h"

Sphere::~Sphere() {}

bool Sphere::intersectRay(ParametrizedLine<float, 3> const &ray)
{
    float a = ray.direction().dot(ray.direction());
    float b = ray.direction().dot(ray.origin() - this->_mCentre);
    float c = (ray.origin() - this->_mCentre).dot(ray.origin() - this->_mCentre) - (this->_mRadius * this->_mRadius);

    float discriminant = (b * b) - (a * c);

    if(discriminant < 0)
    {
        return false;
    }

    float t0 = (-b - sqrt(discriminant));
    float t1 = (-b + sqrt(discriminant));

    return true;
}

bool Sphere::intersectRayForShadow(ParametrizedLine<float, 3> const &ray)
{
    float a = ray.direction().dot(ray.direction());
    float b = ray.direction().dot(ray.origin() - this->_mCentre);
    float c = (ray.origin() - this->_mCentre).dot(ray.origin() - this->_mCentre) - (this->_mRadius * this->_mRadius);

    float discriminant = (b * b) - (a * c);

    if(discriminant < 0)
    {
        return false;
    }

    float t0 = (-b - sqrt(discriminant));
    float t1 = (-b + sqrt(discriminant));
    if(t1 > t0)
        t0 = t1;
    if(t0 <= 0)
    {
        return false;
    }

    return true;
}

float Sphere::intersectRayValue(ParametrizedLine<float, 3> const &ray)
{
    float a = ray.direction().dot(ray.direction());
    float b = ray.direction().dot(ray.origin() - this->_mCentre);
    float c = (ray.origin() - this->_mCentre).dot(ray.origin() - this->_mCentre) - (this->_mRadius * this->_mRadius);

    float discriminant = (b * b) - (a * c);

    if(discriminant < 0)
    {
        return -1;
    }

    float t0 = (-b - sqrt(discriminant));
    float t1 = (-b + sqrt(discriminant));

    if(t1 > t0)
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
    vec3 origin = this->_mCentre;
    vec3 direction = pt - origin;
    direction.normalize();
    return ray3(origin, direction);
}

