#include "sphere.h"

Sphere::~Sphere() {}

/**
 * @brief Sphere::intersectRay
 * @param ray
 * @return Boolean
 * Returns a boolean to see if the sphere has been intersected by a ray
 */
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

/**
 * @brief Sphere::intersectRayForShadow
 * @param ray
 * @return Boolean
 * Returns a boolean to check if the sphere should be in shadow based on a ray intersection
 */
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

/**
 * @brief Sphere::intersectRayValue
 * @param ray
 * @return Float
 * returns the float intersection point value for the sphere
 */
float Sphere::intersectRayValue(ParametrizedLine<float, 3> const &ray)
{
    float a = ray.direction().dot(ray.direction());
    float b = ray.direction().dot(ray.origin() - this->_mCentre);
    float c = (ray.origin() - this->_mCentre).dot(ray.origin() - this->_mCentre) - (this->_mRadius * this->_mRadius);

    float discriminant = (b * b) - (a * c);
    // if the discriminant is less than 0 return -1 as we don't care for it
    if(discriminant < 0)
    {
        return -1;
    }

    float t0 = (-b - sqrt(discriminant));
    float t1 = (-b + sqrt(discriminant));
    // if t1 is greater than t0 than set t1 value to t0 as we care only for the closest intersection
    if(t1 < t0)
        t0 = t1;
    return t0;
}

/**
 * @brief Sphere::getIntersectPoint
 * @param ray
 * @param pt
 * @return Vec3f (Eigen)
 * Return sphere vector point based on the sphere intersection point
 */
vec3 Sphere::getIntersectPoint(ParametrizedLine<float, 3> const &ray, float pt)
{
    vec3 hitPoint = ray.pointAt(pt);
    return hitPoint;
}

/**
 * @brief Sphere::getNormal
 * @param pt
 * @return ParametrizedLine<float, 3>
 * Return the normal of the sphere
 */
ParametrizedLine<float, 3> Sphere::getNormal(vec3 pt)
{
    typedef ParametrizedLine<float, 3> ray3;
    vec3 origin = this->_mCentre;
    vec3 direction = pt - origin;
    direction.normalize();
    return ray3(origin, direction);
}

/**
 * @brief Sphere::textureValue
 * @param pt
 * @return Vec3b (opencv)
 * Grabs the texture from this location and maps it onto a sphere
 */
Colour Sphere::textureValue(vec3 pt)
{
    float phi = acos((pt.z() - this->_mCentre.z())/this->_mRadius);
    float theta = atan2(pt.y() - this->_mCentre.y(), pt.x() - this->_mCentre.x());
    float u = phi / (2 * M_PI);
    float v = (M_PI - theta) / M_PI;
    return this->image.at<Colour>((int)(image.cols*u), (int)(image.rows*v));
}

