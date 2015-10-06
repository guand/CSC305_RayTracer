#include "imageplane.h"

ImagePlane::~ImagePlane() {}

/**
 * @brief ImagePlane::generatePixelPos
 * @param i
 * @param j
 * @return Vec3f (Eigen)
 * This sets up the image plane
 */
vec3 ImagePlane::generatePixelPos(int i, int j)
{
    float u = this->_l + (this->_r - this->_l) * (i + 0.5f) / this->_mRows;
    float v = this->_b + (this->_t - this->_b) * (j + 0.5f) / this->_mCols;
    return vec3(u, v, 0);
}


