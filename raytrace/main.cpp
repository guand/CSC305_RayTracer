#include "icg_common.h"
#include "camera.h"
#include "imageplane.h"
#include "sphere.h"
#include "light.h"
#include <Eigen/Geometry>
#include <math.h>


#ifndef WITH_OPENCV
    #error OpenCV required for this exercise
#endif

typedef cv::Vec3b Colour;
Colour red() { return Colour(0, 0, 255); }
Colour blue() { return Colour(255, 0, 0); }
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
    Sphere sphere(vec3(0,0,1), 0.5, blue());
    Camera camera(vec3(0,0,-1));
    Light light(vec3(5,-3,-4), white());
    ImagePlane plane(vec3(-1,-1,-1), vec3(1,1,1), image.rows, image.cols);

    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            /// TODO: build primary rays
            // vec3 o = vec3(0,0,0);
            // vec3 d = vec3(1,1,0).normalized();
            // ray3 r(o,d);
            
            vec3 pt = plane.generatePixelPos(row, col);
            ray3 ray = camera.generateRay(pt);
            if(sphere.intersectRay(ray) <  0 && sphere.intersectRay(ray) != -1)
            {
                image(row, col) = sphere.getColour();
                float spherePt = sphere.intersectRay(ray);
                vec3 sphereHitPt = sphere.getIntersectPoint(ray, spherePt);
                ray3 rayToLight = light.generateRay(sphereHitPt);
                ray3 sphereNormal = sphere.getNormal(sphereHitPt);
                float dotCalculation = sphereNormal.direction().dot(rayToLight.direction());
                float dot = dotCalculation < 0 ? 0.0f : dotCalculation;
                ray3 rayToCamera = camera.rayToCamera(sphereHitPt);
                vec3 reflectedRayOfLight = 2 * (rayToLight.direction().dot(sphereNormal.direction())) * sphereNormal.direction() - rayToLight.direction();
                cv::Vec3b diffuseComponent = sphere.getKd() * sphere.getColour() * dot;

                cv::Vec3b ambientComponent = sphere.getKa() * sphere.getColour();
                cv::Vec3b specularComponent = sphere.getKs() * light.getColour() * pow(reflectedRayOfLight.dot(rayToCamera.direction()),sphere.getN());


                cv::Vec3b illumination = diffuseComponent + specularComponent + ambientComponent;
                std::cout << dot << std::endl;
                image(row, col) = illumination;

            }

       }
    }
    
    image.show();
    // image.save("output.png"); ///< Does not work on Windows!

    return EXIT_SUCCESS;
}
