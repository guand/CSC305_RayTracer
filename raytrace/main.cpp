#include "icg_common.h"
#include "camera.h"
#include "imageplane.h"
#include "sphere.h"
#include <Eigen/Geometry>

#ifndef WITH_OPENCV
    #error OpenCV required for this exercise
#endif

typedef cv::Vec3b Colour;
Colour red() { return Colour(255, 0, 0); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

struct MyImage{
    /// Data (not private for convenience)
    int cols = 500;
    int rows = 500;
    ///  Channel with [0..255] range image (aka uchar)
    cv::Mat image = cv::Mat(rows, cols, CV_8UC3, cv::Scalar(255,255,255));

    Colour& operator()(int row, int col)
    {
        assert(row >= 0 && row < rows);
        assert(col >= 0 && col < cols);
        return image.at<cv::Vec3b>(cv::Point(col, row));
    }

    /// Displays the image and pauses program execution (until key pressed)
    void show(){
        /// Display the image
        cv::imshow("image", image);
        cv::waitKey(0);
    }

    /// Use files with "*.png" extension
    void save(const std::string& filename){
        cv::imwrite(filename, image);
    }
};


int main(int, char**){
    /// Rays and vectors represented with Eigen
    typedef Eigen::Vector3f vec3;
    typedef Eigen::ParametrizedLine<float, 3> ray3;
    
    MyImage image;
    
    /// TODO: define camera position and sphere position here
    Sphere sphere(vec3(0,0,1), 0.9f);
    Camera camera(vec3(0,0,-1));

    ImagePlane plane(vec3(-1,-1,-1), vec3(1,1,1), image.rows, image.cols);

    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            /// TODO: build primary rays
            // vec3 o = vec3(0,0,0);
            // vec3 d = vec3(1,1,0).normalized();
            // ray3 r(o,d);
            
            vec3 pt = plane.generatePixelPos(row, col);
            ray3 ray = camera.generateRay(pt);

            image(row, col) = (sphere.intersectRay(ray)) ? white() : black();
       }
    }
    
    image.show();
    // image.save("output.png"); ///< Does not work on Windows!

    return EXIT_SUCCESS;
}
