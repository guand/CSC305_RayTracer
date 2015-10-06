#include "icg_common.h"
#include "camera.h"
#include "imageplane.h"
#include "sphere.h"
#include "light.h"
#include "plane.h"
#include "object.h"
#include <Eigen/Geometry>
#include <math.h>


#ifndef WITH_OPENCV
    #error OpenCV required for this exercise
#endif

typedef cv::Vec3b Colour;
typedef cv::Vec3f Coefficient;
Colour red() { return Colour(0, 0, 255); }
Colour blue() { return Colour(255, 0, 0); }
Colour white() { return Colour(255, 255, 255); }
Colour black() { return Colour(0, 0, 0); }

Coefficient objectRed() { return Coefficient(0.0f, 0.0f, 1.0f); }
Coefficient objectBlue() { return Coefficient(1.0f, 0.0f, 0.0f); }
Coefficient objectWhite() { return Coefficient(1.0f, 1.0f, 1.0f); }
Coefficient objectBlack() { return Coefficient(0.0f, 0.0f, 0.0f); }

enum Special { TEXTURE, NO_TEXTURE };

struct MyImage{
    /// Data (not private for convenience)
    int cols = 600;
    int rows = 600;
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

int priorityObjectIndex(vector<float> intersectionObjects)
{
    int minimumValue;
    if(intersectionObjects.size() == 0)
    {
        return -1;
    } else if(intersectionObjects.size() == 1)
    {
        if(intersectionObjects.at(0) > 0)
        {
            return 0;
        }

        return -1;
    }
    float max = 0;
    for(int i = 0; i < intersectionObjects.size(); ++i)
    {
        if(max < intersectionObjects.at(i)) {
            max = intersectionObjects.at(i);
        }
    }
    if(max > 0)
    {
        for(int i = 0; i < intersectionObjects.size(); ++i)
        {
            if(intersectionObjects.at(i) > 0 && intersectionObjects.at(i) <= max)
            {
                max = intersectionObjects.at(i);
                minimumValue = i;
            }
        }
        return minimumValue;
    }
    return -1;
}


int main(int, char**){
    /// Rays and vectors represented with Eigen
    typedef Eigen::Vector3f vec3;
    typedef Eigen::ParametrizedLine<float, 3> ray3;
    
    MyImage image;
    
    /// Define camera, light, and image plane

    Camera camera(vec3(0,-1,-6));
    Light light(vec3(-4.0f, 3.0f, 1.0f), white());
    ImagePlane plane(vec3(-2,-2,-1), vec3(2,2,1), image.rows, image.cols);

    /// Define sphere and plane
    Sphere sphere(vec3(0,0,1), 0.4f, Coefficient(0.5f, 0.0f, 0.0f), TEXTURE);
    Sphere sphere2(vec3(-0.8f,1,1), 0.5f, Coefficient(0.5f, 0.25f, 0.25f), NO_TEXTURE);
    Plane floorPlane(vec3(1, 0, 0), vec3(1.6f, 0, 0), Coefficient(1, 1, 1), TEXTURE);
    floorPlane.setKd(Coefficient(0.2f, 0.2f, 0.2f));
    vector<Object*> scene;
    scene.push_back(dynamic_cast<Object*>(&floorPlane));
    scene.push_back(dynamic_cast<Object*>(&sphere));
    scene.push_back(dynamic_cast<Object*>(&sphere2));
    float accuracy = 0.00000001;
    float shade = 0.5f;
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            image(row, col) = black();
            vec3 pt = plane.generatePixelPos(row, col);
            ray3 ray = camera.generateRay(pt);
            vector<float> intersections;
            for(int i = 0; i < scene.size(); ++i)
            {
                intersections.push_back(scene.at(i)->intersectRayValue(ray));
            }

            int indexPriorityOfObject = priorityObjectIndex(intersections);
            if(indexPriorityOfObject == -1)
            {
                image(row, col) = black();
            } else
            {
                if(intersections.at(indexPriorityOfObject) > accuracy)
                {
                    if(Sphere* c = dynamic_cast<Sphere*>(scene[indexPriorityOfObject]))
                    {
                        bool shadow = false;
                        float spherePt = c->intersectRayValue(ray);
                        vec3 sphereHitPt = c->getIntersectPoint(ray, spherePt);
                        ray3 rayToLight = light.generateRay(sphereHitPt);
                        for(int i = 0; i < scene.size(); ++i)
                        {
                            if(Sphere* e = dynamic_cast<Sphere*>(scene[i]))
                            {
                                if(e != c)
                                {
                                    if(e->intersectRayForShadow(rayToLight))
                                    {
                                        shadow = true;
                                    }
                                }

                            }
                        }
                        ray3 sphereNormal = c->getNormal(sphereHitPt);
                        ray3 rayToCamera = camera.rayToCamera(sphereHitPt);
                        cv::Vec3b ambientComponent = c->ambient(light.getColour());
                        cv::Vec3b diffuseComponent = c->diffuse(sphereNormal, rayToLight, light.getColour());
                        cv::Vec3b specularComponent = c->specular(sphereNormal, rayToLight, rayToCamera, light.getColour());
                        cv::Vec3b illumination = diffuseComponent + ambientComponent + specularComponent;
                        cv::Vec3b textureComponent = c->textureValue(sphereHitPt);
                        if(c->getSpecial() == TEXTURE)
                        {
                            illumination = textureComponent + diffuseComponent + specularComponent;
                        }
                        if(shadow)
                        {   if(c->getSpecial() == TEXTURE)
                            {
                                illumination = textureComponent * shade;
                            } else
                            {
                                illumination = ambientComponent * shade;
                            }
                        }
                        image(row, col) = illumination;
                    } else if(Plane* b = dynamic_cast<Plane*>(scene[indexPriorityOfObject]))
                    {
                        bool shadow = false;
                        float planePt = b->intersectRayValue(ray);
                        vec3 planeHitPt = b->getIntersectPoint(ray, planePt);
                        ray3 rayToLight = light.generateRay(planeHitPt);
                        for(int i = 0; i < scene.size(); ++i)
                        {
                            if(Sphere* d = dynamic_cast<Sphere*>(scene[i]))
                            {
                                if(d->intersectRayForShadow(rayToLight))
                                {
                                    shadow = true;
                                }
                            }
                        }
                        ray3 planeNormal = b->getNormal(planeHitPt);
                        ray3 rayToCamera = camera.rayToCamera(planeHitPt);
                        cv::Vec3b ambientComponent = b->ambient(light.getColour());
                        cv::Vec3b diffuseComponent = b->diffuse(planeNormal, rayToLight, light.getColour());
//                        cv::Vec3b specularComponent = b->specular(planeNormal, rayToLight, rayToCamera, light.getColour());
                        cv::Vec3b illumination = diffuseComponent + ambientComponent;
                        if(b->getSpecial() == TEXTURE)
                        {
                            illumination = light.getColour().mul(b->checkerBoard(planeHitPt));
                        }

                        if(shadow)
                        {
                            if(b->getSpecial() == TEXTURE)
                            {
                                illumination = illumination * shade;
                            } else
                            {
                                illumination = ambientComponent * shade;
                            }
                        }
                        image(row, col) = illumination;
                    }
                }
            }
       }
    }
    
    image.show();
    // image.save("output.png"); ///< Does not work on Windows!

    return EXIT_SUCCESS;
}
